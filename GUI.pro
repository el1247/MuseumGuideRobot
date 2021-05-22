QT += quick

CONFIG += c++11 \
        opencv4

PKGCONFIG += opencv4

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

HEADERS += \
        include/gpio_assign.h \
        #include/logic.hpp \
        include/logic_qml.h \
        include/MadgwickAHRS.h \
        include/mapstruct.h \
        include/movement.h \
        include/nav.h \
        include/proximity.h \
        include/qr.hpp \
        include/sensors.h \
        include/gsound.h

SOURCES += \
        src/imu.c \
        #src/filereader.c \
        src/filereader.cpp \
        #src/logic.cpp \
        src/logic_qml.cpp \
        src/MadgwickAHRS.c \
        src/main.cpp \
        src/nav.c \
        src/movement.c \
        src/proximity.cpp \
        src/qr_module.cpp \
        src/sound.c

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


LIBS += \
    `pkg-config --cflags --libs opencv` \
    -lpigpio \
    -lzbar \
    -lrt \
    -L/opt/opencv-4.1.0/lib \
    -lopencv_core \
    -lopencv_highgui \
    -lopencv_imgproc \
    -lsndfile \
    -lao \
    -lopencv_videoio

INCLUDEPATH += \
    include/ \
    /opt/opencv-4.1.0/include/opencv4
