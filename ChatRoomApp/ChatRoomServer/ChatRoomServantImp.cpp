#include "ChatRoomServantImp.h"
#include "servant/Application.h"
#include "util/tc_socket.h"

using namespace std;
int nodeNum = 0;
map<string, Node> nodeMap;

//////////////////////////////////////////////////////
void ChatRoomServantImp::initialize()
{
    //initialize servant here:
    //...
}

//////////////////////////////////////////////////////
void ChatRoomServantImp::destroy()
{
    //destroy servant here:
    //...
}

int ChatRoomServantImp::testHello(const std::string &sReq, std::string &sRsp, tars::TarsCurrentPtr current)
{
    TLOGDEBUG("testHellosReq:"<< sReq << endl);
    sRsp = sReq;
    return 0;

}

int ChatRoomServantImp::cleanNode() {
    
    return 0;
}

int ChatRoomServantImp::sendAll(Header *hdr) {
    auto iter = nodeMap.begin();
    TLOGDEBUG("sendall" << endl);        
    while (iter != nodeMap.end()) {
        TLOGDEBUG("循环" << endl);        
        TC_Socket tcAll;
        tcAll.init(iter->second.fd, true);
        //tcAll.send((char *)hdr, hdr->length, 0);
        ++iter;
    }
    return 0;
}

int ChatRoomServantImp::sendTo(Node *to, Header *hdr) {
    TC_Socket tcTo;
    tcTo.init(to->fd, true);
    tcTo.send((char *)hdr, hdr->length, 0); 
    return 0;
}

int ChatRoomServantImp::doRequest(tars::TarsCurrentPtr current, vector<char>& response) {
    const vector<char>& request = current->getRequestBuffer();
    if (request.empty()) {
        TLOGDEBUG("request is empty!!!: " << current->getUId() << endl);
        return 0;    
    }
        
    const char *pack = &request[0];
    Header *hdr = (Header *)pack;

    if (hdr->type == 1) {
        TLOGDEBUG("--CRP: length: " << hdr->length << " type: " << hdr->type << " flag: " << hdr->flag << " data: " << hdr->data << " uid: "<< current->getUId() << endl);
        memcpy(hdr->data, "pang", strlen("pang"));
        current->sendResponse(pack, hdr->length);       
    } else if (hdr->type == 2) {
        Node user;
        user.fd = current->getFd();
        user.port = current->getPort();
        user.ip = current->getIp();
        user.name = string(hdr->data);
        auto iter = nodeMap.find(user.name);
        if (iter != nodeMap.end()) {
            TLOGDEBUG(user.name << "-用户已存在" << endl);        
            return 0;
        } else {
            nodeMap.insert({user.name, user});
            TLOGDEBUG(user.name << "-login! " << endl);        
        }
    } else if (hdr->type == 3) {
        if (hdr->flag == 1) {
            TLOGDEBUG("system inform: " << hdr->data << endl);
        } else if (hdr->flag == 2) {
            sendAll(hdr);
        } else if (hdr->flag == 3) {
            //sendTo(hdr);
        }
    }
    TLOGDEBUG("来啊" << endl);        
  
    return 0;
}
