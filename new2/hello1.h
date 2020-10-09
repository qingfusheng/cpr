#include<iostream>
#include"json.hpp"
#include"cpr/cpr.h"
using namespace cpr;
class GAT1400Server
{
public:
    GAT1400Server(std::string &url); //初始化服务器的地址IP:port
    ~GAT1400Server();
    bool Register();   //向服务器注册，返回对错
    bool Unregister(); //向服务器注销，返回对错
    bool Keepalive();  //向服务器保活，返回对错
private:
    const std::string url_;
};
GAT1400Server::GAT1400Server(){ 
};
GAT1400Server::GAT1400Server(std::string &url){
    url_ = url;
    cpr::Session session;
}
GAT1400Server::~GAT1400Server(){
    delete session;

};
GAT1400Server::Register(){
    auto j = R"(
        {
            "RegisterObject":{
                "DeviceID":"31000000001310910561"
            },
            "ProtocolVersion":"2.0"
        }
    )"_json;
    std::string j_string = j.dump();
    cpr::Url url{url_};
    session.SetUrl(url);
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
};
GAT1400Server::Unregister(){
    auto j = R"(
        {
            "RegisterObject":{
                "DeviceID":"31000000001310910561"
            },
            "ProtocolVersion":"2.0"
        }
    )"_json;
    std::string j_string = j.dump();
    cpr::Url url{url_};
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
};
GAT1400Server::Keepalive(){
    auto j = R"({
        "KeepaliveObject":{
            "DeviceID":"31000000001310910561"
        }
    })"_json;
    std::string j_string = j.dump();
    cpr::Url url{url_};
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
};
class GAT1400Face
{
};
class GAT1400Person
{
};
class GAT1400Vehicle
{
};
