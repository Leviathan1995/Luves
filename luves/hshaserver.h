//
//  hsha.h
//  Luves
//
//  Created by leviathan on 16/6/26.
//  Copyright © 2016年 leviathan. All rights reserved.
//

#ifndef HSHA_SERVER_H_
#define HSHA_SERVER_H_

#include <functional>

#include "tcpserver.h"
#include "threadpool.h"
#include "eventhandle.h"
#include "net.h"

namespace luves
{
    
    //
    //Hsha model
    //
    typedef std::shared_ptr<TcpServer> TcpServerPtr;
    typedef std::function<void (const TcpConnectionPtr &)> HshaCallBack;
    
    //HshaServer
    class HshaServer
    {
    public:
        HshaServer(EventLoop *loop,Ip4Addr &addr,int thread_num):server_(TcpServerPtr(new TcpServer(loop,addr))),thread_num_(thread_num){}
        
        void RunServer();
        
        //set call back function
        void SetReadCb(const HshaCallBack & cb){readcb_=cb;};
        void SetWriteCb(const HshaCallBack & cb){writecb_=cb;};

    private:
        int thread_num_;
        TcpServerPtr server_;
        HshaCallBack readcb_,writecb_;
        
    };
}
#endif /* hsha_h */
