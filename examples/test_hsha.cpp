//
//  test_iocp.cpp
//  Luves
//
//  Created by leviathan on 16/6/28.
//  Copyright © 2016年 leviathan. All rights reserved.
//
 
#include "luves.h"

using namespace luves;

void GetInput(const TcpConnPtr & conn)
{
    std::cout<<conn->GetInputBuffer();
}

int main()
{
    EventLoop loop;
    Ip4Addr server_addr("127.0.0.1",8080);
    
    
    HshaServer server(&loop, server_addr,4);
    server.SetReadCb(GetInput);
    
    server.SetWriteCb([](const TcpConnPtr & conn)
                      {conn->Send("HTTP/1.1 200 OK\r\n"
                                  "Content-Type:text/html;charset=utf-8\r\n"
                                  "Content-Length:18\r\n"
                                  "\r\n"
                                  "Welcome to tinyweb");});
    
    server.RunServer();
    loop.loop();
}

//*/