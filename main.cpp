#include <iostream>
#include <array>
#include <cstdio>
#include <fstream>
#include <string>

int main(int argc, char** argv) {
    Url url{"47.111.82.206" + "/url_post.html"};
    Response response = cpr::Post(url, Payload{{"x", "5"}});
    std::cout<<response.text;
    std::cout<<response.url;
    std::cout<<response.header["content-type"];
    std::cout<<response.status_code;
    std::cout<<response.error.code;
}
