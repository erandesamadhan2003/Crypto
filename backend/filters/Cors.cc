
#include "Cors.h"

using namespace drogon;

void Cors::doFilter(const HttpRequestPtr &req, FilterCallback &&fcb, FilterChainCallback &&fccb) {

    // Get Origin from request headers
    std::string origin = req->getHeader("Origin");

    // Handle preflight options request 
    if (req->getMethod() == HttpMethod::Options) {
        auto res = drogon::HttpResponse::newHttpResponse();

        res->addHeader("Access-Control-Allow-Origin", origin.empty() ? "*" : origin);
        res->addHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS, PATCH");
        res->addHeader("Access-Control-Allow-Headers", "Content-Type, Authorization, X-Requested-With, Accept, Origin, X-CSRF-Token");
        res->addHeader("Access-Control-Allow-Credentials", "true");
        res->addHeader("Access-Control-Max-Age", "86400");

        res->setStatusCode(k200OK);
        fcb(res);
        return;
    }

    if (!origin.empty()) {
        req->getAttributes()->insert("cors_origin", origin);
    }
    
    // Continue with the filter chain
    fccb();
}
