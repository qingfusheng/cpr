#include <unistd.h>
#include <iostream>
#include <array>
#include <cstdio>
#include <fstream>
#include <string>
#include "include/cpr/cpr.h"
void Register1();
void Register2();
void Register3();
void Register4();
/*void Register5();*/
void Register6();
void Register7();
void Register8();
int main(int argc, char** argv) {
	while(1){
		Register1();sleep(1);
		Register2();sleep(1);
		Register3();sleep(1);
		Register4();sleep(1);
		/*Register5();sleep(1);*/
		Register6();sleep(1);
		Register7();sleep(1);
		Register8();sleep(1);
	}
}
void Register1()
{
    cpr::Session session;
    cpr::Url url{"http://47.111.82.206:8088/VIID/System/Register"};
    session.SetUrl(url);
    cpr::Response response = session.Post(url, 
                             cpr::Body{"{"RegisterObject":{\"DeviceID\":\"31000000001310910561\",\"ProtocolVersion\":\"2.0\"}}"},
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
void Register2()
{
    cpr::Session session;
    cpr::Url url{"http://47.111.82.206:8088/VIID/System/UnRegister"};
    session.SetUrl(url);
    cpr::Response response = session.Post(url, 
                             cpr::Body{"{"RegisterObject":{\"DeviceID\":\"31000000001310910561\",\"ProtocolVersion\":\"2.0\"}}"},
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
void Register3()
{
    cpr::Session session;
    cpr::Url url{"http://47.111.82.206:8088/VIID/System/Keepalive"};
    session.SetUrl(url);
    cpr::Response response = session.Post(url, 
                            /* cpr::Payload{{"x", "5"}},*/
                             cpr::Body{"{\"KeepaliveObject\":\{\"DeviceID\":\"31000000001310910561\"}}"},
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
void Register4()
{
    cpr::Session session;
    cpr::Url url{"http://47.111.82.206:8088/VIID/Faces"};
    session.SetUrl(url);
    cpr::Response response = session.Post(url, 
                             cpr::Body{"{\"KeepaliveObject\":\{\"DeviceID\":\"31000000001310910561\"}}"},
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
void Register6()
{
    cpr::Session session;
    cpr::Url url{"http://47.111.82.206:8088/VIID/MotorVehicles"};
    session.SetUrl(url);
    cpr::Response response = session.Post(url, 
                             cpr::Body{"{\"KeepaliveObject\":\{\"DeviceID\":\"31000000001310910561\"}}"},
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
void Register7()
{
    cpr::Session session;
    cpr::Url url{"http://47.111.82.206:8088/VIID/ImageServer"};
    session.SetUrl(url);
    cpr::Response response = session.Get(url, 
                             cpr::Parameters{{"x", "5"}},
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
void Register8()
{
    cpr::Session session;
    cpr::Url url{"http://47.111.82.206:8088/VIID/Images"};
    session.SetUrl(url);
    cpr::Response response = session.Post(url,
                             cpr::Body{"{\"RegisterObject\":{\"DeviceID\":\"31000000001310910561\",\"ProtocolVersion\":\"2.0\"}}"}
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
