//
//  channel.cpp
//  Luves
//
//  Created by hashdata on 16/9/30.
//  Copyright © 2016年 hashdata. All rights reserved.
//

#include "channel.h"

//
//事件通道
//
namespace luves {

    Channel::Channel(EventLoop * loop,int fd, bool is_listen)
    {
        loop_ = loop;
        fd_ = fd;
        is_listen_ = is_listen;

        if(!is_listen)
        {
            sockaddr_in local,peer;
            socklen_t len=sizeof(local);
            int ret=getsockname(fd_, (struct sockaddr*)&local, &len);
            if (ret<0)
                ERROR_LOG("get local addr failed! %d %s", errno, strerror(errno));
            ret=getpeername(fd_, (struct sockaddr*)&peer, &len);
            if (ret<0)
                ERROR_LOG("get peer addr failed! %d %s", errno, strerror(errno));
        
            Ip4Addr local_addr(local), peer_addr(peer);
            connection_ = TcpConnectionPtr(new TcpConnection(loop_, fd_));
            SocketOp::SetNonblock(fd_); //设置fd为非阻塞
            TRACE_LOG("TCP服务创建成功:%s--%s:%d",local_addr.ToString().c_str(), peer_addr.ToString().c_str(),fd_);
            
            SetReadCb([this]{GetConnectionPtr()->HandleRead();});
            SetWriteCb([this]{GetConnectionPtr()->HandleWrite();});
            
        }
    };

    //关闭通道
    void Channel::Close()
    {
        //清理缓冲区
        this->connection_->GetInputBuffer().Clear();
        this->connection_->GetOutputBuffer().Clear();
        loop_->DeleteChannel(this);
    }

    void Channel::HandleEvent()
    {
        //listen管道
        if (is_listen_)
        {
            if (read_cb_)
            {
                read_cb_();
                return;
            }
        }
        
        //非listen管道
        if(!is_listen_)
        {
            if (read_cb_)
                    read_cb_();
            if (write_cb_)
                    write_cb_();
        }
        if(connection_->IsClose())
            Close();
        else
        {
            INFO_LOG("Try to re-add %d to kqueue.", this->GetFd());
            loop_->AddChannel(this);
        }
    }
}
