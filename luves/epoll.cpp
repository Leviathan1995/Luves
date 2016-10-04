//
//	epoll.cpp
//	Luves
//
//	Created by leviathan on 16/10/2
//	Copyright @ 2016 leviathan. All rights reserved
//

#ifdef __linux__

#include "epoll.h"

namespace luves{

	//
	// IO复用模型-Epoll
	//

	std::map<int, Channel *> EpollModel::channel_fd_map_;

	EpollModel::EpollModel()
	{
		ep_ = epoll_create(1024);
		max_events_ = 1024;
	}

	void EpollModel::AddChannel(Channel * channel)
	{
		struct epoll_event event;
		event.data.fd = channel->GetFd();
		event.events = EPOLLIN |EPOLLET;

		if(channel->GetIsListen())
			listen_fd_ = channel->GetFd();
		channel_fd_map_[channel->GetFd()] = channel;
		int ret = epoll_ctl(ep_, EPOLL_CTL_ADD, channel->GetFd(), &event);
		if(ret == -1)
		{
			ERROR_LOG("Epoll add file description failed.");
		}
	}

	void EpollModel::DeleteChannel(Channel * channel)
	{
		int ret = epoll_ctl(ep_, EPOLL_CTL_DEL, channel->GetFd(), NULL);
		if(ret == -1)
		{
			ERROR_LOG("Epoll delete file description failed.");
		}
	}

	void EpollModel::UpdateChannel(Channel * channel)
	{

	}

	void EpollModel::RunModel(int64_t wait_time)
	{
		int nfds = epoll_wait(ep_, trigger_events_, max_events_, -1);
		trigger_channel_list_.clear();
			for (int i = 0; i < nfds; ++i)
			{
				if ((trigger_events_[i].events & EPOLLERR) ||
						(trigger_events_[i].events & EPOLLHUP) ||
						(!(trigger_events_[i].events & EPOLLIN)))
				{
					ERROR_LOG("Epoll return error %d %s",errno, strerror(errno));
					exit(1);
				}
				else if((listen_fd_ == trigger_events_[i].data.fd) ||
						((trigger_events_[i].events & EPOLLIN) && (is_hsha_ == false)))
				{
					auto event = & trigger_events_[i];
					auto channel = channel_fd_map_.find(int(event->data.fd))->second;
					channel->SetActiveEvent(event->events);
					trigger_channel_list_.push_back(channel_fd_map_.find(int(event->data.fd))->second);
				}
				else if(trigger_events_[i].events & EPOLLIN && is_hsha_)
				{
					ThreadsPool::AddTask(int(trigger_events_[i].data.fd));
				}
			}
	}

	ChannelList & EpollModel::GetTriggerPtr()
	{
		return trigger_channel_list_;
	}
}

#endif /* LINUX */
