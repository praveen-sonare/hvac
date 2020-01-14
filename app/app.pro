TARGET = hvac
QT = quick qml aglextras

HEADERS += \
    translator.h

SOURCES = main.cpp \
    translator.cpp

CONFIG += link_pkgconfig
PKGCONFIG += qtappfw

RESOURCES += \
    hvac.qrc \
    images/images.qrc

include(app.pri)

LANGUAGES = ja_JP fr_FR
include(translations.pri)
