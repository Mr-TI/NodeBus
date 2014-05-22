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

#include <common.h>
#include "logger.h"
#include "shareddata.h"

namespace JSONBus {

static std::atomic_uint_fast64_t __count;

SharedData::SharedData(): ref(0) {
	logFinest() << "SharedData::new " << "[0x" << 
		toHexString(this) << "] (count=" << ++__count << ")";
}
SharedData::~SharedData() {
	logFinest() << "SharedData::del " << "[0x" << 
		toHexString(this) << "] (count=" << --__count << ")";
}

}