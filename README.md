#Luves


[![Build Status](https://travis-ci.org/Leviathan1995/Luves.svg?branch=master)](https://travis-ci.org/Leviathan1995/Luves)

Luves是一个轻量级的事件触发网络库,封装了Socket,简化基于Socket程序开发。目标是封装了以下三种事件的响应:IO事件,定时器事件,信号事件。支持跨平台,OS X环境使用kqueue模型,Linux环境使用Epoll模型,实现半同步/半异步(HSHA)服务器框架模型。

- [写在前面][0]
- [安装][1]
- [编译][2]
- [示例][3]
- [测试结果][4]
- [版本更新日志][5]
- [License][6]
 


##<a id="title00"/> 写在前面

将自己理解的网络框架以"学习"的标准实现,简化复杂网络库的细节部分,实现网络库的本质,必要的地方注释了代码实现思路,欢迎提PR或更好的建议。



##<a id="title01"/> 安装

		make


##<a id="title03"> 编译
		g++ -std=c++11 test_echo.cpp -L/usr/local/lib  -lluves

##<a id="title03"> 示例
简单的Echo服务器

		#include "luves/luves.h"
		using namespace luves;

		Buffer GetInput(const TcpConnectionPtr & conn)
		{
    			return conn->GetInputBuffer();
		}

		int main()
		{
    			EventLoop loop;
   		 		Ip4Addr server_addr("0.0.0.0", 6543);
    			TcpServer server(&loop, server_addr);
    			server.SetReadCb(GetInput);
 
    			server.SetWriteCb([](const TcpConnectionPtr & conn)
                      		{conn->Send(conn->GetInputBuffer());});

    			server.RunServer();
    			loop.loop();
		}

##<a id="title04"/> Echo测试结果
	
- 使用ApacheBench测试:
	
		@ubuntu:~$ ab -n 10000 -c 10  -k  http://0.0.0.0:6543/
- 测试结果:
		
		Server Software:
		Server Hostname:        0.0.0.0
		Server Port:            6543

		Document Path:          /
		Document Length:        0 bytes

		Concurrency Level:      10
		Time taken for tests:   2.998 seconds
		Complete requests:      10000
		Failed requests:        0
		Keep-Alive requests:    0
		Total transferred:      0 bytes
		HTML transferred:       0 bytes
		Requests per second:    3335.80 [#/sec] (mean)
		Time per request:       2.998 [ms] (mean)
		Time per request:       0.300 [ms] (mean, across all concurrent requests)
		Transfer rate:          0.00 [Kbytes/sec] received

		Connection Times (ms)
              		min  mean[+/-sd] median   max
		Connect:        0    0   1.2      0      77
		Processing:     0    3   4.6      2      78
		Waiting:        0    0   0.0      0       0
		Total:          1    3   4.7      2      79

##<a id="title05"/> 版本更新日志
Version 0.01

- 原型开发,目前使用kqueue,暂支持OS X系统,实现半同步半异步服务器框架.封装了IO事件与定时事件。
- 实现跨平台,添加Linux平台的Epoll模块。
- 修复多线程日志bug
- 修复Hsha模式下kqueue模块bug


##<a id="title06"/> License
MIT

 [0]:#title00
 [1]:#title01
 [2]:#title02
 [3]:#title03
 [4]:#title04
 [5]:#title05
 [6]:#title06
