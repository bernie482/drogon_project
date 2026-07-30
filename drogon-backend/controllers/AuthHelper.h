#pragma once
#include <drogon/HttpRequest.h>
#include <drogon/utils/Utilities.h>
#include <string>

class AuthHelper {
public:
    static bool parseAuth(const drogon::HttpRequestPtr& req,
                          std::string& username,
                          std::string& role);
    static std::string sha256(const std::string& input);
};