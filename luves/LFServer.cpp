//
//  LFServer.cpp
//  Luves
//
//  Created by leviathan on 16/7/24.
//  Copyright © 2016年 leviathan. All rights reserved.
//

#include "LFServer.h"
namespace luves
{
    void LFServer::RunServer()
    {
        server_->SetLF(true);
        server_->RunServer();
        server_->SetReadCb(readcb_);
        server_->SetWriteCb(writecb_);
        
        ThreadsPool::SetThreadsNum(threadNum_);
        ThreadsPool::SetTcpConnectionFdPtr(server_->GetTcpConnMap());
        ThreadsPool::Instance().CreatePool();
        
    }
}