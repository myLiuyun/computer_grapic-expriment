QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Circle.cpp \
    Ellipse.cpp \
    HolePolygon.cpp \
    interface/BoundboxInterface.cpp \
    interface/BoundboxInterface.cpp \
    CircleEngine.cpp \
    EllipseControlWidget.cpp \
    EllipseEngine.cpp \
    HexagonPolygon.cpp \
    Line.cpp \
    LineEngine.cpp \
    Polygon.cpp \
    PolygonEngine.cpp \
    PolygonFillEngine.cpp \
    SquarePolygon.cpp \
    StarPolygon.cpp \
    interface/TransformInterface.cpp \
    interface/TransformInterface.cpp \
    TransformWidget.cpp \
    drawengine.cpp \
    main.cpp \
    MainWindow.cpp \
    mainCanvas.cpp

HEADERS += \
    Circle.h \
    Ellipse.h \
    HolePolygon.h \
    interface/BoundboxInterface.h \
    interface/BoundboxInterface.h \
    CircleEngine.h \
    EllipseControlWidget.h \
    EllipseEngine.h \
    HexagonPolygon.h \
    Line.h \
    LineEngine.h \
    MainWindow.h \
    Polygon.h \
    PolygonEngine.h \
    PolygonFillEngine.h \
    SquarePolygon.h \
    StarPolygon.h \
    interface/TransformInterface.h \
    interface/TransformInterface.h \
    TransformWidget.h \
    drawengine.h \
    mainCanvas.h

FORMS += \
    EllipseControlWidget.ui \
    TransformWidget.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

INCLUDEPATH += $$PWD/Interface
