TEMPLATE = app

load(configure)

qtCompileTest(qlibhomescreen)
qtCompileTest(qlibwindowmanager)

config_qlibhomescreen {
    CONFIG += link_pkgconfig
    PKGCONFIG += qlibhomescreen
    DEFINES += HAVE_QLIBHOMESCREEN
}

config_qlibwindowmanager {
    CONFIG += link_pkgconfig
    PKGCONFIG += qlibwindowmanager
    DEFINES += HAVE_QLIBWINDOWMANAGER
}

DESTDIR = $${OUT_PWD}/../package/root/bin
