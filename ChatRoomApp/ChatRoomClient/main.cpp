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

void heartBeat() {
    char *tmp = new char[MAX_SIZE];
    Header *hdr = (Header *)tmp;
    while (true) {
        hdr->type = 1;
        hdr->flag = 1;
        memcpy(hdr->data, "ping", strlen("ping")); 
        hdr->length = CRP_MIN_SIZE + strlen("ping");
        tcMessage.send((char *)hdr, hdr->length, 0);
        
        memset(hdr, 0, hdr->length);
        tcMessage.recv((char *)hdr, MAX_SIZE, 0);
        if (hdr->length < CRP_MIN_SIZE) {
            return ;
        } 
        
        if (hdr->type == 1) {
            cout << "--CRP length: " << hdr->length << " type: " << hdr->type << " flag: " << hdr->flag <<" data: " << hdr->data << endl; 
        } else {
            continue;
        }
        sleep(60);
    }
    return ;
}

void recvMessage() {
    char  *tmp = new char[MAX_SIZE];
    Header *hdr = (Header *)tmp;
    while (true) {
        tcMessage.recv((char *)hdr, MAX_SIZE, 0);
        std::ofstream ofile;
        ofile.open("./" + myName + ".log", std::ios::out | std::ios::app);
        if (hdr->length < CRP_MIN_SIZE) {
            return ;
        } else {
            if (hdr->type == 2) {
                if (hdr->flag == 1) {
                    return ;
                }
            } else if (hdr->type == 3) {
                if (hdr->flag == 1) {
                    ofile << "system message: " << hdr->data << endl;
                } else if (hdr->flag == 2) {
                    ofile << "group chat: " << hdr->data << endl;
                } else if (hdr->flag == 3) {
                    ofile << "single chat: " << hdr->data << endl;
                }
                cout.flush();
            } else {
                continue;
            }
        }
        ofile.close();
    }
    return ;
}

void sendMessage() {
    char *tmp = new char[MAX_SIZE];
    Header *hdr = (Header *)tmp;
    hdr->type = 2;
    hdr->flag = 1;
    memcpy(hdr->data, myName.c_str(), myName.length()) ;
    hdr->length = CRP_MIN_SIZE + strlen(hdr->data);
    tcMessage.send((char *)hdr, hdr->length, 0);
    memset(hdr->data, 0, sizeof(hdr->data));

    while (true) {
        cout << "请在以下的对话框输入信息，按回车发送\n" << endl;
        memset(hdr->data, 0, sizeof(hdr->data));
        cin >> hdr->data;
        hdr->type = 3;
        hdr->flag = 2;
        hdr->length = CRP_MIN_SIZE + strlen(hdr->data);
        tcMessage.send((char *)hdr, hdr->length, 0);
        system("clear");
    }
    return ;
}

int main(int argc,char ** argv) {
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
        try {
            tcPool.exec(bind(&heartBeat));
            tcPool.exec(bind(&recvMessage));
            tcPool.exec(bind(&sendMessage));
        } catch(exception &ex) {
            cerr << "ex:" << ex.what() << endl;    
        } catch(...) {
            cerr << "unknown exception." << endl;    
        }
        tcPool.waitForAllDone();
    } catch(exception& e) {
        cerr << "exception:" << e.what() << endl;
    } catch (...) {
        cerr << "unknown exception." << endl;
    }

    return 0;

}
