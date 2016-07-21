//
//  hsha.cpp
//  Luves
//
//  Created by leviathan on 16/6/26.
//  Copyright © 2016年 leviathan. All rights reserved.
//

#include "hsha_server.h"

namespace luves
{
    
    void HshaServer::RunServer()
    {
        this->server_->SetHsha(true);
        this->server_->RunServer();
        this->server_->SetReadCb(readcb_);
        this->server_->SetWriteCb(writecb_);
        
        ThreadsPool::SetThreadsNum(threads_nums_);
        ThreadsPool::SetTcpConnectionFdMapPtr(server_->GetTcpConnMap());
        ThreadsPool::Instance().CreatePool();
    }
    
}