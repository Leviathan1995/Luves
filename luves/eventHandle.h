//
//  event_handle.h
//  Luves
//
//  Created by leviathan on 16/5/17.
//  Copyright © 2016年 leviathan. All rights reserved.
//

#ifndef EVENT_HANDLE_H_
#define EVENT_HANDLE_H_

#include <sys/event.h>
#include <memory>
#include <vector>
#include <map>
#include <functional>
#include <unistd.h>

//#include "threads.h"
#include "timer.h"
#include "logger.h"

namespace luves {
    
    
    class EventModel;
    class Channel;
    
    //
    //事件循环
    //
    class EventLoop
    {
    public:
        typedef std::vector<Channel *> TriggerChannels;
        EventLoop();
        ~EventLoop(){};
        
        //进入事件循环
        void loop();
        
        //对通道进行操作
        void AddChannel(Channel * channel);
        void DeleteChannel(Channel *channel);
        void UpdateChannel(Channel *channel);
        
        //定时事件操作
        TimerId startTimer(int64_t delaytime,const TimerTask & task,int64_t interval=0);
        bool stopTimer(TimerId timerid);
        //获取IO模型指针
        std::shared_ptr<EventModel> &  GetIOModel(){return IOModel_;}
        
        void Exit();
        
        void SetHsha(bool hsha);
        
    private:
        bool is_hsha_;
        TriggerChannels trigger_channels_;
        bool looping_;
        bool quit_;
        std::shared_ptr<EventModel> IOModel_;   //事件循环使用的IO复用模型
        std::shared_ptr<Timer> timer_;          //定时处理单元
        
    };
    
    //监听事件类型
    const int readevent=EVFILT_READ;
    const int writeevent=EVFILT_WRITE;
    
    //
    //事件通道,接管一个fd
    //
    class Channel
    {
    public:
        Channel(EventLoop * loop,int fd):loop_(loop),fd_(fd),is_listen_(false){};
        ~Channel();
        
        //处理事件
        void HandleEvent();
        
        //设置回调函数
        void SetReadCb(const std::function<void ()> & cb){readcb_=cb;}
        void SetWriteCb(const std::function<void ()> & cb){writecb_=cb;}
        //关闭通道
        void Close();
        
        //可读可写的判断
        bool ReadEnable();
        bool WriteEnable();
        
        //获取事件描述符
        int GetFd(){return fd_;}
        //获取监听类型
        int GetEvent(){return event_;}
        //设置监听IO类型
        void SetEvent(int event){event_=event;}
        //设置活动事件的IO类型
        void SetActiveEvent(ushort active_event){active_events_=active_event;}
        
        //设置is_listen_
        void SetIsListen(bool listen){is_listen_=listen;}
        bool GetIsListen(){return is_listen_;}
        
    private:
        bool is_listen_;        //是否为listen监听套接字
        EventLoop * loop_;
        int fd_;                //事件描述符
        int event_;             //监听的IO事件类型
        ushort active_events_;  //当前活动事件的IO类型
        std::function<void ()> readcb_,writecb_; //读写回调函数
        
    };
    
    //
    //Event Model
    //
    typedef std::vector<Channel *> ChannelList;
    typedef std::map<int ,Channel*> ChannelMap;
    typedef std::vector<struct kevent*> MonitorEvents;
    
    extern int monitor_nums;

    
    class EventModel
    {
    public:
        EventModel():kq_(kqueue()),monitor_nums_(0){};
        ~EventModel(){};
        
        typedef std::vector<Channel *> ChannelList;
        typedef std::map<int ,Channel*> ChannelMap;
        
        //启动事件模型
        void RunModel(int64_t waittime);
        
        //事件通道操作,只暴露给事件循环EventLoop模块操作
        void AddChannel(Channel * channel);
        void UpdateChannel(Channel * channel);
        void DeleteChannel(Channel * channel);
                
        ChannelList  & GetTriggerPtr();
        
        void SetHsha(bool hsha){isHSHA_=hsha;}
        void SetLF(bool lf){isLF_=lf;}
        
        //传递线程池指针
        //void PassPoolPtr(ThreadsPool *  pool){pool_=pool;}
    private:
        //ThreadsPool * pool_;
        bool isHSHA_;
        bool isLF_;
        int monitor_nums_;                  //监听的事件总数
        struct kevent monitor_events[1024];
        struct kevent trigger_events[1024];
        static ChannelMap channel_fd_map_;  //channel与fd 映射
        ChannelList channel_list_;          //channel集合数组
        ChannelList trigger_channel_list_;  //触发的Channel数组
        int kq_;
        int listen_fd_;                     //监听套接字
    };
}

#endif /* event_handle_h */
