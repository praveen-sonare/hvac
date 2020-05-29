/*
 * Copyright (C) 2016 The Qt Company Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <QtGui/QGuiApplication>
#include <QDebug>
#include <QUrlQuery>
#include <QCommandLineParser>
#include <QtQml/QQmlApplicationEngine>
#include <QtQml/QQmlContext>
#include <QScreen>

#include <qpa/qplatformnativeinterface.h>
#include <wayland-client.h>

#include "shell-desktop.h"
#include "wayland-agl-shell-desktop-client-protocol.h"

#include "translator.h"

/* this could be a combintion or some sort */
#define OUTPUT_ID       "Virtual-1"

static void
global_add(void *data, struct wl_registry *reg, uint32_t name,
		const char *interface, uint32_t)
{
	struct agl_shell_desktop **shell = static_cast<struct agl_shell_desktop **>(data);

	if (strcmp(interface, agl_shell_desktop_interface.name) == 0) {
		*shell = static_cast<struct agl_shell_desktop *>(
				wl_registry_bind(reg, name, &agl_shell_desktop_interface, 1)
				);
	}
}

static void
global_remove(void *data, struct wl_registry *reg, uint32_t id)
{
	/* Don't care */
	(void) data;
	(void) reg;
	(void) id;
}

static const struct wl_registry_listener registry_listener = {
	global_add,
	global_remove,
};

static struct agl_shell_desktop *
register_agl_shell_desktop(QPlatformNativeInterface *native)
{
	struct wl_display *wl;
	struct wl_registry *registry;
	struct agl_shell_desktop *shell = nullptr;

	wl = static_cast<struct wl_display *>(native->nativeResourceForIntegration("display"));
	registry = wl_display_get_registry(wl);

	wl_registry_add_listener(registry, &registry_listener, &shell);

	/* Roundtrip to get all globals advertised by the compositor */
	wl_display_roundtrip(wl);
	wl_registry_destroy(registry);

	return shell;
}

static QScreen *
find_qscreen(const char *screen_name)
{
	QList<QScreen *> screens = qApp->screens();
	QScreen *found = nullptr;
	QString qstr_name = QString::fromUtf8(screen_name, -1);

	for (QScreen *xscreen : screens) {
		if (qstr_name == xscreen->name()) {
			found = xscreen;
			break;
		}
	}

	return found;
}


int main(int argc, char *argv[])
{
	setenv("QT_QPA_PLATFORM", "wayland", 1);
	int port;
	QString token;
	QString graphic_role = "hvac";
	struct agl_shell_desktop *agl_shell_desktop = nullptr;

	QCommandLineParser parser;
	QGuiApplication app(argc, argv);
	app.setDesktopFileName(graphic_role);

	parser.addPositionalArgument("port",
		app.translate("main", "port for binding"));
	parser.addPositionalArgument("secret",
		app.translate("main", "secret for binding"));

        parser.addHelpOption();
        parser.addVersionOption();
        parser.process(app);
        QStringList positionalArguments = parser.positionalArguments();

	if (positionalArguments.length() == 2) {
		port = positionalArguments.takeFirst().toInt();
		token = positionalArguments.takeFirst();
		qInfo() << "setting port:" << port << ", token:" << token;
	} else {
		qInfo() << "Need to specify port and token";
		exit(EXIT_FAILURE);
	}

	QPlatformNativeInterface *native = qApp->platformNativeInterface();
	agl_shell_desktop = register_agl_shell_desktop(native);
	if (!agl_shell_desktop) {
		qDebug() << "Could not find agl_shell_desktop extension. Is agl-compositor running?";
		exit(EXIT_FAILURE);
	}

	std::shared_ptr<struct agl_shell_desktop> shell{agl_shell_desktop, agl_shell_desktop_destroy};
	Shell *aglShell = new Shell(shell, nullptr);

	QScreen *screen_to_put = find_qscreen(OUTPUT_ID);
	if (screen_to_put)
		aglShell->set_window_on_screen(screen_to_put, graphic_role);

	QUrl bindingAddress;
	bindingAddress.setScheme(QStringLiteral("ws"));
	bindingAddress.setHost(QStringLiteral("localhost"));
	bindingAddress.setPort(port);
	bindingAddress.setPath(QStringLiteral("/api"));

	QUrlQuery query;
	query.addQueryItem(QStringLiteral("token"), token);
	bindingAddress.setQuery(query);

	QQmlApplicationEngine engine;
	engine.rootContext()->setContextProperty("bindingAddress", bindingAddress);
	qmlRegisterType<Translator>("Translator", 1, 0, "Translator");
	engine.load(QUrl(QStringLiteral("qrc:/HVAC.qml")));

	return app.exec();
}
