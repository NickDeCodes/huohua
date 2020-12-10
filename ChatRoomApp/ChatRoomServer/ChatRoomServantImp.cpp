#include "ChatRoomServantImp.h"
#include "servant/Application.h"
#include "util/tc_socket.h"

using namespace std;
int nodeNum = 0;
map<string, Node> nodeMap;

//////////////////////////////////////////////////////
void ChatRoomServantImp::initialize() {
    //initialize servant here:
    //...
}

//////////////////////////////////////////////////////
void ChatRoomServantImp::destroy() {
    //destroy servant here:
    //...
}

int ChatRoomServantImp::testHello(const std::string &sReq, std::string &sRsp, tars::TarsCurrentPtr current) {
    TLOGDEBUG("testHellosReq:"<< sReq << endl);
    sRsp = sReq;
    return 0;

}

int ChatRoomServantImp::cleanNode() {

    return 0;
}

int ChatRoomServantImp::sendAll(Header *hdr) {
    TLOGDEBUG("sendall" << endl);        
    for (auto iter = nodeMap.begin(); iter != nodeMap.end(); ++iter) {
        TLOGDEBUG("循环" << endl);        
        TC_Socket tcAll;
        tcAll.init(iter->second.fd, true);
        tcAll.send((char *)hdr, hdr->length, 0);
    }
    return 0;
}

int ChatRoomServantImp::sendTo(Node *to, Header *hdr) {
    TC_Socket tcTo;
    tcTo.init(to->fd, true);
    tcTo.send((char *)hdr, hdr->length, 0); 
    return 0;
}

int ChatRoomServantImp::doRequest(tars::TarsCurrentPtr current, vector<char> &response) {
    const vector<char>& request = current->getRequestBuffer();
    if (request.empty()) {
        TLOGDEBUG("request is empty!!!: " << current->getUId() << endl);
        return 0;    
    }

    const char *pack = &request[0];
    Header *hdr = (Header *)pack;

    switch (hdr->type) {
    case 1:
        TLOGDEBUG("--CRP: length: " << hdr->length << 
                " type: " << hdr->type << 
                " flag: " << hdr->flag << 
                " data: " << hdr->data << 
                " uid: "<< current->getUId() << endl);
        memcpy(hdr->data, "pang", strlen("pang"));
        current->sendResponse(pack, hdr->length); 
        break;
    case 2:
        break;
    case 3:
        break;
    default:
        TLOGDEBUG("hdr->type error!!!: " << hdr->type << endl);
        break;
    }

    return 0;
}
