//
//  timer.cpp
//  Luves
//
//  Created by leviathan on 16/6/6.
//  Copyright © 2016年 leviathan. All rights reserved.
//

#include "timer.h"

namespace luves
{
    //
    //定时事件模块
    //

    Timer::Timer()
    {
        timerPL_=0;
        nexttimeout_=10;
    }

    //处理超时时间
    void Timer::HandleTimeoutEvent()
    {
        int64_t now=Timer::GetTimeSecond();
        while (timertasks_.size()&&timertasks_.begin()->first.first<=now)
        {
            TimerTask task=timertasks_.begin()->second;
            timertasks_.erase(timertasks_.begin());
            task();
        }
    }

    TimerId Timer::StartTimer(int64_t delaytime,const TimerTask & task,int64_t interval)
    {
        if (interval) //重复任务
        {
            int64_t now=Timer::GetSystemTick();
            TimerId tid {delaytime+now,++timerPL_};
            TimerRepeat * tr=new TimerRepeat;
            tr->at=now+delaytime;
            tr->interval=interval;
            tr->timerid=tid;
            tr->taskfunc=task;
            timer_repeat_tasks_[tid]=*tr;

            timertasks_[tr->timerid]=[this,tr]{UpdateRepeatEvent(tr);};
            UpdateNextTimeout();
            return tr->timerid;
        }
        else    //非重复任务
        {
            TimerId tid {delaytime+Timer::GetSystemTick(),++timerPL_};
            timertasks_.insert({tid,task});
            UpdateNextTimeout();
            return tid;
        }
    }

    /*
     定时事件存储在两个map中,一个是timertasks_,重复定时事件增加一个副本存放在timer_repeat_tasks_,
     timertasks_: key为timeid,value为回调函数
     timer_repeat_tasks_:key 为timeid,value为TimerRepeat
     timer_repeat_tasks_中的key为初始timeid,不会改变
     timer_repeat_tasks_中的value中的at变量为定时事件的最新超时时间,timeSeq为定时事件的最新唯一标识,at与timeSeq会随着定时事件的处理变化
     */
    void Timer::UpdateRepeatEvent(TimerRepeat * tr)
    {
        timer_repeat_tasks_[tr->timerid].at+=tr->interval;
        timer_repeat_tasks_[tr->timerid].timerPL=++timerPL_;
        tr->at+=tr->interval;
        TimerId tid {tr->at,timerPL_};
        timertasks_[tid]=[this,tr]{UpdateRepeatEvent(tr);};

        UpdateNextTimeout();

        tr->taskfunc();
    }

    //更新下一个的最小等待时间
    void Timer::UpdateNextTimeout()
    {
        if (timertasks_.empty())
        {
            //nexttimeout_=1<<30;
        }
        else
        {
            const TimerId & t= timertasks_.begin()->first;
            nexttimeout_=t.first-Timer::GetSystemTick();
            nexttimeout_=nexttimeout_< 0 ? 0 : nexttimeout_;
        }
    }

    //取消定时事件
    bool Timer::StopTimer(TimerId timeid)
    {
        auto er=timer_repeat_tasks_.find(timeid);

        if (er==timer_repeat_tasks_.end())//非重复事件
        {
            auto e=timertasks_.find(timeid);
            if (e!=timertasks_.end())
            {
                timertasks_.erase(e);
            }
            timer_repeat_tasks_.erase(er);
            return true;
        }
        else //重复事件
        {
            TimerId ep {er->second.at,er->second.timerPL};
            auto e=timertasks_.find(ep);
            if (e!=timertasks_.end())
            {
                timertasks_.erase(e);
            }
            timer_repeat_tasks_.erase(er);
        }
        return false;
    }

    //精度为秒
    int64_t Timer::GetTimeSecond()
    {
        std::chrono::time_point<std::chrono::system_clock> p = std::chrono::system_clock::now();
        int64_t now=std::chrono::duration_cast<std::chrono::seconds>(p.time_since_epoch()).count();
        return now;
    }
}
