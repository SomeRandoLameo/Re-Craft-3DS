#pragma once

#include "../mclib.h"
#include "../common/Json.h"

#include <map>
#include <memory>

namespace mc {
namespace util {

typedef std::map<std::string, std::string> Headers;

struct HTTPResponse {
    int status;
    Headers headers;
    std::string body;
};

class HTTPClient {
public:
    HTTPClient() = default;
    virtual ~HTTPClient() { }
    HTTPClient(HTTPClient&& other) = default;
    HTTPClient& operator=(HTTPClient&& rhs) = default;

    virtual HTTPResponse Get(const std::string& url, Headers headers = {}) = 0;
    virtual HTTPResponse Post(const std::string& url, const std::string& data, Headers headers = {}) = 0;
    virtual HTTPResponse PostJSON(const std::string& url, const std::string& data, Headers headers = {}) = 0;
    virtual HTTPResponse PostJSON(const std::string& url, const json& json, Headers headers = {}) = 0;
};

class CurlHTTPClient : public HTTPClient {
private:
    class Impl;
    std::unique_ptr<Impl> m_Impl;
public:
    CurlHTTPClient();
    ~CurlHTTPClient();

    CurlHTTPClient(const CurlHTTPClient& other);
    CurlHTTPClient& operator=(const CurlHTTPClient& rhs);
    CurlHTTPClient(CurlHTTPClient&& other);
    CurlHTTPClient& operator=(CurlHTTPClient&& rhs);

    HTTPResponse Get(const std::string& url, Headers headers = {});
    HTTPResponse Post(const std::string& url, const std::string& data, Headers headers = {});
    HTTPResponse PostJSON(const std::string& url, const std::string& data, Headers headers = {});
    HTTPResponse PostJSON(const std::string& url, const json& json, Headers headers = {});
};

} // ns util
} // ns mc