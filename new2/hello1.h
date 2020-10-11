#include<iostream>
#include"json.hpp"
#include"cpr/cpr.h"
using namespace cpr;
using nlohmann::json;
class GAT1400Server
{
public:
    GAT1400Server();
    GAT1400Server(std::string &url); //初始化服务器的地址IP:port
    ~GAT1400Server();
    bool Register(std::string deviceid);   //向服务器注册，返回对错
    bool Unregister(std::string deviceid); //向服务器注销，返回对错
    bool Keepalive(std::string deviceid);  //向服务器保活，返回对错
private:
    cpr::Session *session = new Session();//使用session对象包装以达到网络连接复用的目的
};
GAT1400Server::GAT1400Server(){ 
};
GAT1400Server::GAT1400Server(std::string &url_){
    cpr::Url url{url_};
    session->SetUrl(url);
    session->SetOption(cpr::Header{{"Content-Type","application/json;charset=utf-8"}});
}
GAT1400Server::~GAT1400Server(){ //析构
    delete session;

};
bool GAT1400Server::Register(std::string deviceid){
    json j; //定义一个json串
	j["RegisterObject"]={"DeviceID",deviceid};
	j["ProtocolVersion"]="2.0";
    std::string j_string = j.dump();//将json串转换为字符串
    session->SetOption(cpr::Body{j_string});
    cpr::Response response = session->Post();
    std::cout<<response.text<<std::endl;    //expected_text
    std::cout<<response.url<<std::endl;     //url
    std::cout<<response.header["content-type"]<<std::endl;  //std::string{"application/json"}
    std::cout<<response.status_code<<std::endl; //status_code
    if(response.error.code==cpr::ErrorCode::OK)
        std::cout<<"OK"<<std::endl;
    else
        std::cout<<"ERROR"<<std::endl;
};
bool GAT1400Server::Unregister(std::string deviceid){
    json j;
	j["RegisterObject"]={"DeviceID",deviceid};
	j["ProtocolVersion"]="2.0";
    std::string j_string = j.dump();
    session->SetOption(cpr::Body{j_string});
    cpr::Response response = session->Post();
    std::cout<<response.text<<std::endl;
    std::cout<<response.url<<std::endl;
    std::cout<<response.header["content-type"]<<std::endl;
    std::cout<<response.status_code<<std::endl;
    if(response.error.code==cpr::ErrorCode::OK)
        std::cout<<"OK"<<std::endl;
    else
        std::cout<<"ERROR"<<std::endl;
};
bool GAT1400Server::Keepalive(std::string deviceid){
    json j;
	j["RegisterObject"]={"DeviceID",deviceid};
	j["ProtocolVersion"]="2.0";
    std::string j_string = j.dump();
    session->SetOption(cpr::Body{j_string});
    cpr::Response response = session->Post();
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
