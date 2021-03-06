#include <unistd.h>
#include <iostream>
#include <array>
#include <cstdio>
#include <fstream>
#include <string>
#include "cpr/cpr.h"
#include "json.hpp"
using nlohmann::json;
void Register1(cpr::Session &session);
void Register2(cpr::Session &session);
void Register3(cpr::Session &session);
void Register4(cpr::Session &session);
/*void Register5();*/
void Register6(cpr::Session &session);
void Register7(cpr::Session &session);
void Register8(cpr::Session &session);
int main(int argc, char** argv) {
        cpr::Session session;
	while(1){
		Register1(session);sleep(1);
		Register2(session);sleep(1);
		Register3(session);sleep(1);
		Register4(session);sleep(1);
		/*Register5();sleep(1);*/
		Register6(session);sleep(1);
		Register7(session);sleep(1);
		Register8(session);sleep(1);
	}
}
void Register1(cpr::Session &session)
{
    auto j = R"(
        {
            "RegisterObject":{
                "DeviceID":"31000000001310910561"
            },
            "ProtocolVersion":"2.0"
        }
    )"_json;
    std::string j_string = j.dump();
    cpr::Url url{"http://47.111.82.206:8088/VIID/System/Register"};
    session.SetUrl(url);
   /* session.SetOption(cpr::Body{"{\"RegisterObject\":{\"DeviceID\":\"31000000001310910561\",\"ProtocolVersion\":\"2.0\"}}"});*/
    session.SetOption(cpr::Body{j_string});
    session.SetOption(cpr::Header{{"Content-Type","application/json;charset=utf-8"}});
    cpr::Response response = session.Post();
    std::cout<<response.text<<std::endl;
    std::cout<<response.url<<std::endl;
    std::cout<<response.header["content-type"]<<std::endl;
    std::cout<<response.status_code<<std::endl;
    if(response.error.code==cpr::ErrorCode::OK)
        std::cout<<"OK"<<std::endl;
    else
        std::cout<<"ERROR"<<std::endl;
}
void Register2(cpr::Session &session)
{
    auto j = R"(
        {
            "RegisterObject":{
                "DeviceID":"31000000001310910561"
            },
            "ProtocolVersion":"2.0"
        }
    )"_json;
    std::string j_string = j.dump();
    cpr::Url url{"http://47.111.82.206:8088/VIID/System/UnRegister"};
    session.SetUrl(url);
    session.SetOption(cpr::Header{{"Content-Type","application/json;charset=utf-8"}});
    session.SetOption(cpr::Body{j_string});
    cpr::Response response = session.Post();
    std::cout<<response.text<<std::endl;
    std::cout<<response.url<<std::endl;
    std::cout<<response.header["content-type"]<<std::endl;
    std::cout<<response.status_code<<std::endl;
    if(response.error.code==cpr::ErrorCode::OK)
        std::cout<<"OK"<<std::endl;
    else
        std::cout<<"ERROR"<<std::endl;
}
void Register3(cpr::Session &session)
{
    auto j = R"({
        "KeepaliveObject":{
            "DeviceID":"31000000001310910561"
        }
    })"_json;
    std::string j_string = j.dump();
    cpr::Url url{"http://47.111.82.206:8088/VIID/System/Keepalive"};
    session.SetUrl(url);
    session.SetOption(cpr::Header{{"Content-Type","application/json;charset=utf-8"}});
    session.SetOption(cpr::Body{j_string});
    cpr::Response response = session.Post();
    std::cout<<response.text<<std::endl;
    std::cout<<response.url<<std::endl;
    std::cout<<response.header["content-type"]<<std::endl;
    std::cout<<response.status_code<<std::endl;
    if(response.error.code==cpr::ErrorCode::OK)
        std::cout<<"OK"<<std::endl;
    else
        std::cout<<"ERROR"<<std::endl;
}
void Register4(cpr::Session &session)
{
    auto j = R"({
        "KeepaliveObject":{
            "DeviceID":"31000000001310910561"
        }
    })"_json;
    std::string j_string = j.dump();
    cpr::Url url{"http://47.111.82.206:8088/VIID/Faces"};
    session.SetUrl(url);
    session.SetOption(cpr::Header{{"Content-Type","application/json;charset=utf-8"}});
    session.SetOption(cpr::Body{j_string});
    cpr::Response response = session.Post();
    std::cout<<response.text<<std::endl;
    std::cout<<response.url<<std::endl;
    std::cout<<response.header["content-type"]<<std::endl;
    std::cout<<response.status_code<<std::endl;
    if(response.error.code==cpr::ErrorCode::OK)
        std::cout<<"OK"<<std::endl;
    else
        std::cout<<"ERROR"<<std::endl;
}
/*void Register5()
{
    cpr::Url url{"http://47.111.82.206:8088/VIID/Persons"};
    cpr::Response response = cpr::Post(url, 
                             cpr::Body{{"x", "5"}},
                             cpr::Header{{"Content-Type","application/json;charset=utf-8"}});
    std::cout<<response.text<<std::endl;
    std::cout<<response.url<<std::endl;
    std::cout<<response.header["content-type"]<<std::endl;
    std::cout<<response.status_code<<std::endl;
    if(response.error.code==cpr::ErrorCode::OK)
        std::cout<<"OK"<<std::endl;
    else
        std::cout<<"ERROR"<<std::endl;
}*/
void Register6(cpr::Session &session)
{
    auto j = R"({
        "KeepaliveObject":{
            "DeviceID":"31000000001310910561"
        }
    })"_json;
    std::string j_string = j.dump();
    cpr::Url url{"http://47.111.82.206:8088/VIID/MotorVehicles"};
    session.SetUrl(url);
    session.SetOption(cpr::Header{{"Content-Type","application/json;charset=utf-8"}});
    session.SetOption(cpr::Body{j_string});
    cpr::Response response = session.Post();
    std::cout<<response.text<<std::endl;
    std::cout<<response.url<<std::endl;
    std::cout<<response.header["content-type"]<<std::endl;
    std::cout<<response.status_code<<std::endl;
    if(response.error.code==cpr::ErrorCode::OK)
        std::cout<<"OK"<<std::endl;
    else
        std::cout<<"ERROR"<<std::endl;
}
void Register7(cpr::Session &session)
{
    cpr::Url url{"http://47.111.82.206:8088/VIID/ImageServer"};
    session.SetUrl(url);
    /*cpr::Response response = session.Get(url, 
                             cpr::Parameters{{"x", "5"}},
                             cpr::Header{{"Content-Type","application/json;charset=utf-8"}});*/
    session.SetOption(cpr::Header{{"Content-Type","application/json;charset=utf-8"}});
    session.SetOption(cpr::Parameters{{"x", "5"}});
    cpr::Response response = session.Get();
    std::cout<<response.text<<std::endl;
    std::cout<<response.url<<std::endl;
    std::cout<<response.header["content-type"]<<std::endl;
    std::cout<<response.status_code<<std::endl;
    if(response.error.code==cpr::ErrorCode::OK)
        std::cout<<"OK"<<std::endl;
    else
        std::cout<<"ERROR"<<std::endl;
}
void Register8(cpr::Session &session)
{
    auto j = R"({
        "RegisterObject":{
            "DeviceID":"31000000001310910561"
        },
        "ProtocolVersion":"2.0"
    })"_json;
    std::string j_string = j.dump();
    cpr::Url url{"http://47.111.82.206:8088/VIID/Images"};
    session.SetUrl(url);
    session.SetOption(cpr::Header{{"Content-Type","application/json;charset=utf-8"}});
    session.SetOption(cpr::Body{j_string});
    cpr::Response response = session.Post();
    std::cout<<response.text<<std::endl;
    std::cout<<response.url<<std::endl;
    std::cout<<response.header["content-type"]<<std::endl;
    std::cout<<response.status_code<<std::endl;
    if(response.error.code==cpr::ErrorCode::OK)
        std::cout<<"OK"<<std::endl;
    else
        std::cout<<"ERROR"<<std::endl;
}
