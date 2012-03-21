
#include <QStringList>
#include <jsonbus/core/common.h>
#include <jsonbus/core/cliarguments.h>
#include <jsonbus/core/settings.h>
#include <master.h>

namespace JSONBus {

Master::Master(int &argc, char **argv)
	: QCoreApplication(argc, argv) {
	m_cliArguments.define("daemonize",	'd', tr("Launch this service in background"));
	m_cliArguments.define("config",		'c', tr("Set a custom config path"), "/etc/jsonbus/jsonbus.conf");
	m_cliArguments.define("help",		'h', tr("Display this help"));
	m_cliArguments.define("setup",		's', tr("Setup the service"));
#ifdef WIN32
	m_cliArguments.define("win-cli",	'\0', tr("Switch to cli mode, don't launch a Windows service"));
#endif
	m_cliArguments.parse(arguments());
}

Master::~Master() {
}

void Master::launch() {
#ifdef WIN32
	Settings settings("OpenIHS.org", "JSONBus", QSettings::NativeFormat);
#else
	Settings settings(m_cliArguments.getValue("config").toString(), QSettings::NativeFormat);
#endif
	settings.define("master/pidfile",		tr("Path of the file where the service PID will be written in"),	JSONBUS_DEFAULT_PIDFILE);
	settings.define("router/listen",		tr("Addresses where the router will listen on"), 					JSONBUS_DEFAULT_LISTEN_ADDRESSES);
	settings.define("router/plugindir",		tr("Plugin dir paths"), 											JSONBUS_DEFAULT_PLUGIN_DIR_PATH);
	if (m_cliArguments.isEnabled("setup")) {
		settings.setup();
		return;
	}
}

}
