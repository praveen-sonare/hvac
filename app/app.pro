TARGET = hvac
QT = quick qml websockets

HEADERS += \
    translator.h

SOURCES = main.cpp \
    translator.cpp

CONFIG += c++11 link_pkgconfig
PKGCONFIG += qtappfw-hvac

RESOURCES += \
    hvac.qrc \
    images/images.qrc

include(app.pri)

LANGUAGES = ja_JP fr_FR
include(translations.pri)
