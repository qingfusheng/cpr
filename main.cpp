#include <iostream>
#include <array>
#include <cstdio>
#include <fstream>
#include <string>
#include "include/cpr/cpr.h"
int main(int argc, char** argv) {
    cpr::Url url{"http://47.111.82.206:8080/VIID/System/Register"};
    cpr::Response response = cpr::Post(url, cpr::Payload{{"x", "5"}});
    std::cout<<response.text;
    std::cout<<response.url;
    std::cout<<response.header["content-type"];
    std::cout<<response.status_code;
    if(response.error.code==cpr::ErrorCode::OK)
        std::cout<<"OK";
    else
        std::cout<<"ERROR";
}
