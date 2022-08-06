TEMPLATE = app
TARGET = hvac
QT = qml quick dbus
CONFIG += c++11 link_pkgconfig

DBUS_INTERFACES = $$[QT_SYSROOT]/usr/share/dbus-1/interfaces/org.automotivelinux.AppLaunch.xml

PKGCONFIG += qtappfw-hvac qtappfw-vehicle-signals

HEADERS += \
    homescreenhandler.h \
    translator.h

SOURCES = \
    main.cpp \
    homescreenhandler.cpp \
    translator.cpp \

RESOURCES += \
    hvac.qrc \
    images/images.qrc

LANGUAGES = ja_JP fr_FR
include(translations.pri)

target.path = /usr/bin
target.files += $${OUT_PWD}/$${TARGET}
target.CONFIG = no_check_exist executable

INSTALLS += target
