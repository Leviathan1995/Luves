//
//  LFServer.h
//  Luves
//
//  Created by leviathan on 16/7/24.
//  Copyright © 2016年 leviathan. All rights reserved.
//

#ifndef LF_SERVER_H_
#define LF_SERVER_H_

#include "tcpServer.h"
#include "threads.h"

namespace luves {
    
    typedef std::shared_ptr<TcpServer> TcpServerPtr;
    typedef std::function<void (const TcpConnectionPtr &)> HSHACallBack;

    class LFServer
    {
    public:
        LFServer(EventLoop *loop,Ip4Addr &addr):server_(new TcpServer(loop,addr)){}
        void RunServer();
    private:
        int threadNum_;
        TcpServerPtr server_;
        HSHACallBack readcb_,writecb_;
        
    };
}
#endif /* LF_serve_h */
