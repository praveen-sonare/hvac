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

#include <QtAGLExtras/AGLApplication>
#include <QtQml/QQmlApplicationEngine>
#include <hvac.h>

#include "translator.h"

int main(int argc, char *argv[])
{
    AGLApplication app(argc, argv);
    app.setApplicationName("HVAC");
    app.setupApplicationRole("hvac");

    QQmlApplicationEngine *engine = app.getQmlApplicationEngine();
    QQmlContext *context = engine->rootContext();
    QVariant v = context->contextProperty(QStringLiteral("bindingAddress"));
    if(v.canConvert(QMetaType::QUrl)) {
        QUrl bindingAddress = v.toUrl();
        context->setContextProperty("hvac", new HVAC(bindingAddress));
    } else {
        qCritical("Cannot find bindingAddress property in context, SignalComposer unavailable");
    }

    qmlRegisterType<Translator>("Translator", 1, 0, "Translator");
    app.load(QUrl(QStringLiteral("qrc:/HVAC.qml")));
    return app.exec();
}

