#include "ChatRoomServer.h"
#include "ChatRoomServantImp.h"

using namespace std;

struct CRProtocolStream {
    static int parse(string &strInput, string &strOutput) {
        if (strInput.length() < CRP_MIN_SIZE) {
            return TC_EpollServer::PACKET_LESS;         
        }

        Header *pHDR = (Header*)strInput.c_str();
        if ((int32_t)strInput.length() < pHDR->length) {
            TLOGDEBUG("TC_EpollServer::PACKET_LESS" << endl);
            return TC_EpollServer::PACKET_LESS;         
        }

        strOutput = strInput.substr(0, pHDR->length);
        TLOGDEBUG("length: " << pHDR->length << " type: "<< pHDR->type << "flag: " << pHDR->flag << endl);

        strInput = strInput.substr(pHDR->length);
            
        return TC_EpollServer::PACKET_FULL;    
    }
};

ChatRoomServer g_app;

/////////////////////////////////////////////////////////////////
void
ChatRoomServer::initialize()
{
    //initialize application here:
    //...

    addServant<ChatRoomServantImp>(ServerConfig::Application + "." + ServerConfig::ServerName + ".ChatRoomServantObj");
    addServantProtocol(ServerConfig::Application + "." + ServerConfig::ServerName + ".ChatRoomServantObj", &CRProtocolStream::parse);
}
/////////////////////////////////////////////////////////////////
void
ChatRoomServer::destroyApp()
{
    //destroy application here:
    //...
}
/////////////////////////////////////////////////////////////////
int main(int argc, char* argv[]) {
    try {
        g_app.main(argc, argv);
        g_app.waitForShutdown();
    } catch (std::exception& e) {
        cerr << "std::exception:" << e.what() << std::endl;
    } catch (...) {
        cerr << "unknown exception." << std::endl;
    }
    return -1;
}
/////////////////////////////////////////////////////////////////
