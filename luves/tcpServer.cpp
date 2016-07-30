//
//  tcp.cpp
//  Luves
//
//  Created by leviathan on 16/5/17.
//  Copyright © 2016年 leviathan. All rights reserved.
//

#include "tcpServer.h"

namespace luves {
    
    
    //
    //TCP Server模块
    //用于新建TCP服务端
    //
    TcpServer::~TcpServer()
    {
        delete listenChannel_;
    }
    void  TcpServer::Bind()
    {
        bzero(&serverAddr_, sizeof(serverAddr_));
        serverAddr_.sin_family=AF_INET;
        serverAddr_.sin_addr.s_addr=inet_addr(addr_.GetIp().c_str());
        serverAddr_.sin_port=htons(addr_.GetPort());
        
        Socket::Bind(listenFd_,this->GetServerAddrPointer());
    }
    
    void TcpServer::Listen()
    {
        listenning_=true;
        Socket::Listen(listenFd_);
    }
    
    void TcpServer::RunServer()
    {
        this->Bind();
        this->Listen();
        listenChannel_->SetEvent(EVFILT_READ);
        listenChannel_->SetReadCb([this]{HandleAccept();});    
        listenChannel_->SetIsListen(true);
        loop_->AddChannel(listenChannel_);

    }
    
    //当有新的连接,epoll被触发时,回调HandleAccept()函数并建立新的连接
    void TcpServer::HandleAccept()
    {
        struct sockaddr_in client_addr;
        int accept_fd;
        while((accept_fd=Socket::Accept(listenChannel_->GetFd(), &client_addr))>=0)
        {
            sockaddr_in local,peer;
            socklen_t len=sizeof(local);
            int ret;
            ret=getsockname(accept_fd, (struct sockaddr*)&local, &len);
            if (ret<0)
            {
                ERROR_LOG("get local addr failed! %d %s",errno,strerror(errno));
                continue;
            }
            ret=getpeername(accept_fd, (sockaddr*)&peer, &len);
            if (ret<0)
            {
                ERROR_LOG("get peer addr failed! %d %s",errno,strerror(errno));
                continue;
            }
            Ip4Addr local_addr(local),peer_addr(peer);
            this->NewConnection(accept_fd,local_addr,peer_addr);
        }

    }
    
    void TcpServer::NewConnection(int accept_fd,Ip4Addr local,Ip4Addr peer)
    {
        TcpConnectionPtr connection=TcpConnectionPtr(new TcpConnection(loop_));
        if (readcb_)
            connection->SetReadCb(readcb_);
        if (writecb_)
            connection->SetWriteCb(writecb_);
        if (closecb_)
            connection->SetCloseCb(closecb_);
        
        connection->Register(loop_, accept_fd,local,peer);
        tcpConnectionFd_[accept_fd]=connection;    //添加Tcp连接与accept套接字至映射队列        
    }
    
    //
    //TCP Client模块
    //新建TCP客户端
    //
    void TcpClient::RunClient()
    {
        this->Bind();
        this->HandleConnect();
        client_channel_->SetEvent(EVFILT_READ);
        loop_->UpdateChannel(client_channel_);
        
        TcpConnectionPtr conn=TcpConnectionPtr(new TcpConnection(loop_));
        if (readcb_)
        {
            conn->SetReadCb(readcb_);
        }
        if (writecb_) {
            conn->SetWriteCb(writecb_);
        }
        
    }
    
    void TcpClient::Bind()
    {
        struct sockaddr_in client_addr;
        bzero(&client_addr, sizeof(client_addr));
        client_addr.sin_family=AF_INET;
        client_addr.sin_addr.s_addr=htons(INADDR_ANY);
        client_addr.sin_port=htons(0);
        
        bind(client_fd_, (struct sockaddr *)&client_addr, sizeof(client_addr));
    }
    
    void TcpClient::HandleConnect()
    {
        struct sockaddr_in server_addr;
        server_addr.sin_family=AF_INET;
        server_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
        server_addr.sin_port=htons(server_addr_.GetPort());
        
        Socket::Bind(client_fd_, &server_addr);
        connect(client_fd_, (struct sockaddr *)&server_addr, sizeof(server_addr));
    }
    
}