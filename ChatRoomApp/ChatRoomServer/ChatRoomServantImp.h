#ifndef _ChatRoomServantImp_H_
#define _ChatRoomServantImp_H_

#include "servant/Application.h"
#include "ChatRoomServant.h"


const int CRP_MIN_SIZE = 8;

struct Header{
    int length = 0;
    int16_t type = 0; // 1 for keepalive, 2 for data string
    int16_t flag = 0; // 1 for system, 2 for 群聊, 3 for 单聊
    char data[0];
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

    virtual int ChatRoomServantImp::cleanNode(tars::TarsCurrentPtr current);

    virtual int ChatRoomServantImp::sendAll(tars::TarsCurrentPtr current);

    virtual int ChatRoomServantImp::sendTo(tars::TarsCurrentPtr current, Node *to);

};
/////////////////////////////////////////////////////
#endif
