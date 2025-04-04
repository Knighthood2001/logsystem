#include "logsystem.h"

int main()
{
    try {
        Logger logger("log.txt");

        logger.log(LogLevel::INFO, "Starting application.");

        int user_id = 42;
        std::string action = "login";
        double duration = 3.5;
        std::string world = "World";

        logger.log(LogLevel::INFO, "User {} performed {} in {} seconds.", user_id, action, duration);
        logger.log(LogLevel::DEBUG, "Hello {}", world);
        logger.log(LogLevel::WARN, "This is a message without placeholders.");
        logger.log(LogLevel::ERROR, "Multiple placeholders: {}, {}, {}.", 1, 2, 3);
    }
    catch(const std::exception& e){ 
        std::cerr << "error is " << e.what() << std::endl;
    }

	return 0;
}
