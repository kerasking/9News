HEADERS += \
    $$PWD/selectfilesdialog.h

SOURCES += \
    $$PWD/selectfilesdialog.cpp

symbian{
    message(Symbian build)
    RESOURCES += \
        $$PWD/symbian.qrc
    OTHER_FILES += \
        $$PWD/symbian.qml
}

simulator{
    message(Simulator build)
    RESOURCES += \
        $$PWD/symbian.qrc

    OTHER_FILES += \
        $$PWD/symbian.qml \
        $$PWD/meego.qml
}

contains(MEEGO_EDITION, harmattan){
    message(Harmattan build)
    DEFINES += HARMATTAN_BOOSTER

    RESOURCES += \
        $$PWD/meego.qrc

    OTHER_FILES += \
        $$PWD/meego.qml
}

RESOURCES += \
    $$PWD/icons.qrc
