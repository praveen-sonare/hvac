TARGET = hvac
QT = quick qml websockets

CONFIG += c++11 link_pkgconfig
PKGCONFIG += qtappfw afb-helpers-qt

#aglextras

HEADERS += \
    translator.h

SOURCES = main.cpp \
    translator.cpp

RESOURCES += \
    hvac.qrc \
    images/images.qrc

include(app.pri)

LANGUAGES = ja_JP fr_FR
include(translations.pri)
