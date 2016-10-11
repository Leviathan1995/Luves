//
//  tcp_connection.cpp
//  Luves
//
//  Created by leviathan on 16/6/21.
//  Copyright © 2016年 leviathan. All rights reserved.
//

#include "connection.h"

namespace luves {
    
    
    //
    //TCP连接模块
    //

    //写入消息至缓冲区
    void TcpConnection::Send(Buffer & msg)
    {
        output_.Append(msg);
        output_.WriteImp(fd_);
    }
    
    void TcpConnection::Send(const std::string & msg)
    {
        output_.Append(msg);
        output_.WriteImp(fd_);
    }
    
    //接收消息
    void TcpConnection::HandleRead()
    {

        while (1)
        {
            int n=input_.ReadImp(fd_);
            if (n>0)
            {
                if (readcb_)
                    readcb_(shared_from_this());
            }
            else
            {
                close(fd_);
                INFO_LOG("Close %d socket.", fd_);
                is_close_ = true;
                break;
            }
        }
    }
    
    //发送消息
    void TcpConnection::HandleWrite()
    {
        if (writecb_)
            writecb_(shared_from_this());
    }
    
    void TcpConnection::HandleClose(const TcpConnectionPtr & conn)
    {
        if (closecb_)
            closecb_(shared_from_this());
    }
}
