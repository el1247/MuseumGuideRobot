QT += quick

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

HEADERS += \
        include/gpio_assign.h \
        include/logic.h \
        include/MadgwickAHRS.h \
        include/movement.h \
        include/proximity.h \
        include/qr.h \
        include/sensors.h\
        include/test.h \

SOURCES += \
        src/imu.c \
        src/logic.cpp \
        src/MadgwickAHRS.c \
        src/main.cpp \
        src/movement.c \
        src/proximity.cpp \
        src/qr_module.cpp \
        src/test.cpp

RESOURCES += src/qml.qrc

TRANSLATIONS += \
    media/GUI_en_GB.ts

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

INCLUDEPATH += \
    include/

#LIBS += \
    #opencv?
