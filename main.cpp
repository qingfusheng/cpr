#include <gtest/gtest.h>
#include<iostream>
#include <array>
#include <cstdio>
#include <fstream>
#include <string>
#include "httpServer.hpp"
namespace cpr;
static cpr::HttpServer* server = new cpr::HttpServer();
TEST(UrlEncodedPostTests, UrlPostSingleTest) {
    Url url{server->GetBaseUrl() + "/url_post.html"};
    Response response = cpr::Post(url, Payload{{"x", "5"}});
    std::string expected_text{
            "{\n"
            "  \"x\": 5\n"
            "}"};
    std::cout<<response.text;
    std::cout<<response.url;
    std::cout<<response.header["content-type"];
    std::cout<<response.status_code;
    std::cout<<response.error.code;
}

int main(int argc, char** argv) {
    return RUN_ALL_TESTS();
}
