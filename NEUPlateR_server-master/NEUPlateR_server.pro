QT -= gui
QT += network sql

CONFIG += c++11 console
CONFIG -= app_bundle

# https://www.arubacloud.com/tutorial/how-to-install-and-configure-redis-on-ubuntu-20-04.aspx

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
#DEFINES += QT_DEPRECATED_WARNINGS QT_MESSAGELOGCONTEXT

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        main.cpp

win32{
INCLUDEPATH += C:/opencv/build/install/include
INCLUDEPATH += C:/Boost/include
INCLUDEPATH += C:/hiredis/include
LIBS += C:/opencv/build/install/x86/mingw/bin/libopencv_*.dll
LIBS += C:/Boost/lib/libboost_*.dll
LIBS += C:/hiredis/lib/libhiredis.dll
LIBS += -lgdi32
LIBS += -lwsock32
LIBS += -lws2_32
}

include(Action/Action.pri)
include(Network/Network.pri)
include(Route/Route.pri)
include(Exception/Exception.pri)
include(demo/demo.pri)
include(Photo/Photo.pri)
include(log/log.pri)
include(pool/pool.pri)
include(Business/business.pri)
include(RedisClient/RedisClient.pri)
include(test/Test.pri)

HEADERS += \
    status_code.h \
    neu_head.h
INCLUDEPATH += /usr/local/include \
               /usr/local/include/opencv4

LIBS += -lopencv_core
LIBS += -lopencv_highgui
LIBS += -lopencv_imgproc
LIBS += -lopencv_imgcodecs
LIBS += -lhiredis
LIBS += -lboost_thread -lboost_system -lboost_serialization

RESOURCES += \
    config.qrc

