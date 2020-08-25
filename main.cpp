#include <unistd.h>
#include <iostream>
#include <array>
#include <cstdio>
#include <fstream>
#include <string>
#include "include/cpr/cpr.h"
void Register();
int main(int argc, char** argv) {
	while(1){
		Register();
		sleep(1);
	}
}
void Register()
{
    cpr::Url url{"http://47.111.82.206:8088/VIID/System/Register"};
    cpr::Response response = cpr::Post(url, 
                             cpr::Payload{{"x", "5"}},
                             cpr::Header{{"Content-Type","application/json;charset=utf-8"}});
    std::cout<<response.text<<std::endl;
    std::cout<<response.url<<std::endl;
    std::cout<<response.header["content-type"]<<std::endl;
    std::cout<<response.status_code<<std::endl;
    if(response.error.code==cpr::ErrorCode::OK)
        std::cout<<"OK"<<std::endl;
    else
        std::cout<<"ERROR"<<std::endl;
}
