/**
 *
 *  Cors.h
 *
 */

#pragma once

#include <drogon/HttpFilter.h>
using namespace drogon;


class Cors : public HttpFilter<Cors>
{
  public:
    Cors() {}
    void doFilter(const HttpRequestPtr &req,
                  FilterCallback &&fcb,
                  FilterChainCallback &&fccb) override;
};

