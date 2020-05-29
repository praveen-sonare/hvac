/*
 * Copyright © 2020 Collabora Ltd.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice (including the
 * next paragraph) shall be included in all copies or substantial
 * portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT.  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef SHELLDESKTOP_H
#define SHELLDESKTOP_H

#include <QObject>
#include <QString>
#include <QScreen>
#include <QWindow>
#include <QDebug>
#include <memory>

#include "wayland-agl-shell-desktop-client-protocol.h"

static void
application_id_event(void *data, struct agl_shell_desktop *agl_shell_desktop,
		     const char *app_id);
static void
application_state_event(void *data, struct agl_shell_desktop *agl_shell_desktop,
                        const char *app_id, const char *app_data,
                        uint32_t app_state, uint32_t app_role);

static const struct agl_shell_desktop_listener agl_shell_desktop_listener = {
        application_id_event,
        application_state_event,
};

class Shell : public QObject
{
Q_OBJECT

public:
	std::shared_ptr<struct agl_shell_desktop> shell;
	Shell(std::shared_ptr<struct agl_shell_desktop> shell, QObject *parent = nullptr) :
		QObject(parent), shell(shell)
	{
		struct agl_shell_desktop *agl_shell_desktop = shell.get();
		agl_shell_desktop_add_listener(agl_shell_desktop,
					       &agl_shell_desktop_listener, this);
	}

public slots: // calls out of qml into CPP
	void set_window_on_screen(QScreen *screen, const QString &app_id);
};

static void
application_id_event(void *data, struct agl_shell_desktop *agl_shell_desktop,
		     const char *app_id)
{
        Shell *aglShell = static_cast<Shell *>(data);
        (void) agl_shell_desktop;
}

static void
application_state_event(void *data, struct agl_shell_desktop *agl_shell_desktop,
                        const char *app_id, const char *app_data,
                        uint32_t app_state, uint32_t app_role)
{
	(void) data;
	(void) agl_shell_desktop;
	(void) app_id;
	(void) app_data;
	(void) app_state;
	(void) app_role;
}

#endif // SHELLDESKTOP_H
