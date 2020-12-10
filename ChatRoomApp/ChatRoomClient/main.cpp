/*************************************************************************
	> File Name: main.cpp
	> Author: zhengdongqi
	> Mail: zhengdongqi@huohua.cn
	> Created Time: 2020年11月23日 星期一 15时32分27秒
 ************************************************************************/

#include <iostream>
#include <fstream>
#include "servant/Communicator.h"
#include "ChatRoomServant.h"
#include "util/tc_thread_pool.h"

using namespace std;
using namespace ChatRoomApp;
using namespace tars;

const int MAX_SIZE = 1024;
const int MIN_SIZE = 32;
const int CRP_MIN_SIZE = 8;

struct Header {
    int32_t length = 0;
    int16_t type = 0; // 1 for keeplive, 2 login init, 3 for data string 
    int16_t flag = 0; // 1 for system, 2 for 群聊, 3 for 单聊
    char data[0]; // 存放Data结构
};

struct Data {
    char form[0]; // 发送者
    char to[0]; // 发给谁
    char content[0]; // 内容
};

int ChatRoomAppPort = 25555;
string ChatRoomAppIP = "172.16.2.188";
string myName;

TC_ThreadPool tcPool;
TC_Socket tcMessage;

Header *setHeader(int type, int flag, const char *data) {
    char *tmp = new char[MAX_SIZE];
    Header *hdr = (Header *)tmp;

    hdr->type = type;
    hdr->flag = flag;
    memcpy(gdr->data, data, strlen(data));
    hdr->length = CRP_MIN_SIZE + strlen(data);
    
    return hdr;
}

void heartBeat() {
    while (true) {
        Header *hdr = setHeader(1, 1, "ping");

        tcMessage.send((char *)hdr, hdr->length, 0);
        memset(hdr, 0, hdr->length);
        sleep(60);
        tcMessage.recv((char *)hdr, MAX_SIZE, 0);
        
        if (hdr->length < CRP_MIN_SIZE) {
            return ;
        } 
        
        switch (hdr->type) {
        case 1: 
            cout << "heartBeat--CRP length: " << hdr->length << " type: " << hdr->type << " flag: " << hdr->flag <<" data: " << hdr->data << endl; 
            break;
        default: 
            cout << "hdr->type error" << hdr->type << endl;
            break;
        }
        
    }
    return ;
}

int main(int argc, char **argv) {
    if (argc != 2) {
        cout << "usage: " << argv[0] << " Name" << endl;
        return -1;  
    }
    myName = argv[1];

    try {
        tcPool.init(5);
        tcPool.start();
        tcMessage.createSocket();
        tcMessage.connect(ChatRoomAppIP, ChatRoomAppPort);
        tcPool.exec(bind(&heartBeat));
    } catch(exception &e) {
        cerr << "exception:" << e.what() << endl;
    } catch (...) {
        cerr << "unknown exception." << endl;
    }

    return 0;

}
