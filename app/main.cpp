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

#include <QtCore/QDebug>
#include <QtCore/QCommandLineParser>
#include <QtCore/QUrlQuery>
#include <QtGui/QGuiApplication>
#include <QtQml/QQmlApplicationEngine>
#include <QtQml/QQmlContext>
#include <QtQuickControls2/QQuickStyle>
#include <QQuickWindow>
#include <libhomescreen.hpp>
#include "qlibwindowmanager.h"

int main(int argc, char *argv[])
{
    std::string myname = std::string("HVAC");

    QGuiApplication app(argc, argv);
    app.setApplicationName(myname.c_str());
    app.setApplicationVersion(QStringLiteral("0.1.0"));
    app.setOrganizationDomain(QStringLiteral("automotivelinux.org"));
    app.setOrganizationName(QStringLiteral("AutomotiveGradeLinux"));

    QQuickStyle::setStyle("AGL");

    QCommandLineParser parser;
    parser.addPositionalArgument("port", app.translate("main", "port for binding"));
    parser.addPositionalArgument("secret", app.translate("main", "secret for binding"));
    parser.addHelpOption();
    parser.addVersionOption();
    parser.process(app);
    QStringList positionalArguments = parser.positionalArguments();

    QQmlApplicationEngine engine;
    if (positionalArguments.length() == 2) {
        int port = positionalArguments.takeFirst().toInt();
        QString secret = positionalArguments.takeFirst();
        QUrl bindingAddress;
        bindingAddress.setScheme(QStringLiteral("ws"));
        bindingAddress.setHost(QStringLiteral("localhost"));
        bindingAddress.setPort(port);
        bindingAddress.setPath(QStringLiteral("/api"));
        QUrlQuery query;
        query.addQueryItem(QStringLiteral("token"), secret);
        bindingAddress.setQuery(query);
        QQmlContext *context = engine.rootContext();
        context->setContextProperty(QStringLiteral("bindingAddress"), bindingAddress);

        std::string token = secret.toStdString();
        LibHomeScreen* hs = new LibHomeScreen();
        QLibWindowmanager* qwm = new QLibWindowmanager();

        // WindowManager
        if(qwm->init(port,secret) != 0){
            exit(EXIT_FAILURE);
        }
        if (qwm->requestSurface(json_object_new_string(myname.c_str())) != 0) {
            exit(EXIT_FAILURE);
        }
        qwm->set_event_handler(QLibWindowmanager::Event_SyncDraw, [qwm, myname](json_object *object) {
            fprintf(stderr, "Surface got syncDraw!\n");
            qwm->endDraw(json_object_new_string(myname.c_str()));
        });

        // HomeScreen
        hs->init(port, token.c_str());
        hs->set_event_handler(LibHomeScreen::Event_TapShortcut, [qwm, myname](json_object *object){
            qDebug("object %s", json_object_to_json_string(object));
            json_object *appnameJ = nullptr;
            if(json_object_object_get_ex(object, "application_name", &appnameJ))
            {
                const char *appname = json_object_get_string(appnameJ);
                qDebug("appnameJ %s", json_object_to_json_string(appnameJ));
                if(myname == appname)
                {
                    qDebug("Surface %s got tapShortcut\n", appname);
                    qwm->activateSurface(json_object_new_string(myname.c_str()));
                }
            }
        });

        engine.load(QUrl(QStringLiteral("qrc:/HVAC.qml")));
        QObject *root = engine.rootObjects().first();
        QQuickWindow *window = qobject_cast<QQuickWindow *>(root);
        QObject::connect(window, SIGNAL(frameSwapped()), qwm, SLOT(slotActivateSurface()
        ));
    }
    return app.exec();
}

