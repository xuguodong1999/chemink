QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += \
    c++11 \
    c++14 \
#    console

QMAKE_CXXFLAGS_RELEASE += -O3

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    src/main.cpp \
    src/opencvyolov3.cpp \
    src/writtingpanel.cpp \
    src/alkanetopo.cpp \
    src/chembalancer.cpp \
    src/detectionobject.cpp \
    src/fdeepcnn.cpp \
    src/forcefieldcaller.cpp \
    src/glcheminfo.cpp \
    src/glmolarea.cpp \
    src/scribblearea.cpp \
    src/threadsupport.cpp

HEADERS += \
    src/opencvyolov3.h \
    src/writtingpanel.h \
    src/alkanetopo.h  \
    src/chembalancer.h  \
    src/detectionobject.h  \
    src/fdeepcnn.h  \
    src/forcefieldcaller.h  \
    src/glcheminfo.h  \
    src/glmolarea.h  \
    src/scribblearea.h  \
    src/threadsupport.h

FORMS += \
    src/writtingpanel.ui

INCLUDEPATH += \
    /usr/local/Cellar/freeglut/3.0.0/include \
    third_party/eigen3.3.7 \
    third_party/json/include \
    third_party/frugally-deep/include \
    third_party/FunctionalPlus/include \
    third_party/opencv4.1.1/include/opencv4 \
    third_party/opencv4.1.1/include/opencv4/opencv2 \
    third_party/openbabel3.0.0a2/include/openbabel3

LIBS += \
    /Users/xgd/code/ChemInk/third_party/opencv4.1.1/lib/opencv4/3rdparty/lib*.a \
    /Users/xgd/code/ChemInk/third_party/opencv4.1.1/lib/opencv4/3rdparty/libippiw.a \
    /Users/xgd/code/ChemInk/third_party/opencv4.1.1/lib/opencv4/3rdparty/libippicv.a \
    /Users/xgd/code/ChemInk/third_party/openbabel3.0.0a2/lib/libopenbabel.a \
    /Users/xgd/code/ChemInk/third_party/opencv4.1.1/lib/libopencv_*.a \
    /usr/local/Cellar/freeglut/3.0.0/lib/*.a \
    /usr/lib/liblapack.dylib \
    /usr/lib/libcblas.dylib \
    -ldl \
    -lpthread \
#    /Users/xgd/code/ChemInk/third_party/opencv4.1.1/lib/opencv4/3rdparty/liblibtiff.a \
#    -lapack


RC_ICONS = ChemInk.ico

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
