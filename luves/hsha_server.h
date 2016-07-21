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
    //半同步半异步模式
    //
    typedef std::shared_ptr<TcpConnection> TcpConnectionPtr;
    typedef std::shared_ptr<TcpServer> TcpServerPtr;
    typedef std::function<void (const TcpConnectionPtr &)> HshaCallBack;
    
    //class HshaServer;
    
    //typedef std::shared_ptr<HshaServer> HshaServerPtr;
    
    class HshaServer
    {
    public:
        HshaServer(EventLoop *loop,Ip4Addr &addr,int threads_nums):server_(TcpServerPtr(new TcpServer(loop,addr))),threads_nums_(threads_nums){}
        
        void RunServer();
        
        //设置回调函数
        void SetReadCb(const HshaCallBack & cb){readcb_=cb;};
        void SetWriteCb(const HshaCallBack & cb){writecb_=cb;};
        
        
    private:
        int threads_nums_;              //线程池中线程数量
        TcpServerPtr server_;
        HshaCallBack readcb_,writecb_;
        
    };
}
#endif /* hsha_h */
