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

#ifndef HOMESCREENHANDLER_H
#define HOMESCREENHANDLER_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <string>

#include "applaunch_interface.h"

using namespace std;

class HomescreenHandler : public QObject
{
    Q_OBJECT
public:
    explicit HomescreenHandler(QObject *parent = 0);
    ~HomescreenHandler();

    Q_INVOKABLE void setOrientation(QString application_id, uint32_t orientation);
    void onRep(struct json_object* reply_contents);

private:
    org::automotivelinux::AppLaunch *applaunch_iface;
};

#endif // HOMESCREENHANDLER_H
