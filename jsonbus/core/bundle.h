/*
 *   Copyright 2012-2013 Emeric Verschuur <emericv@openihs.org>
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
 * @brief JSONBus : Bundle management.
 * 
 * @author <a href="mailto:emericv@openihs.org">Emeric Verschuur</a>
 * @date 2013
 * @copyright Apache License, Version 2.0
 */

#ifndef JSONBUS_BUNDLE_H
#define JSONBUS_BUNDLE_H

#include <jsonbus/core/exception.h>
#include <jsonbus/core/bundlecontext.h>
#include <jsonbus/core/sharedlib.h>
#include <jsonbus/core/bundleactivator.h>
#include <jsonbus/core/bundlemanifest.h>

#ifndef JSONBUS_EXPORT
#define JSONBUS_EXPORT
#endif

#include <QString>

namespace JSONBus {

jsonbus_declare_exception(BundleException, Exception);

/**
 * @brief Bundle management.
 * 
 * @author <a href="mailto:emericv@openihs.org">Emeric Verschuur</a>
 * @date 2013
 * @copyright Apache License, Version 2.0
 */
class JSONBUS_EXPORT Bundle: public SharedData {
public:
	
	/// @brief Bundle state
	enum State {
		/// @brief Resolved
		UNINSTALLED,
		/// @brief Resolved
		RESOLVED,
		/// @brief Starting
		STARTING,
		/// @brief Active (started)
		ACTIVE,
		/// @brief Stopping
		STOPPING
	};
	
private:
	Bundle(const Bundle &other);
	
	/// @brief Bundle state
	State m_state;
	
	/// @brief Bundle context
	BundleContext m_context;
	
	/// @brief Shared lib file
	SharedLib m_libFile;
	
	QMutex m_mutex;
	
	BundleManifest m_manifest;
	BundleActivatorPtr m_bundleActivator;
	Qt::HANDLE m_owner;
	
	QString toString(JSONBus::Bundle::State state);
	
public:
	
	/**
	 * @brief Bundle constructor.
	 */
	Bundle(const QString &path) throw(BundleException);

	/**
	 * @brief Bundle destructor.
	 */
	~Bundle();
	
	/**
	 * @brief Get bundle manifest
	 */
	BundleManifest manifest();
	
	/**
	 * @brief Get a bundle property
	 * 
	 * @param name Property name
	 */
	QVariant property(const QString &name) const;
	
	/**
	 * @brief Install the bundle
	 */
	virtual void install() throw(BundleException);
	
	/**
	 * @brief Start the bundle
	 */
	virtual void start() throw(BundleException);
	
	/**
	 * @brief Stop the bundle
	 */
	virtual void stop() throw(BundleException);
	
	/**
	 * @brief Uninstall the bundle
	 */
	virtual void uninstall() throw(BundleException);
	
	/**
	 * @brief Get the bundle state
	 */
	State state();
	
};

/// @brief Bundle shared pointer type
typedef SharedPtr<Bundle> BundlePtr;

inline Bundle::State Bundle::state() {
	return m_state;
}
inline Bundle::~Bundle() {
	if (m_state == ACTIVE) {
		stop();
	}
}
inline QMap< QString, QVariant > Bundle::manifest() {
	return m_manifest;
}
inline QVariant Bundle::property(const QString& name) const {
	return m_manifest[name];
}

}

#endif