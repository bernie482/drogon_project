# pragma once
#include <drogon/HttpController.h>

class UserController : public drogon::HttpController<UserController> {
public:
    METHOD_LIST_BEGIN
    ADD_METHOD_TO(UserController::registerUser, "/api/auth/register",   drogon::Post);
    ADD_METHOD_TO(UserController::getUsers,     "/api/users",           drogon::Get);
    ADD_METHOD_TO(UserController::updateRole,   "/api/users/{id}/role", drogon::Put);
    ADD_METHOD_TO(UserController::deleteUser,   "/api/users/{id}",      drogon::Delete);
    METHOD_LIST_END

    void registerUser(const drogon::HttpRequestPtr&, std::function<void(const drogon::HttpResponsePtr&)>&&);
    void getUsers    (const drogon::HttpRequestPtr&, std::function<void(const drogon::HttpResponsePtr&)>&&);
    void updateRole  (const drogon::HttpRequestPtr&, std::function<void(const drogon::HttpResponsePtr&)>&&, int id);
    void deleteUser  (const drogon::HttpRequestPtr&, std::function<void(const drogon::HttpResponsePtr&)>&&, int id);
};