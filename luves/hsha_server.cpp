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
        server_->SetHSHA(true);
        server_->RunServer();
        server_->SetReadCb(readcb_);
        server_->SetWriteCb(writecb_);
        
        ThreadsPool::SetThreadsNum(threadNum_);
        ThreadsPool::SetTcpConnectionFdMapPtr(server_->GetTcpConnMap());
        ThreadsPool::Instance().CreatePool();
    }
    
}