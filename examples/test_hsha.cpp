//
//  test_iocp.cpp
//  Luves
//
//  Created by leviathan on 16/6/28.
//  Copyright © 2016年 leviathan. All rights reserved.
//


#include "luves.h"

using namespace luves;

void GetInput(const TcpConnectionPtr & conn)
{
    std::cout<<conn->GetInputBuffer();
}

int main()
{
    EventLoop loop;
    Ip4Addr server_addr("127.0.0.1",6543);


    HshaServer server(&loop, server_addr,8);
    server.SetReadCb(GetInput);

    server.SetWriteCb([](const TcpConnectionPtr & conn)
                      {conn->Send("HTTP/1.1 200 OK\r\n"
                                  "Content-Type:text/html;charset=utf-8\r\n"
                                  "Content-Length:0\r\n"
                                  "\r\n"
                                  );});

    server.RunServer();
    loop.loop();
}
