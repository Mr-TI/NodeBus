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
 * @brief NodeBus : NodeBus slave application management.
 * 
 * @author <a href="mailto:emericv@openihs.org">Emeric Verschuur</a>
 * @date 2014
 * @copyright Apache License, Version 2.0
 */

#ifndef NODEBUS_SLAVEAPPLICATION_H
#define NODEBUS_SLAVEAPPLICATION_H

#include <QCoreApplication>
#include <nodebus/core/application.h>
#include "settings.h"

namespace NodeBus {

/**
 * @brief NodeBus container management.
 */
class SlaveApplication : public Application {
	Q_OBJECT
public:
	/**
	 * @brief Service constructor.
	 */
	SlaveApplication(int &argc, char **argv);

	/**
	 * @brief Service destructor.
	 */
	~SlaveApplication();
	
protected:
	/**
	 * @brief Method called during start process
	 */
	virtual void onStart();
	
protected slots:
	/**
	 * @brief Inpout data treatment
	 * @param data
	 */
	virtual void onDataAvailable(QVariant data) = 0;
};

}

#endif //NODEBUS_SLAVEAPPLICATION_H