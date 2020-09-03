#include <gtest/gtest.h>

#include <array>
#include <cstdio>
#include <fstream>
#include <string>

#include <cpr/cpr.h>
#include <cpr/multipart.h>

using namespace cpr;

static HttpServer* server = new HttpServer();
TEST(){
	Url url{"http://47.111.82.206:8088/VIID/System/Register"};
	Session session;
    session.SetUrl(url);
    Response response = Post(url, 
                             Body{"{"RegisterObject":{\"DeviceID\":\"31000000001310910561\",\"ProtocolVersion\":\"2.0\"}}"},
                             Header{{"Content-Type","application/json;charset=utf-8"}});
    EXPECT_EQ(expected_text,response.text);
	EXPORT_EQ(url,response.url);
	EXPECT_EQ(std::string{"application/json"},response.header["content-type"]);
	EXPECT_EQ(201,response.status_code);
	EXPECT_EQ(ErrorCode::OK, response.error.code);
} 
TEST(){
	Url url{"http://47.111.82.206:8088/VIID/System/UnRegister"};
	Session session;
    session.SetUrl(url);
    Response response = Post(url, 
                             Payload{{"x", "5"}},
                             Header{{"Content-Type","application/json;charset=utf-8"}});
    EXPECT_EQ(expected_text,response.text);
	EXPORT_EQ(url,response.url);
	EXPECT_EQ(std::string{"application/json"},response.header["content-type"]);
	EXPECT_EQ(201,response.status_code);
	EXPECT_EQ(ErrorCode::OK, response.error.code);
} 
TEST(){
	Url url{"http://47.111.82.206:8088/VIID/System/Keepalive"};
	Session session;
    session.SetUrl(url);
    Response response = Post(url, 
                             Body{"{\"KeepaliveObject\":\{\"DeviceID\":\"31000000001310910561\"}}"},
                             Header{{"Content-Type","application/json;charset=utf-8"}});
    EXPECT_EQ(expected_text,response.text);
	EXPORT_EQ(url,response.url);
	EXPECT_EQ(std::string{"application/json"},response.header["content-type"]);
	EXPECT_EQ(201,response.status_code);
	EXPECT_EQ(ErrorCode::OK, response.error.code);
} 
TEST(){
	Url url{"http://47.111.82.206:8088/VIID/Faces"};
	Session session;
    session.SetUrl(url);
    Response response = Post(url, 
                             Payload{{"x", "5"}},
                             Header{{"Content-Type","application/json;charset=utf-8"}});
    EXPECT_EQ(expected_text,response.text);
	EXPORT_EQ(url,response.url);
	EXPECT_EQ(std::string{"application/json"},response.header["content-type"]);
	EXPECT_EQ(201,response.status_code);
	EXPECT_EQ(ErrorCode::OK, response.error.code);
} 
TEST(){
	Url url{"http://47.111.82.206:8088/VIID/MotorVehicles"};
	Session session;
    session.SetUrl(url);
    Response response = Post(url, 
                             Payload{{"x", "5"}},
                             Header{{"Content-Type","application/json;charset=utf-8"}});
    EXPECT_EQ(expected_text,response.text);
	EXPORT_EQ(url,response.url);
	EXPECT_EQ(std::string{"application/json"},response.header["content-type"]);
	EXPECT_EQ(201,response.status_code);
	EXPECT_EQ(ErrorCode::OK, response.error.code);
} 
TEST(){
	Url url{"http://47.111.82.206:8088/VIID/ImageServer"};
	Session session;
    session.SetUrl(url);
    Response response = Get(url, 
                             Parameters{{"x", "5"}},
                             Header{{"Content-Type","application/json;charset=utf-8"}});
    EXPECT_EQ(expected_text,response.text);
	EXPORT_EQ(url,response.url);
	EXPECT_EQ(std::string{"application/json"},response.header["content-type"]);
	EXPECT_EQ(201,response.status_code);
	EXPECT_EQ(ErrorCode::OK, response.error.code);
} 
TEST(){
	Url url{"http://47.111.82.206:8088/VIID/Images"};
	Session session;
    session.SetUrl(url);
    Response response = Post(url, 
                             Body{"{\"RegisterObject\":{\"DeviceID\":\"31000000001310910561\",\"ProtocolVersion\":\"2.0\"}}"}
                             Header{{"Content-Type","application/json;charset=utf-8"}});
    EXPECT_EQ(expected_text,response.text);
	EXPORT_EQ(url,response.url);
	EXPECT_EQ(std::string{"application/json"},response.header["content-type"]);
	EXPECT_EQ(201,response.status_code);
	EXPECT_EQ(ErrorCode::OK, response.error.code);
} 

int main(int argc, char** argv) {
    return RUN_ALL_TESTS();
}

