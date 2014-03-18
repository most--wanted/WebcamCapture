#-------------------------------------------------
#
# Project created by QtCreator 2014-03-18T05:20:05
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = WebCamFinal
TEMPLATE = app


SOURCES +=  main.cpp\
            mainwindow.cpp\
            devicemanager.cpp

HEADERS  += mainwindow.h \
            devicemanager.h

FORMS    += mainwindow.ui

# linux includes
unix:!macx: LIBS += -L$$PWD/../opencv-2.4.8/lib/ -lopencv_core \
                    -L$$PWD/../opencv-2.4.8/lib/ -lopencv_highgui \

unix:!macx: INCLUDEPATH += $$PWD/../opencv-2.4.8/include
unix:!macx: DEPENDPATH += $$PWD/../opencv-2.4.8/include

# windows includes

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../opencv/build/x64/vc12/lib/ -lopencv_core248 \
                                              -L$$PWD/../opencv/build/x64/vc12/lib/ -lopencv_highgui248

else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../opencv/build/x64/vc12/lib/ -lopencv_core248d \
                                                 -L$$PWD/../opencv/build/x64/vc12/lib/ -lopencv_highgui248d

win32: INCLUDEPATH += $$PWD/../opencv/build/include
win32: DEPENDPATH += $$PWD/../opencv/build/include
