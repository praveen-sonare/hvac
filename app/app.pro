TARGET = hvac
QT = quick qml

SOURCES = main.cpp

CONFIG += link_pkgconfig
PKGCONFIG += libhomescreen libwindowmanager qlibwindowmanager

RESOURCES += \
    hvac.qrc \
    images/images.qrc

include(app.pri)
