#include <drogon/drogon.h>
#include "filters/Cors.h" 

int main() {
    drogon::app()
        .addListener("0.0.0.0", 5555)
        .setLogLevel(trantor::Logger::kWarn)
        .setThreadNum(16)
        .loadConfigFile("../config.json");  

    std::cout<<"Starting Drogon server..." << std::endl;
    drogon::app().run();
    return 0;
}
