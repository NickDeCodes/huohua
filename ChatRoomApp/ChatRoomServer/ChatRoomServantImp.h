#ifndef _ChatRoomServantImp_H_
#define _ChatRoomServantImp_H_

#include "servant/Application.h"
#include "ChatRoomServant.h"


const int CRP_MIN_SIZE = 8; // 报文最小size

/* 报文头 */
struct Header {
    int length = 0;
    int16_t type = 0; // 1 for keepalive, 2 for login init, 3 for data string
    int16_t flag = 0; // 1 for system, 2 for 群聊, 3 for 单聊
    char data[0]; // 存放Data结构
};

/* 数据包 */
struct Data {
    char form[0]; // 发送者
    char to[0]; // 发给谁
    char content[0]; // 内容
};

/* 用户节点 */
struct Node {
    int fd; // socketFd
    uint16_t port; // 连接端口
    string ip; // ip地址
    string name; // 名字
};

/**
 *
 *
 */
class ChatRoomServantImp : public ChatRoomApp::ChatRoomServant {
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
    virtual int test(tars::TarsCurrentPtr current) { return 0; };

    /**
     *
     */
    virtual int testHello(const std::string &sReq, std::string &sRsp, tars::TarsCurrentPtr current);

    /*****************************************************************************
     *
     * * 说明：处理请求包
     * * Author : zhengdongqi@huohua.cn
     * @param current：当前指针
     * @param reponse：响应 
     * **************************************************************************/ 
    virtual int doRequest(tars::TarsCurrentPtr current, vector<char>& response);

    /*****************************************************************************
     *
     * * 说明：清理用户节点
     * * Author : zhengdongqi@huohua.cn
     * @param 
     * **************************************************************************/ 
    virtual int cleanNode();

    /*****************************************************************************
     *
     * * 说明：群发给所有人
     * * Author : zhengdongqi@huohua.cn
     * @param hdr：数据包
     * **************************************************************************/ 
    virtual int sendAll(Header *hdr);

    /*****************************************************************************
     *
     * * 说明：单发给某人
     * * Author : zhengdongqi@huohua.cn
     * @param to：发送给指定的人
     * @param hdr：数据包 
     * **************************************************************************/ 
    virtual int sendTo(Node *to, Header *hdr);

};
/////////////////////////////////////////////////////
#endif
