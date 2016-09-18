QT += core gui widgets

CONFIG += c++11

TARGET = UPOEditor


TEMPLATE = app

SOURCES += main.cpp \
    UClassViewer.cpp \
    UColorSelectionDialog.cpp \
    UCommon.cpp \
    UD3DRenderWidget.cpp \
    UMainViewport.cpp \
    UMainWindow.cpp \
    UPropertyBrowser.cpp

LIBS += -L"Bin" -lUPOEngine

HEADERS += \
    UClassViewer.h \
    UColorSelectionDialog.h \
    UCommon.h \
    UD3DRenderWidget.h \
    UMainViewport.h \
    UMainWindow.h \
    UPropertyBrowser.h
