QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += \
    c++11 \
    c++14 \
#    console

QMAKE_CXXFLAGS_RELEASE += -O3

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    ../src/main.cpp \
    ../src/opencvyolov3.cpp \
    ../src/writtingpanel.cpp \
    ../src/alkanetopo.cpp \
    ../src/chembalancer.cpp \
    ../src/detectionobject.cpp \
    ../src/fdeepcnn.cpp \
#    ../src/forcefieldcaller.cpp \
    ../src/glcheminfo.cpp \
#    ../src/glmolarea.cpp \
    ../src/scribblearea.cpp \
    ../src/threadsupport.cpp

HEADERS += \
    ../src/opencvyolov3.h \
    ../src/writtingpanel.h \
    ../src/alkanetopo.h  \
    ../src/chembalancer.h  \
    ../src/detectionobject.h  \
    ../src/fdeepcnn.h  \
#    ../src/forcefieldcaller.h  \
    ../src/glcheminfo.h  \
#    ../src/glmolarea.h  \
    ../src/scribblearea.h  \
    ../src/threadsupport.h

FORMS += \
    ../src/writtingpanel.ui

INCLUDEPATH += \
    ../third_party/eigen3.3.7 \
    ../third_party/json/include \
    ../third_party/frugally-deep/include \
    ../third_party/FunctionalPlus/include \
    ../third_party/opencv4.1.1/include/opencv2 \
    ../third_party/opencv4.1.1/include

LIBS += \
    libopencv_java4.so

RESOURCES += \
    ../ChemInk.qrc \
    ../weights(1).qrc \
    ../weights(2).qrc \
    ../weights(3).qrc \
    ../weights(4).qrc

RC_ICONS = ChemInk.ico

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

contains(ANDROID_TARGET_ARCH,armeabi-v7a) {
    ANDROID_EXTRA_LIBS = \
        $$PWD/libopencv_java4.so
}
