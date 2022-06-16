TEMPLATE = app
TARGET = hvac
QT = qml quick
CONFIG += c++11 link_pkgconfig

PKGCONFIG += qtappfw-hvac qtappfw-vehicle-signals

HEADERS += \
    translator.h

SOURCES = \
    main.cpp \
    translator.cpp

RESOURCES += \
    hvac.qrc \
    images/images.qrc

LANGUAGES = ja_JP fr_FR
include(translations.pri)

target.path = /usr/bin
target.files += $${OUT_PWD}/$${TARGET}
target.CONFIG = no_check_exist executable

INSTALLS += target
