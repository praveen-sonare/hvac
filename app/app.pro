TARGET = hvac
QT = quickcontrols2

HEADERS = qlibwindowmanager.h
SOURCES = main.cpp qlibwindowmanager.cpp

CONFIG += link_pkgconfig
PKGCONFIG += libhomescreen libwindowmanager

RESOURCES += \
    hvac.qrc \
    images/images.qrc

include(app.pri)
