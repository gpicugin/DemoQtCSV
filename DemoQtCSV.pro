QT = core

CONFIG += c++17 cmdline

TARGET = Demo


# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        Cortege.cpp \
        main.cpp \
        trendslibrary/AlarmTracker.cpp \
        trendslibrary/Channel.cpp \
        trendslibrary/IFile.cpp \
        trendslibrary/IFileSpecification.cpp \
        trendslibrary/ParamStat.cpp \
        trendslibrary/StorageWriter.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    Common.h \
    Cortege.h \
    trendslibrary/AlarmTracker.h \
    trendslibrary/Channel.h \
    trendslibrary/IFile.h \
    trendslibrary/IFileSpecification.h \
    trendslibrary/ParamStat.h \
    trendslibrary/StorageWriter.h

INCLUDEPATH += trendslibrary/ \

build_dir = $$_PRO_FILE_PWD_/build
DESTDIR = $$build_dir
PRECOMPILED_DIR = $$build_dir

#нужна для сборки внутренних библиотек qt, для решения конфликтов необходимо добавить qt в PATH
CONFIG(debug, debug|release): win32:QMAKE_POST_LINK += windeployqt.exe $$DESTDIR/Demo.exe

OBJECTS_DIR = $$build_dir/trash/obj
MOC_DIR     = $$build_dir/trash/moc
RCC_DIR     = $$build_dir/trash/rcc
UI_DIR      = $$build_dir/trash/ui


