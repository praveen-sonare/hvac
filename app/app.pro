TARGET = hvac
QT = quickcontrols2 qml

SOURCES = main.cpp

CONFIG += link_pkgconfig
PKGCONFIG += libhomescreen libwindowmanager qlibwindowmanager qlibhomescreen

RESOURCES += \
    hvac.qrc \
    images/images.qrc

include(app.pri)
