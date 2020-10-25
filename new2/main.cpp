#include<iostream>
#include"hello1.h"
using namespace std;
int main(){
    std::string url = "http://47.111.82.206:8088/VIID/System/Register";
    std::string DeviceID = "31000000001310910561";
    GAT1400Server *gs = new GAT1400Server(url);
    gs->Register(DeviceID);
    gs->Unregister(DeviceID);
    gs->Keepalive(DeviceID);
}