/*
 * Copyright (C) 2016 The Qt Company Ltd.
 * Copyright (C) 2022 Konsulko Group
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

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QDebug>
#include <hvac.h>
#include <vehiclesignals.h>

#include "translator.h"

int main(int argc, char *argv[])
{
	setenv("QT_QUICK_CONTROLS_STYLE", "AGL", 1);

	QGuiApplication app(argc, argv);

	QQmlApplicationEngine engine;
	VehicleSignalsConfig vsConfig("hvac");
	engine.rootContext()->setContextProperty("hvac", new HVAC(new VehicleSignals(vsConfig)));
	qmlRegisterType<Translator>("Translator", 1, 0, "Translator");
	engine.load(QUrl(QStringLiteral("qrc:/HVAC.qml")));

	return app.exec();
}
