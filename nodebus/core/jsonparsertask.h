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

/**
 * @brief NodeBus : JSON task parser management.
 * 
 * @author <a href="mailto:emericv@openihs.org">Emeric Verschuur</a>
 * @date 2014
 * @copyright Apache License, Version 2.0
 */


#ifndef NODEBUS_JSONPARSERTASK_H
#define NODEBUS_JSONPARSERTASK_H

#include <QRunnable>
#include <nodebus/core/jsonparser.h>
#include <nodebus/core/streamchannel.h>

namespace NodeBus {

class JSONParserTask : public QObject, public NodeBus::JSONParser, public QRunnable {
	Q_OBJECT
public:
	
	/**
	 * @brief JSONParserTask constructor.
	 * @param fd File descriptor
	 * @param parent Parent object
	 */
	JSONParserTask(const StreamChannelPtr &channel, QObject* parent = 0);
	
	/**
	 * @brief Task main
	 */
	virtual void run();
signals:
	void dataAvailable(QVariant data);
	void terminated();
public slots:
	void cancel();
private:
	StreamChannelPtr m_channel;
	bool m_stop;
};

}

#endif // NODEBUS_JSONPARSERTASK_H