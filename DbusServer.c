#include <vdr/tools.h>
#include <string.h>
#include <errno.h>
#include <iostream>

#include "DbusServer.h"

cPluginDbusServer *cPluginDbusServer::m_Instance = NULL;

cPluginDbusServer::cPluginDbusServer(void):
		cThread("dbus server")
{
	Start();
}

cPluginDbusServer::~cPluginDbusServer()
{
	Stop();
}

void cPluginDbusServer::Initialize(void)
{
	dsyslog("status: cPluginDbusServer::Initialize");

	if (m_Instance == NULL) {
		dsyslog("status: cPluginDbusServer thread started");
		m_Instance = new cPluginDbusServer;
	}
}

void cPluginDbusServer::Destruct(void)
{
	DELETENULL(m_Instance);
}

void cPluginDbusServer::Stop(void)
{
    dispatcher.leave();
	if (Running())
		Cancel(3);
}

void cPluginDbusServer::Action(void)
{
	DBus::default_dispatcher = &dispatcher;
	DBus::Connection conn = DBus::Connection::SystemBus();
	
	conn.request_name("org.vdr.Events");
	PluginDBusServer server(conn);

	dispatcher.enter();
}
