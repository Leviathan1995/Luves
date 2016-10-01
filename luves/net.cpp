//
//  net.cpp
//  Luves
//
//  Created by leviathan on 16/5/17.
//  Copyright © 2016年 leviathan. All rights reserved.
//
#include <stdio.h>

#include "net.h"

namespace luves
{
    //
    //Socket属性操作
    //
    int SocketOp::SetNonblock(int fd)
    {
        int flags=fcntl(fd, F_GETFL,0);
        if(flags<0)
            return errno;
        return fcntl(fd, F_SETFL,flags|O_NONBLOCK);
    }

    int SocketOp::SetReuseaddr(int fd)
    {
        int flags=1;
        int len=sizeof(flags);
        return setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &flags, len);
    }

    int SocketOp::SetReuseport(int fd)
    {
        int flags=1;
        int len=sizeof(flags);
        return setsockopt(fd, SOL_SOCKET, SO_REUSEPORT, &flags, len);
    }

    //
    //Ip4地址模块
    //
    std::string Ip4Addr::ToString()
    {
        uint32_t host = addr_.sin_addr.s_addr;
        char str[64];
        sprintf(str, "%d.%d.%d.%d:%d",(host>> 0)&0xff,(host >> 8)&0xff,(host >> 16)&0xff,(host>> 24)&0xff, ntohs(addr_.sin_port));
        return str;
    }

    //
    //Socket
    //
    Socket::~Socket()
    {
        Socket::Close(fd_);
    }

    void Socket::Close(int fd)
    {
        close(fd);
    }

    int Socket::CreateNonBlockSocket()
    {
        int socket_fd=socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        FATALIF_LOG(!socket_fd,"socket fd create failed!");
        return socket_fd;
    }

    int Socket::Bind(int fd,sockaddr_in * server_addr)
    {
        int ret =bind(fd,(struct sockaddr *)server_addr,sizeof(*server_addr));
        if (ret==-1)
        {
            close(fd);
            ERROR_LOG("bind to %d failed! %d %s",fd,errno,strerror(errno));
            return ret;
        }
        return 0;
    }

    void Socket::Listen(int fd)
    {
        int ret=listen(fd, 20);
        if (ret==-1)
        {
            ERROR_LOG("listen to %d failed! %d %s",fd,errno,strerror(errno));
        }
    }

    int Socket::Accept(int fd,sockaddr_in * client_addr)
    {
        socklen_t client_len=sizeof(*client_addr);
        int accept_fd=accept(fd, (struct sockaddr *)client_addr,&client_len);
        if(accept_fd==-1 & errno!=EAGAIN)
            ERROR_LOG("accept to %d failed! %d %s",fd,errno,strerror(errno));
        return accept_fd;
    }

    void Socket::Connect(int fd,sockaddr_in * server_addr)
    {
        connect(fd, (struct sockaddr *)&server_addr, sizeof(server_addr));

    }

}
