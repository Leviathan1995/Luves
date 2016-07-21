//
//  timer.h
//  Luves
//
//  Created by leviathan on 16/6/6.
//  Copyright © 2016年 leviathan. All rights reserved.
//

#ifndef TIMER_H_
#define TIMER_H_

#include <chrono>
#include <map>


namespace luves
{
    
    typedef std::pair<int64_t, int64_t> TimerId;
    typedef std::function<void ()> TimerTask;


    //重复的定时事件数据结构
    struct TimerRepeat
    {
        int64_t at;        //事件的当前超时时间
        int64_t timerPL;   //事件的当前唯一id
        int64_t interval;  //间隔时间
        TimerId timerid;   //原始TIMEID,不会改变
        TimerTask taskfunc;
    };
    
    //
    //定时事件模块
    //
    class Timer
    {
    public:
        Timer();
        ~Timer(){};
        
       
        //添加定时事件
        TimerId startTimer(int64_t delaytime,const TimerTask & task,int64_t interval=0);
        //处理超时任务
        void HandleTimeoutEvent();
        //更新下一个的最小等待时间
        void UpdateNextTimeout();
        //更新重复的定时事件
        void UpdateRepeatEvent(TimerRepeat * tr);
        //取消定时事件
        bool stopTimer(TimerId timerid);
        
        //获取系统时间,精度是millisecond
        static int64_t GetSystemTick()
            {
                return (int64_t)std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
            }
        static int64_t GetSteadyTick()
            {
                return (int64_t)std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
            }
        //获取系统时间,精度是second
        static int64_t timeSec();
        
        int64_t Getnexttimeout(){return nexttimeout_;}
        
    private:
        int64_t nexttimeout_;     //下一个定时事件的超时时间
        std::atomic<int64_t> timerPL_;  //定时事件唯一ID
        std::map<TimerId,TimerRepeat> timer_repeat_tasks_; //重复的定时事件
        std::map<TimerId,TimerTask> timertasks_ ;//定时事件
    };
}

#endif /* timer_h */
