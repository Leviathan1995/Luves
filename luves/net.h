//
//  net.h
//  Luves
//
//  Created by leviathan on 16/5/17.
//  Copyright © 2016年 leviathan. All rights reserved.
//

#ifndef NET_H_
#define NET_H_

#include <iostream>
#include <cstring>
#include <unistd.h>
#include <string>
#include <netinet/in.h>
#include <sys/socket.h>
#include <fcntl.h>

#include "logger.h"

namespace luves {
    
    //Socket operation
    class SocketOp
    {
    public:
        static int SetNonblock(int fd);
        static int SetReuseaddr(int fd);
        static int SetReuseport(int fd);
    };
    
    //
    //Ipv4
    //
    class Ip4Addr
    {
    public:
        Ip4Addr(const std::string & ip,int port):
            ip_(ip),port_(port){};
        Ip4Addr(const sockaddr_in & addr):addr_(addr){};
        
        //get data
        short GetPort(){return port_;}
        std::string GetIp(){return ip_;}
        
        std::string ToString();
    private:
        sockaddr_in addr_;
        std::string ip_;
        short port_;
    };
    
    //
    //Socket
    //
    class Socket
    {
    public:
        explicit Socket(int fd):fd_(Socket::CreateNonBlockSocket()){}
        ~Socket(){};
        
        int GetFd(){return fd_;}
        
        //Socket Operation
        static void ShutDownWrite();
        static int Bind(int fd,sockaddr_in * server_addr);
        static void Listen(int fd);
        static int Accept(int fd,sockaddr_in * client_addr);
        static void Connect(int fd,sockaddr_in * server_addr);
        static int CreateNonBlockSocket();
        static void Close(int fd);
    private:
        const int fd_;
    };
    
}

#endif /* net_h */
