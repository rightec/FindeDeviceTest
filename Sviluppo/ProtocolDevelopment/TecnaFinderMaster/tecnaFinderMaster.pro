QT += network widgets
requires(qtConfig(udpsocket))

HEADERS       = \
    modelviewdialog.h \
    tecnaProtocol.h \
    tecnaProtocolDef.h \
    tecna_gui_def.h \
    tecna_master.h \
    tecna_protocol.h
SOURCES       = \
                main.cpp \
                modelviewdialog.cpp \
                tecnaProtol.c \
                tecna_master.cpp \
                tecna_protocol.cpp

# install
target.path = $$[QT_INSTALL_EXAMPLES]/network/broadcastsender
INSTALLS += target

CONFIG += console

FORMS += \
    modelviewdialog.ui

#win32 {
#DESTDIR = $$PWD/bin
#QMAKE_POST_LINK =  windeployqt $$shell_path($$DESTDIR/$${TARGET}.exe)
#}
