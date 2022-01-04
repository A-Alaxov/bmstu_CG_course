QT       += core gui \
            multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
#QMAKE_CXXFLAGS -= -latomic
#QMAKE_LFLAGS -= -latomic
#QMAKE_CXXFLAGS -= -fno-exceptions
#QMAKE_LFLAGS -= -fno-exceptions
#QMAKE_CXXFLAGS -= -fno-xxxx
#QMAKE_LFLAGS -= -fno-xxxx
#QMAKE_CXXFLAGS -= -Og
#QMAKE_LFLAGS -= -Og
#QMAKE_CXXFLAGS += -O0
#QMAKE_LFLAGS += -O0
#QMAKE_CXXFLAGS -= -O1
#QMAKE_LFLAGS -= -O1
#QMAKE_CXXFLAGS -= -O2
#QMAKE_LFLAGS -= -O2
QMAKE_CXXFLAGS += -O3
QMAKE_LFLAGS += -O3

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    dot.cpp \
    drawer.cpp \
    illuminant.cpp \
    illuminantplacechooser.cpp \
    main.cpp \
    mainwindow.cpp \
    model_file_loader.cpp \
    objects.cpp \
    specialgraphicsview.cpp \
    surface.cpp \
    usagefacade.cpp

HEADERS += \
    base_loader.hpp \
    config.hpp \
    dot.hpp \
    drawer.hpp \
    errors.hpp \
    illuminant.hpp \
    illuminantplacechooser.hpp \
    mainwindow.h \
    model_file_loader.hpp \
    objects.hpp \
    specialgraphicsview.hpp \
    surface.hpp \
    usagefacade.hpp

FORMS += \
    illuminantplacechooser.ui \
    mainwindow.ui

RC_ICONS += imgs\icon.ico

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
