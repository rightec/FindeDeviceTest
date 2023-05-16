QT += network widgets
requires(qtConfig(udpsocket))

HEADERS       = \
    TP_Protocol/tecnaProtocol.h \
    TP_Protocol/tecnaProtocolDef.h \
    tecna_gui_def.h \
    tecna_protocol.h \
    tecna_slave.h
SOURCES       = \
                TP_Protocol/tecnaProtol.c \
                main.cpp \
                tecna_protocol.cpp \
                tecna_slave.cpp

# install
target.path = $$[QT_INSTALL_EXAMPLES]/network/broadcastsender
INSTALLS += target

CONFIG += console

FORMS +=

#win32 {
#DESTDIR = $$PWD/bin
#QMAKE_POST_LINK =  windeployqt $$shell_path($$DESTDIR/$${TARGET}.exe)
#}
