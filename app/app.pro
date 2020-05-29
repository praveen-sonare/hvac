TARGET = hvac
QT = quick qml websockets gui-private

CONFIG += c++11 link_pkgconfig wayland-scanner
PKGCONFIG += qtappfw afb-helpers-qt wayland-client

#aglextras

HEADERS += shell-desktop.h \
    translator.h

SOURCES = main.cpp shell-desktop.cpp \
    translator.cpp

RESOURCES += \
    hvac.qrc \
    images/images.qrc

include(app.pri)

LANGUAGES = ja_JP fr_FR
include(translations.pri)

WAYLANDCLIENTSOURCES += \
	protocol/agl-shell-desktop.xml

