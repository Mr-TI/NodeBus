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

/**
 * @brief Sample NodeBus HelloWorld bundle.
 * 
 * @author <a href="mailto:emericv@mbedsys.org">Emeric Verschuur</a>
 * @date 2014
 * @copyright Apache License, Version 2.0
 */

#ifndef NODEBUS_BUNDLES_HELLOWORLD_H
#define NODEBUS_BUNDLES_HELLOWORLD_H

#include <nodebus/core/exception.h>
#include <nodebus/bundle/bundleactivator.h>

using namespace NodeBus;

class HelloWorld: public BundleActivator {
	virtual void start(BundleContext& context);
	virtual void stop(BundleContext& context);
};

#endif // NODEBUS_BUNDLES_HELLOWORLD_H
