QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    advancetranslatepair.cpp \
    filetreewidget.cpp \
    main.cpp \
    listwidget.cpp \
    mainwidget.cpp \
    mainwindow.cpp \
    scrollbar.cpp \
    settingswidget.cpp \
    tableviewfilters.cpp \
    translatepair.cpp

HEADERS += \
    advancetranslatepair.h \
    filetreewidget.h \
    listwidget.h \
    mainwidget.h \
    mainwindow.h \
    scrollbar.h \
    settingswidget.h \
    tableviewfilters.h \
    translatepair.h

CONFIG += lrelease

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
