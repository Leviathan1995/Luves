//
//  hsha.cpp
//  Luves
//
//  Created by leviathan on 16/6/26.
//  Copyright © 2016年 leviathan. All rights reserved.
//

#include "hshaserver.h"

namespace luves
{

    void HSHAServer::RunServer()
    {
        server_->SetHSHA(true);
        server_->RunServer();
        server_->SetReadCb(readcb_);
        server_->SetWriteCb(writecb_);
        
        ThreadsPool::SetThreadsNum(threadNum_);
        ThreadsPool::SetTcpConnectionFdPtr(server_->GetTcpConnMap());
        ThreadsPool::Instance().CreatePool();
    }
    
}
