/*
 * Copyright (c) 2017 TOYOTA MOTOR CORPORATION
 * Copyright (c) 2018,2019 TOYOTA MOTOR CORPORATION
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

#include <QDBusMessage>
#include <QDBusConnection>
#include "homescreenhandler.h"

#define APPLAUNCH_DBUS_IFACE     "org.automotivelinux.AppLaunch"
#define APPLAUNCH_DBUS_OBJECT    "/org/automotivelinux/AppLaunch"

HomescreenHandler::HomescreenHandler(QObject *parent) : QObject(parent)
{
    applaunch_iface = new org::automotivelinux::AppLaunch(APPLAUNCH_DBUS_IFACE, APPLAUNCH_DBUS_OBJECT, QDBusConnection::sessionBus(), this);
}

HomescreenHandler::~HomescreenHandler()
{
}

void HomescreenHandler::setOrientation(QString application_id, uint32_t orientation)
{
    QDBusPendingReply<> reply = applaunch_iface->split(application_id, orientation);
    reply.waitForFinished();
    if (reply.isError()) {
        fprintf(stderr, "HVAC: Unable to split application '%s': %s",
                  application_id.toStdString().c_str(),
                  reply.error().message().toStdString().c_str());
    }

    fprintf(stderr, "hvac done calling set split for appid %s, orientation %d\n",
		    application_id.toStdString().c_str(), orientation);
}
