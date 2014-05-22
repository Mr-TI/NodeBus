/*
 *   Copyright 2012-2014 Emeric Verschuur <emericv@openihs.org>
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

#include "channel.h"
#include "logger.h"
#include "selector.h"
#include "selectionkey.h"
#include <sys/ioctl.h>
#include <sys/time.h>
#include <string.h>
#include <unistd.h>
#include <QString>

#define THROW_IOEXP_ON_ERR(exp) \
	if ((exp) == -1) throw IOException(QString() + __FILE__ + ":" + QString::number(__LINE__) + ": " + strerror(errno))

namespace JSONBus {

Channel::Channel() {
}

Channel::~Channel() {
	auto list = m_keys.values();
	for (auto it = list.begin(); it != list.end(); it++) {
		(*it)->cancel();
	}
}

void Channel::close() {
	auto list = m_keys.values();
	for (auto it = list.begin(); it != list.end(); it++) {
		(*it)->cancel();
	}
}

SelectionKeyPtr Channel::registerTo(Selector& selector, int options) {
	if (m_keys.contains(&selector)) {
		m_keys[&selector]->cancel();
	}
	return new SelectionKey(selector, this, options);
}

}