#include <iostream>
#include <filesystem>
#include <drogon/drogon.h>
#include "controllers/recognize.hpp"

namespace fs = std::filesystem;
using namespace std;

int main() {
    drogon::app()
            .loadConfigFile("config.json")
            .run();
}
