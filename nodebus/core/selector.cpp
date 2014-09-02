/*
 *   Copyright 2012-2014 Emeric Verschuur <emericv@mbedsys.org>
 *
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *   You may obtain a copy of the License at
 *
 *		   http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 */

#include "selector.h"
#include "selectionkey.h"
#include "logger.h"
#include "serversocketchannel.h"
#include <sys/ioctl.h>
#include <sys/time.h>
#include <string.h>
#include <unistd.h>

#define NODEBUS_SELECTOR_EPOLL_EVENT_SIZE 64

#define THROW_IOEXP_ON_ERR(exp) \
	if ((exp) == -1) throw IOException(QString() + __FILE__ + ":" + QString::number(__LINE__) + ": " + QString::fromLocal8Bit(strerror(errno)))

namespace NodeBus {

Selector::Selector() : m_enabled(true), m_synchronize(QMutex::Recursive) {
#ifdef WIN32
	
#else //WIN32
	THROW_IOEXP_ON_ERR(m_epfd = epoll_create1(0));
	m_events = new epoll_event[NODEBUS_SELECTOR_EPOLL_EVENT_SIZE];
#endif //WIN32
}

Selector::~Selector() {
	QMutexLocker locker(&m_synchronize);
	auto list = m_keys.values();
	for (auto it = list.begin(); it != list.end(); it++) {
		(*it)->cancel();
	}
#ifdef WIN32
	
#else //WIN32
	delete[] m_events;
	::close(m_epfd);
#endif //WIN32
}

bool Selector::select(int timeout) {
	QMutexLocker locker(&m_synchronize);
#ifdef WIN32
	
#else //WIN32
	ssize_t ret = 0, fdc;
	m_pendingKeys.clear();
	locker.unlock();
	while (m_enabled && timeout != 0) {
		locker.relock();
		ret = epoll_wait(m_epfd, m_events,NODEBUS_SELECTOR_EPOLL_EVENT_SIZE ,1);
		if (errno == EINTR) {
			ret = 0;
			continue;
		}
		THROW_IOEXP_ON_ERR(ret);
		if (ret > 0) {
			for (ssize_t i = 0; i < ret; i++) {
				fdc = m_events[i].data.fd;
				THROW_IOEXP_ON_ERR(epoll_ctl (m_epfd, EPOLL_CTL_DEL, fdc, NULL));
				if (!m_keys.contains(fdc)) {
					continue;
				}
				SelectionKeyPtr key = m_keys[fdc];
				m_keys.remove(fdc);
				key->channel()->m_keys.remove(this);
				key->channel()->updateStatus(m_events[i].events);
				key->m_events = m_events[i].events;
				m_pendingKeys.append(key);
			}
			return true;
		}
		timeout--;
		locker.unlock();
	}
#endif //WIN32
	return false;
}

QList< SelectionKeyPtr > Selector::selectedKeys() {
	QMutexLocker _(&m_synchronize);
	return m_pendingKeys;
}


void Selector::put(const SelectionKeyPtr& key, int events) {
	QMutexLocker _(&m_synchronize);
#ifdef WIN32
	
#else //WIN32
	m_keys[key->channel()->fd()] = key;
	bzero(&m_event, sizeof(epoll_event));
	m_event.data.fd = key->channel()->fd();
	m_event.events = events | EPOLLET;
	THROW_IOEXP_ON_ERR(epoll_ctl (m_epfd, EPOLL_CTL_ADD, m_event.data.fd, &m_event));
#endif //WIN32
}

void Selector::remove(const SelectionKeyPtr& key) {
	QMutexLocker _(&m_synchronize);
#ifdef WIN32
	
#else //WIN32
	m_keys.remove(key->channel()->fd());
	THROW_IOEXP_ON_ERR(epoll_ctl (m_epfd, EPOLL_CTL_DEL, key->channel()->fd(), NULL));
#endif //WIN32
}

}
