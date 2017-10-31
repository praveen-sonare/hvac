/*
 * Copyright (c) 2017 TOYOTA MOTOR CORPORATION
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

#include "qlibwindowmanager.h"
#include <QDebug>
#include <unistd.h>

int QLibWindowmanager::init(int port, const QString &token) {
    std::string ctoken = token.toStdString();
    return this->wm->init(port, ctoken.c_str());
}

int QLibWindowmanager::requestSurface(json_object *label) {
    applabel = json_object_get_string(label);

    json_object *obj = json_object_new_object();
    json_object_object_add(obj, wm->kKeyDrawingName, label);
    return this->wm->requestSurface(obj);
}

int QLibWindowmanager::activateSurface(json_object *label) {
    json_object *obj = json_object_new_object();
    json_object_object_add(obj, wm->kKeyDrawingName, label);
    json_object_object_add(obj, wm->kKeyDrawingArea, json_object_new_string("normal.full"));

    return this->wm->activateSurface(obj);
}

int QLibWindowmanager::deactivateSurface(json_object *label) {
    json_object *obj = json_object_new_object();
    json_object_object_add(obj, wm->kKeyDrawingName, label);
    return this->wm->deactivateSurface(obj);
}

int QLibWindowmanager::endDraw(json_object *label) {
    json_object *obj = json_object_new_object();
    json_object_object_add(obj, wm->kKeyDrawingName, label);
    return this->wm->endDraw(obj);
    }

void QLibWindowmanager::set_event_handler(enum QEventType et,
 handler_fun f) {
    LibWindowmanager::EventType wet = (LibWindowmanager::EventType)et;
    return this->wm->set_event_handler(wet, std::move(f));
}

void QLibWindowmanager::slotActivateSurface(){
    if(!isActive){
        qDebug("Let's show HVAC");
        isActive = true;
        this->activateSurface(json_object_new_string(applabel.c_str()));
    }
}

QLibWindowmanager::QLibWindowmanager(QObject *parent)
    :QObject(parent), isActive(false)
{
    wm = new LibWindowmanager();
}

QLibWindowmanager::~QLibWindowmanager() { }