#Luves
======
[![Build Status](https://travis-ci.org/Leviathan1995/Luves.svg?branch=master)](https://travis-ci.org/Leviathan1995/Luves)

Luves是一个轻量级的事件触发网络库,封装了Socket,简化基于Socket程序开发。目标是封装了以下三种事件的响应:IO事件,定时器事件,信号事件。支持跨平台,OS X环境使用kqueue模型,Linux环境使用Epoll模型,实现半同步/半异步(HSHA)服务器框架模型。

- [写在前面][0]
- [安装][1]
- [技术实现][2]
- [示例][3]
- [版本更新日志][4]
- [License][5]
 


##<a id="title00"/> 写在前面

将自己理解的网络框架以"学习"的标准实现,简化复杂网络库的细节部分,实现网络库的本质,必要的地方注释了代码实现思路,欢迎提PR或更好的建议。



##<a id="title01"/> 安装




##<a id="title02"> 技术实现


##<a id="title03"> 示例
简单的半同步/半异步服务器

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



##<a id="title04"/> 版本更新日志
Version 0.01

- 原型开发,目前使用kqueue,暂支持OS X系统,实现半同步半异步服务器框架.封装了IO事件与定时事件。



##<a id="title04"/> License
MIT

 [0]:#title00
 [1]:#title01
 [2]:#title02
 [3]:#title03
 [4]:#title04
 [5]:#title05
