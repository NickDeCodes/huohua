#ifndef _ChatRoomServantImp_H_
#define _ChatRoomServantImp_H_

#include "servant/Application.h"
#include "ChatRoomServant.h"


const int CRP_MIN_SIZE = 8;

struct Header {
    int length = 0;
    int16_t type = 0; // 1 for keepalive, 2 for login init, 3 for data string
    int16_t flag = 0; // 1 for system, 2 for 群聊, 3 for 单聊
    char data[0]; // 存放Data结构
};

struct Data {
    char form[0]; // 发送者
    char to[0]; // 发给谁
    char content[0]; // 内容
};

struct Node {
    int fd;
    uint16_t port;
    string ip;
    string name;
};

/**
 *
 *
 */
class ChatRoomServantImp : public ChatRoomApp::ChatRoomServant
{
public:
    /**
     *
     */
    virtual ~ChatRoomServantImp() {}

    /**
     *
     */
    virtual void initialize();

    /**
     *
     */
    virtual void destroy();

    /**
     *
     */
    virtual int test(tars::TarsCurrentPtr current) { return 0;};

    /**
     *
     */
    virtual int testHello(const std::string &sReq, std::string &sRsp, tars::TarsCurrentPtr current);

    virtual int doRequest(tars::TarsCurrentPtr current, vector<char>& response);

    virtual int cleanNode();

    virtual int sendAll(Header *hdr);

    virtual int sendTo(Node *to);

};
/////////////////////////////////////////////////////
#endif
