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
        server_->SetReadCb(read_cb_);
        server_->SetWriteCb(write_cb_);
        server_->SetHsha(true);
        
        server_->RunServer();

        ThreadsPool::SetThreadNum(thread_num_);
        ThreadsPool::SetChannelPtr(server_->GetChannelPtr());
        ThreadsPool::Instance().CreatePool();
    }
    
}
