#ifndef _ChatRoomServer_H_
#define _ChatRoomServer_H_

#include <iostream>
#include "servant/Application.h"

using namespace tars;

/**
 *
 **/
class ChatRoomServer : public Application
{
public:
    /**
     *
     **/
    virtual ~ChatRoomServer() {};

    /**
     *
     **/
    virtual void initialize();

    /**
     *
     **/
    virtual void destroyApp();
};

extern ChatRoomServer g_app;

////////////////////////////////////////////
#endif
