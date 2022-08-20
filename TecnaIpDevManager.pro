QT += quick virtualkeyboard

SOURCES += \
        main.cpp

#resources.files = main.qml
#resources.prefix = /$${TARGET}

#RESOURCES += resources \
#    tecnaResFile.qrc

TRANSLATIONS += \
    TecnaIpDevManager_en_150.ts
CONFIG += lrelease
CONFIG += embed_translations
CONFIG += console
CONFIG+=qtquickcompiler

RC_ICONS = Risorse/Logo.ico

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resFile.qrc
