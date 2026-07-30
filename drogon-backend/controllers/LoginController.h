#pragma once
#include <drogon/HttpController.h>

class LoginController : public drogon::HttpController<LoginController> {
public:
    METHOD_LIST_BEGIN
    ADD_METHOD_TO(LoginController::login,  "/api/login",  drogon::Post);
    ADD_METHOD_TO(LoginController::logout, "/api/logout", drogon::Post);
    ADD_METHOD_TO(LoginController::me,     "/api/me",     drogon::Get);
    METHOD_LIST_END

    void login  (const drogon::HttpRequestPtr&, std::function<void(const drogon::HttpResponsePtr&)>&&);
    void logout (const drogon::HttpRequestPtr&, std::function<void(const drogon::HttpResponsePtr&)>&&);
    void me     (const drogon::HttpRequestPtr&, std::function<void(const drogon::HttpResponsePtr&)>&&);

    // token helper
    static std::string makeToken(const std::string& username, const std::string& role);
    static bool parseToken(const std::string& token, std::string& username, std::string& role);
};