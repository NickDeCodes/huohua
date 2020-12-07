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

int ChatRoomServantImp::cleanNode(tars::TarsCurrentPtr current) {
    
    return 0;
}

int ChatRoomServantImp::sendAll(tars::TarsCurrentPtr current) {
    auto iter = nodeMap.begin();
    while (iter != nodeMap.end()) {
        TC_Socket tcAll;
        //tcAll.init(iter->secend->fd, true);
    }
    return 0;
}

int ChatRoomServantImp::sendTo(tars::TarsCurrentPtr current, Node *to) {

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
        TLOGDEBUG("--CRP: length:" << hdr->length << " type: " << hdr->type << " flag: " << hdr->flag << " data: " << hdr->data << " uid: "<< current->getUId() << endl);
        memcpy(hdr->data, "pang", strlen("pang"));
        current->sendResponse(pack, hdr->length);       
    } else if (hdr->type == 2) {
        Node user;
        user.fd = current->getFd();
        user.port = current->getPort();
        user.ip = current->getIp();
        user.name = string(hdr->data);
        string nameMap = to_string(user.fd) + "-" + to_string(user.port) + "-" + user.ip + "+" + user.name;
        auto iter = nodeMap.find(nameMap);
        if (iter != nodeMap.end()) {
            TLOGDEBUG(nameMap << "-用户已存在" << endl);        
        } else {
            nodeMap.insert({nameMap, user});
            TLOGDEBUG(nameMap << "-login! " << endl);        
        }
    } else if (hdr->type == 3) {
        if (hdr->flag == 1) {
            TLOGDEBUG("system inform: " << hdr->data << endl);
        } else if (hdr->flag == 2) {
            //sendAll();
        } else if (hdr->flag == 3) {
        }
    }
  
    return 0;
}
