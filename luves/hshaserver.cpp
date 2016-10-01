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

    void HshaServer::RunServer()
    {
        server_->RunServer();
        server_->SetReadCb(readcb_);
        server_->SetWriteCb(writecb_);
        
        ThreadsPool::SetThreadNum(thread_num_);
        ThreadsPool::SetTcpConnectionFdPtr(server_->GetTcpConnMap());
        ThreadsPool::Instance().CreatePool();
    }
    
}
