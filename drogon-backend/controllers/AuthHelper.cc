#include "AuthHelper.h"
#include <openssl/sha.h>
#include <sstream>
#include <iomanip>

bool AuthHelper::parseAuth(const drogon::HttpRequestPtr& req,
                           std::string& username,
                           std::string& role)
{
    auto auth = req->getHeader("Authorization");
    if (auth.empty() || auth.substr(0, 7) != "Bearer ") return false;

    auto token = auth.substr(7);
    try {
        auto decoded = drogon::utils::base64Decode(token);
        auto pos = decoded.find(':');
        if (pos == std::string::npos) return false;
        username = decoded.substr(0, pos);
        role     = decoded.substr(pos + 1);
        return true;
    } catch(...) {
        return false;
    }
}

// SHA256 Hash
std::string AuthHelper::sha256(const std::string& input) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256(
        reinterpret_cast<const unsigned char*>(input.c_str()),
        input.size(),
        hash
    );

    std::ostringstream ss;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    }
    return ss.str();
}
