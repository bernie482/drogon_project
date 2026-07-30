#include "LoginController.h"
#include "AuthHelper.h"
#include <drogon/orm/DbClient.h>
#include <drogon/orm/Exception.h>

using namespace drogon;
using namespace drogon::orm;

// Helper -> Response JSON
static HttpResponsePtr jsonResp(int status, Json::Value body) {
    auto resp = HttpResponse::newHttpJsonResponse(body);
    resp->setStatusCode((HttpStatusCode)status);
    resp->addHeader("Access-Control-Allow-Origin", "*");
    resp->addHeader("Access-Control-Allow-Headers", "Content-Type,Authorization");
    return resp;
}

void LoginController::login(
    const HttpRequestPtr& req,
    std::function<void(const HttpResponsePtr&)>&& callback)
{
    auto json = req->getJsonObject();
    if (!json) {
        Json::Value ret;
        ret["success"] = false;
        ret["message"] = "Invalid JSON";
        callback(jsonResp(400, ret));
        return;
    }

    auto username = (*json)["username"].asString();
    auto password = AuthHelper::sha256((*json)["password"].asString()); // Change to SHA256
    LOG_INFO << "SHA256 password: " << password;

    // Get User Info from DB
    auto db = app().getDbClient();
    db->execSqlAsync(
        "SELECT id, username, role FROM users WHERE username=$1 AND password=$2",
        [callback](const Result& rows) {
            if (rows.empty()) {
                Json::Value ret;
                ret["success"] = false;
                ret["message"] = "Incorrect account or password";
                callback(jsonResp(401, ret));
                return;
            }

            auto row      = rows[0];
            auto username = row["username"].as<std::string>();
            auto role     = row["role"].as<std::string>();
            auto token    = drogon::utils::base64Encode(username + ":" + role);

            Json::Value ret;
            ret["success"]          = true;
            ret["token"]            = token;
            ret["user"]["id"]       = row["id"].as<int>();
            ret["user"]["username"] = username;
            ret["user"]["role"]     = role;
            callback(jsonResp(200, ret));
        },
        [callback](const DrogonDbException& e) {
            LOG_ERROR << "DB error: " << e.base().what();
            Json::Value ret;
            ret["success"] = false;
            ret["message"] = "DB error";
            callback(jsonResp(500, ret));
        },
        username, password
    );
}

void LoginController::logout(
    const HttpRequestPtr& req,
    std::function<void(const HttpResponsePtr&)>&& callback)
{
    // JWT is stateless, logout only need to remove token from frontend
    Json::Value ret;
    ret["success"] = true;
    ret["message"] = "Logout";
    callback(jsonResp(200, ret));
}

void LoginController::me(
    const HttpRequestPtr& req,
    std::function<void(const HttpResponsePtr&)>&& callback)
{
    std::string username, role;

    if (!AuthHelper::parseAuth(req, username, role)) {
        Json::Value ret;
        ret["success"] = false;
        ret["message"] = "Invalid token";
        callback(jsonResp(401, ret));
    }

    LOG_INFO << "token 解析成功 - username: " << username << ", role: " << role;

    Json::Value ret;
    ret["success"]          = true;
    ret["user"]["username"] = username;
    ret["user"]["role"]     = role;
    callback(jsonResp(200, ret));
}