
#include <QStringList>
#include <jsonbus/core/common.h>
#include <jsonbus/core/cliarguments.h>
#include <jsonbus/core/settings.h>
#include <jsonbus/core/sharedlib.h>
#include <jsonbus/core/plugin.h>
#include <container.h>
#ifdef WIN32
#	define JSONBUS_SERVICEFILE_PREFIX ""
#	define JSONBUS_SERVICEFILE_SUFFIX ".dll"
#else
#	define JSONBUS_SERVICEFILE_PREFIX "lib"
#	define JSONBUS_SERVICEFILE_SUFFIX ".so"
#endif

namespace JSONBus {

typedef Plugin& (*get_singleton_t) ();

Container::Container(int &argc, char **argv)
	: QCoreApplication(argc, argv) {
	m_cliArguments.define("service-root",	'd', tr("Plugin root directory (excluding namaspace directory)"), "/usr/lib/jsonbus/services/");
	m_cliArguments.define("service-ns",		'N', tr("Plugin namespace"), "");
	m_cliArguments.define("service-name",	'n', tr("Plugin name"), "");
	m_cliArguments.define("service-path",	'f', tr("Plugin path"), "");
	m_cliArguments.define("config",			'c', tr("Set a custom config path"), "");
	m_cliArguments.define("setup",			's', tr("Setup the service"));
	m_cliArguments.define("help",			'h', tr("Display this help"));
	m_cliArguments.parse(arguments());
}

Container::~Container() {
}

void Container::launch() {
#ifdef WIN32
	Settings settings("OpenIHS.org", "JSONBus::" + m_cliArguments.getValue("service-name").toString(), QSettings::NativeFormat);
#else
	Settings settings(m_cliArguments.getValue("config").toString(), QSettings::NativeFormat);
#endif
	if (m_cliArguments.isEnabled("setup")) {
		settings.setup();
		return;
	}
	QString serviceRoot = m_cliArguments.getValue("service-root").toString();
	QString serviceName = m_cliArguments.getValue("service-name").toString();
	QString serviceNs = m_cliArguments.getValue("service-ns").toString();
	QString servicePath = m_cliArguments.getValue("service-path").toString();
	
	if (serviceNs.isEmpty()) {
		throw new ContainerException("Undefinied service namespace");
	}
	if (serviceName.isEmpty()) {
		throw new ContainerException("Undefinied service name");
	}
	
	if (servicePath.isEmpty()) {
		if (serviceRoot.isEmpty()) {
			throw new ContainerException("Undefinied root service directory");
		}
		servicePath = serviceRoot + "/" + serviceNs + "/" JSONBUS_SERVICEFILE_PREFIX + serviceName + JSONBUS_SERVICEFILE_SUFFIX;
	}
	
	SharedLib pluginlib(servicePath);
	
	get_singleton_t *getSingleton = static_cast<get_singleton_t*>(pluginlib.getSymbol("getSingleton"));
	
	Plugin &plugin = *getSingleton();
	plugin.onInit();
}

}