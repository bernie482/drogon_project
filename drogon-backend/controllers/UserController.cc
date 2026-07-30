#include "UserController.h"
#include "AuthHelper.h"
#include <drogon/orm/DbClient.h>
#include <drogon/orm/Exception.h>

using namespace drogon;
using namespace drogon::orm;

static HttpResponsePtr jsonResp(int status, Json::Value body) {
    auto resp = HttpResponse::newHttpJsonResponse(body);
    resp->setStatusCode((HttpStatusCode)status);
    resp->addHeader("Access-Control-Allow-Origin", "*");
    resp->addHeader("Access-Control-Allow-Headers", "Content-Type,Authorization");
    return resp;
}

// Create new account (superadmin only)
void UserController::registerUser(
    const HttpRequestPtr& req,
    std::function<void(const HttpResponsePtr&)>&& callback)
{
    // Check permissions
    std::string username, role;
    if (!AuthHelper::parseAuth(req, username, role)) {
        Json::Value ret;
        ret["success"] = false;
        ret["message"] = "Unauthorized";
        callback(jsonResp(401, ret));
        return;
    }

    if (role != "superadmin") {
        Json::Value ret;
        ret["success"] = false;
        ret["message"] = "Premession denied";
        callback(jsonResp(403, ret));
        return;
    }

    // Get Request data
    auto jsonBody = req->getJsonObject();
    if (!jsonBody) {
        Json::Value ret;
        ret["success"] = false;
        ret["message"] = "Invalid JSON";
        callback(jsonResp(400, ret));
        return;
    }

    auto newUsername = (*jsonBody)["username"].asString();
    auto newPassword = AuthHelper::sha256((*jsonBody)["password"].asString());
    auto newRole     = (*jsonBody)["role"].asString();

    // Verify role
    if (newRole != "superadmin" && newRole != "admin" && newRole != "user") {
        Json::Value ret;
        ret["success"] = false;
        ret["message"] = "Invalid role";
        callback(jsonResp(400, ret));
        return;
    }

    auto db = app().getDbClient();
    db->execSqlAsync(
        "INSERT INTO users (username, password, role) VALUES ($1, $2, $3) RETURNING id",
        [callback, newUsername, newRole](const Result& rows) {
            Json::Value ret;
            ret["success"]          = true;
            ret["message"]          = "User created";
            ret["user"]["id"]       = rows[0]["id"].as<int>();
            ret["user"]["username"] = newUsername;
            ret["user"]["role"]     = newRole;
            callback(jsonResp(201, ret));
        },
        [callback](const DrogonDbException& e) {
            LOG_ERROR << "DB error: " << e.base().what();
            Json::Value ret;
            ret["success"] = false;
            ret["message"] = "Username already exists";
            callback(jsonResp(409, ret));
        },
        newUsername, newPassword, newRole
    );
}

// Get User List (admin)
void UserController::getUsers(
    const HttpRequestPtr& req,
    std::function<void(const HttpResponsePtr&)>&& callback)
{
    std::string username, role;
    if (!AuthHelper::parseAuth(req, username, role)) {
        Json::Value ret;
        ret["success"] = false;
        ret["message"] = "Unauthorized";
        callback(jsonResp(401, ret));
        return;
    }

    if (role != "admin" && role != "superadmin") {
        Json::Value ret;
        ret["success"] = false;
        ret["message"] = "Permission denied";
        callback(jsonResp(403, ret));
        return;
    }

    auto db = app().getDbClient();
    db->execSqlAsync(
        "SELECT id, username, role FROM users ORDER BY id",
        [callback](const Result& rows) {
            Json::Value users(Json::arrayValue);
            for (const auto& row : rows) {
                Json::Value user;
                user["id"]       = row["id"].as<int>();
                user["username"] = row["username"].as<std::string>();
                user["role"]     = row["role"].as<std::string>();
                users.append(user);
            }
            Json::Value ret;
            ret["success"] = true;
            ret["users"]   = users;
            callback(jsonResp(200, ret));
        },
        [callback](const DrogonDbException& e) {
            LOG_ERROR << "DB error: " << e.base().what();
            Json::Value ret;
            ret["success"] = false;
            ret["message"] = "DB error";
            callback(jsonResp(500, ret));
        }
    );
}

// Change Role (superadmin only)
void UserController::updateRole(
    const HttpRequestPtr& req,
    std::function<void(const HttpResponsePtr&)>&& callback, int id)
{
    std::string username, role;
    if (!AuthHelper::parseAuth(req, username, role)) {
        Json::Value ret;
        ret["success"] = false;
        ret["message"] = "Unauthorized";
        callback(jsonResp(401, ret));
        return;
    }

    if (role != "superadmin") {
        Json::Value ret;
        ret["success"] = false;
        ret["message"] = "Permission denied";
        callback(jsonResp(403, ret));
        return;
    }

    auto jsonBody = req->getJsonObject();
    if (!jsonBody) {
        Json::Value ret;
        ret["success"] = false;
        ret["message"] = "Invalid JSON";
        callback(jsonResp(400, ret));
        return;
    }

    auto newRole = (*jsonBody)["role"].asString();

    if (newRole != "superadmin" && newRole != "admin" && newRole != "user") {
        Json::Value ret;
        ret["success"] = false;
        ret["message"] = "Invalid role";
        callback(jsonResp(400, ret));
        return;
    }

    auto db = app().getDbClient();
    db->execSqlAsync(
        "UPDATE users SET role=$1 WHERE id=$2",
        [callback, id, newRole](const Result& rows) {
            Json::Value ret;
            ret["success"] = true;
            ret["message"] = "Role updated";
            callback(jsonResp(200, ret));
        },
        [callback](const DrogonDbException& e) {
            LOG_ERROR << "DB error: " << e.base().what();
            Json::Value ret;
            ret["success"] = false;
            ret["message"] = "DB error";
            callback(jsonResp(500, ret));
        },
        newRole, id
    );
}

// Delete users (superadmin only)
void UserController::deleteUser(
    const HttpRequestPtr& req,
    std::function<void(const HttpResponsePtr&)>&& callback, int id)
{
    std::string username, role;
    if (!AuthHelper::parseAuth(req, username, role)) {
        Json::Value ret;
        ret["success"] = false;
        ret["message"] = "Unauthorized";
        callback(jsonResp(401, ret));
        return;
    }

    if (role != "superadmin") {
        Json::Value ret;
        ret["success"] = false;
        ret["message"] = "Permission denied";
        callback(jsonResp(403, ret));
        return;
    }

    auto db = app().getDbClient();
    db->execSqlAsync(
        "DELETE FROM users WHERE id=$1",
        [callback](const Result& rows) {
            Json::Value ret;
            ret["success"] = true;
            ret["message"] = "User deleted";
            callback(jsonResp(200, ret));
        },
        [callback](const DrogonDbException& e) {
            LOG_ERROR << "DB error: " << e.base().what();
            Json::Value ret;
            ret["success"] = false;
            ret["message"] = "DB error";
            callback(jsonResp(500, ret));
        },
        id
    );
}