// logsystem.cpp: 定义应用程序的入口点。
//

#include "logsystem.h"

void LogQueue::push(const std::string& msg)
{
	std::lock_guard<std::mutex> lock(mutex_);
	queue_.push(msg); //向队列中添加消息
	cond_var_.notify_one();
}

bool LogQueue::pop(std::string& msg)
{
	std::unique_lock<std::mutex> lock(mutex_);
	// 使用while循环防止虚假唤醒
	//如果lambda返回false，那么就会挂起，如果返回true，就会继续往下走
	cond_var_.wait(lock, [this]() -> bool {  // wait 方法需要一个 std::unique_lock 对象作为参数。
		return !queue_.empty() || is_shutdown_;
	});
	//消费逻辑
	if (is_shutdown_ && queue_.empty()) {
		return false;
	}
	msg = queue_.front();
	queue_.pop();
	return true;
}

void LogQueue::shutdown()
{
	std::lock_guard<std::mutex> lock(mutex_);
	is_shutdown_ = true;
	cond_var_.notify_all();
}


Logger::Logger(const std::string& filename): log_file_(filename, std::ios::out | std::ios::app), exit_flag_(false)
{
	if (!log_file_.is_open()) {//如果文件没有打开
		throw std::runtime_error("无法打开日志文件");
	}

	worker_thread_ = std::thread([this]()->void {
		std::string msg;
		while (log_queue_.pop(msg)) {
			log_file_ << msg << std::endl;
		}
	});
	 
}

Logger::~Logger()  // 析构的话，退出标志设置为true，将队列关闭 
{
	exit_flag_ = true;
	log_queue_.shutdown();//关闭队列
	//主线程退出的话，子线程还没来得及退出，就会出现崩溃
	//主线程要等待子线程 退出，用joinable
	if (worker_thread_.joinable()) {
		worker_thread_.join();
	}
	//判断文件是不是打开的
	if (log_file_.is_open()) {
		log_file_.close();
	}

}
