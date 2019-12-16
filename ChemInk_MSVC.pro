QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11 console

DEFINES += QT_DEPRECATED_WARNINGS

#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

#！！！
# 修改QMAKE_CFLAGS设置编译类型：动态/静态
#！！！
QMAKE_CFLAGS_DEBUG += -MTd
#This variable contains the flags for the C compiler in debug mode
QMAKE_CFLAGS_RELEASE += -MT
#This variable contains the compiler flags for creating a non-debuggable application
QMAKE_CXXFLAGS_DEBUG += -MTd
#This variable contains the C++ compiler flags for creating a debuggable application
QMAKE_CXXFLAGS_RELEASE += -MT
#This variable contains the C++ compiler flags for creating an application

INCLUDEPATH += \
    ChemInk/third_party/eigen3.3.7 \
    ChemInk/third_party/frugally-deep/include \
    ChemInk/third_party/json/include \
    ChemInk/third_party/FunctionalPlus/include \
    ChemInk/third_party/opencv-4.1.1/include \
    ChemInk/third_party/opencv-4.1.1/include/opencv2 \
    ChemInk/third_party/openbabel-2.4.1/openbabel/include/openbabel-2.0
    

LIBS += \
    $${PWD}/../ChemInk/ChemInk/third_party/opencv-4.1.1/x64/vc16/staticlib/zlib.lib \
    $${PWD}/../ChemInk/ChemInk/third_party/opencv-4.1.1/x64/vc16/staticlib/quirc.lib \
    $${PWD}/../ChemInk/ChemInk/third_party/opencv-4.1.1/x64/vc16/staticlib/IlmImf.lib \
    $${PWD}/../ChemInk/ChemInk/third_party/opencv-4.1.1/x64/vc16/staticlib/ade.lib \
    $${PWD}/../ChemInk/ChemInk/third_party/opencv-4.1.1/x64/vc16/staticlib/ippicvmt.lib \
    $${PWD}/../ChemInk/ChemInk/third_party/opencv-4.1.1/x64/vc16/staticlib/ippiw.lib \
    $${PWD}/../ChemInk/ChemInk/third_party/opencv-4.1.1/x64/vc16/staticlib/ittnotify.lib \
    $${PWD}/../ChemInk/ChemInk/third_party/opencv-4.1.1/x64/vc16/staticlib/libjasper.lib \
    $${PWD}/../ChemInk/ChemInk/third_party/opencv-4.1.1/x64/vc16/staticlib/libjpeg-turbo.lib \
    $${PWD}/../ChemInk/ChemInk/third_party/opencv-4.1.1/x64/vc16/staticlib/libpng.lib \
    $${PWD}/../ChemInk/ChemInk/third_party/opencv-4.1.1/x64/vc16/staticlib/libprotobuf.lib \
    $${PWD}/../ChemInk/ChemInk/third_party/opencv-4.1.1/x64/vc16/staticlib/libtiff.lib \
    $${PWD}/../ChemInk/ChemInk/third_party/opencv-4.1.1/x64/vc16/staticlib/libwebp.lib \
    $${PWD}/../ChemInk/ChemInk/third_party/opencv-4.1.1/x64/vc16/staticlib/opencv_img_hash411.lib \
    $${PWD}/../ChemInk/ChemInk/third_party/opencv-4.1.1/x64/vc16/staticlib/opencv_world411.lib \
    $${PWD}/../ChemInk/ChemInk/third_party/openbabel-2.4.1/openbabel/lib/openbabel-2.lib

SOURCES += \
    ChemInk/src/alkanetopo.cpp \
    ChemInk/src/browser.cpp \
    ChemInk/src/chembalancer.cpp \
    ChemInk/src/classifier.cpp \
    ChemInk/src/fdeepcaller.cpp \
    ChemInk/src/forcefield.cpp \
    ChemInk/src/glumolwindow.cpp \
    ChemInk/src/main.cpp \
    ChemInk/src/molecule.cpp \
    ChemInk/src/objdetector.cpp \
    ChemInk/src/rawscript.cpp \
    ChemInk/src/scribblearea.cpp \
    ChemInk/src/spellcorrector.cpp \
    ChemInk/src/stdafx.cpp
    

HEADERS += \
    ChemInk/src/alkanetopo.h \
    ChemInk/src/browser.h \
    ChemInk/src/chembalancer.h \
    ChemInk/src/classifier.h \
    ChemInk/src/fdeepcaller.h \
    ChemInk/src/forcefield.h \
    ChemInk/src/glumolwindow.h \
    ChemInk/src/molecule.h \
    ChemInk/src/objdetector.h \
    ChemInk/src/rawscript.h \
    ChemInk/src/scribblearea.h \
    ChemInk/src/spellcorrector.h \
    ChemInk/src/stdafx.h \
    

FORMS += 

RESOURCES += \
    ChemInk/chemink.qrc

RC_FILE += \
    ChemInk/ChemInk.rc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

# ！！！
# 链接后数据的拷贝事件，内容需要和生成路径保持一致，可执行文件和运行时数据在CONFIG中设置
# ！！！
QMAKE_POST_LINK += cd $${PWD} && xcopy /s /e /y /d %cd%\..\ChemInk\ChemInk\data %cd%\release\data\ && @echo fuck

# 指定生成路径，由qtcreator自动生成，上面的xcopy按照默认配置编写
#CONFIG(debug, debug|release) {
#    DESTDIR = xxx
#} else {
#    DESTDIR = xxx
#}
