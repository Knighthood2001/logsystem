// logsystem.h: 标准系统包含文件的包含文件
// 或项目特定的包含文件。

#pragma once

#include <iostream>
#include <queue>
#include <string>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <fstream>
#include <atomic>
#include <sstream>
#include <vector>
#include <stdexcept>
#include "utils.h"

// 辅助函数。实现将单个参数转化成字符串
//使用模板，因为这个参数可以是各种类型
// 方法一：不用特化版本实现
template <typename t>
std::string to_string_helper(t && arg) {//这里使用万能引用，因为你不知道这里
	std::ostringstream oss;
	oss << std::forward<t>(arg);
	return oss.str(); 
}

//template <typename T, typename = std::enable_if_t<!std::is_convertible<T, std::string>::value>>
//std::string to_string_helper(T&& arg) {
//	std::ostringstream oss;
//	oss << std::forward<T>(arg);
//	return oss.str();
//}
//
//template <typename T>
//std::string to_string_helper(T&& arg, typename std::enable_if<std::is_convertible<T, std::string>::value, int>::type = 0) {//这里使用万能引用，因为你不知道这里
//	return std::forward<T>(arg);
//}

class LogQueue {
public:
	void push(const std::string&msg);
	bool pop(std::string& msg);
	void shutdown();
private:
	std::queue<std::string> queue_;// 队列
	std::mutex mutex_;//保证线程安全，使用互斥锁
	std::condition_variable cond_var_; //要保证同步
	bool is_shutdown_ = false;

};
enum class LogLevel {
	INFO, DEBUG, WARN, ERROR
};

class Logger {
public:
	Logger(const std::string& filename);
	~Logger();
	//模板函数打印
	template <typename ... Args>
	void log(LogLevel loglevel, const std::string& format, Args&& ... args) {
		std::string level_str;
		switch (loglevel)
		{
		case LogLevel::INFO:
			level_str = "[INFO] ";
			break;
		case LogLevel::DEBUG:
			level_str = "[DEBUG] ";
			break;
		case LogLevel::WARN:
			level_str = "[WARN] ";
			break;
		case LogLevel::ERROR:
			level_str = "[ERROR] ";
			break;
		default:
			break;
		}
		log_queue_.push(level_str + formatMessage(format, std::forward<Args>(args)...));
	}
	//打日志，只需要将这些参数，替换到format中就可以
	//log("Hello {}, my name is {}", "zack", "jay"); 
private:
	LogQueue log_queue_;
	std::thread worker_thread_;
	std::ofstream log_file_;
	std::atomic<bool> exit_flag_;

	template <typename ... Args>
	std::string formatMessage(const std::string format, Args&& ... args) {
		std::vector<std::string> args_strings = {to_string_helper(std::forward<Args>(args))...};
		std::ostringstream oss;
		size_t arg_index = 0; //这是format中{}数量
		size_t pos = 0; // 这是要插入的位置
		size_t placeholder = format.find("{}",pos);
		while (placeholder != std::string::npos) {//如果没有在format中找到{}
			oss << format.substr(pos, placeholder-pos); // 把前面部分先插入进去
			if (arg_index < args_strings.size()) { //如果format中{}数量小于ages的数量
				oss << args_strings[arg_index++];// 先arg_index,然后++
			}
			else {//如果format中{}数量大于ages的数量，那就照常插入{}
				oss << "{}";
			}
			pos = placeholder + 2;
			placeholder = format.find("{}", pos);
		}
		oss << format.substr(pos);
		while (arg_index < args_strings.size()) {
			oss << args_strings[arg_index++];
		}
		return "[" + getCurrentTime() + "] " + oss.str();
	}

};





