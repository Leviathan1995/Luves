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

#include "tcp_server.h"
#include "threads.h"
#include "event_handle.h"
#include "net.h"

namespace luves
{
    
    //
    //HSHA model
    //
    typedef std::shared_ptr<TcpServer> TcpServerPtr;
    typedef std::function<void (const TcpConnectionPtr &)> HSHACallBack;
    
    //HSHAServer
    class HshaServer
    {
    public:
        HshaServer(EventLoop *loop,Ip4Addr &addr,int threads_nums):server_(TcpServerPtr(new TcpServer(loop,addr))),threadNum_(threads_nums){}
        
        void RunServer();
        
        //set call back function
        void SetReadCb(const HSHACallBack & cb){readcb_=cb;};
        void SetWriteCb(const HSHACallBack & cb){writecb_=cb;};
        
        
    private:
        int threadNum_;              //线程池中线程数量
        TcpServerPtr server_;
        HSHACallBack readcb_,writecb_;
        
    };
}
#endif /* hsha_h */
