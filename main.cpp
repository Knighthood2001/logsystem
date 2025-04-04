#include "logsystem.h"

int main()
{
    Logger logger("log.txt");

    logger.log("Starting application.");

    int user_id = 42;
    std::string action = "login";
    double duration = 3.5;
    std::string world = "World";

    logger.log("User {} performed {} in {} seconds.", user_id, action, duration);
    logger.log("Hello {}", world);
    logger.log("This is a message without placeholders.");
    logger.log("Multiple placeholders: {}, {}, {}.", 1, 2, 3);
	return 0;
}
