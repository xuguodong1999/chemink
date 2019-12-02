#include <map>
#include <list>
#include <tuple>
#include <string>
#include <vector>
#include <thread>

#include <cmath>
#include <cstdio>
#include <climits>

#include <sstream>
#include <iostream>
#include <memory>
#include <algorithm>

/*
对要使用cout的文件，要用 #pragma execution_character_set("GB2312") 解除utf-8编码
*/
#pragma execution_character_set("UTF-8")

/*  链接策略说明：
库目录通过VS的GUI或者直接编辑vcxproj来设置
要链接的静态库在stdafx.cpp文件用#pragma comment(lib,"xxx.lib")链接
*/

/*第三方库头文件包含策略：
通过VS的GUI或者直接编辑vcxproj来设置
<IncludePath>
$(SolutionDir)ChemInk/third_party/eigen3.3.7;$(SolutionDir)ChemInk/third_party/json/include;$(SolutionDir)ChemInk/third_party/FunctionalPlus/include;$(SolutionDir)ChemInk/third_party/frugally-deep/include;$(SolutionDir)ChemInk/third_party/opencv-4.1.1/include;$(SolutionDir)ChemInk/third_party/opencv-4.1.1/include/opencv2;$(SolutionDir)ChemInk/third_party/openbabel-2.4.1/openbabel/include/openbabel-2.0;
</IncludePath>
*/

//  包含Eigen3头文件全家桶
#define INCLUDE_EIGEN3

//  包含QT5头文件全家桶
#define INCLUDE_QT5

//  链接openbabel-2.4.1静态库
#define LINK_BABEL2

//  链接opencv4.1.1静态库
#define LINK_CV411

//  链接Qt5静态库
//#define LINK_STATIC_QT5

//  申请安卓权限
//  #define ANDROID_BUILDBY_WIN_QMAKE

#ifdef INCLUDE_EIGEN3
//  包含一下Eigen全家桶
#include <Eigen/Cholesky>
//#include <Eigen/QR>
//#include <Eigen/QtAlignedMalloc>
#include <Eigen/Sparse>
#include <Eigen/Core>
//#include <Eigen/SparseCholesky>
#include <Eigen/Dense>
//#include <Eigen/SparseCore>
//#include <Eigen/Eigen>
//#include <Eigen/SparseLU>
//#include <Eigen/Eigenvalues>
//#include <Eigen/SparseQR>
//#include <Eigen/Geometry>
//#include <Eigen/Householder>
//#include <Eigen/StdDeque>
//#include <Eigen/IterativeLinearSolvers>
#include <Eigen/StdList>
//#include <Eigen/Jacobi>
#include <Eigen/StdVector>
//#include <Eigen/LU>
//#include <Eigen/SVD>
//#include <Eigen/OrderingMethods>

//  下面的头文件没有必要包含
//#include <Eigen/PaStiXSupport>
//#include <Eigen/CholmodSupport>
//#include <Eigen/SPQRSupport>
//#include <Eigen/SuperLUSupport>
//#include <Eigen/MetisSupport>
//#include <Eigen/PardisoSupport>
#endif // INCLUDE_EIGEN3

#ifdef INCLUDE_QT5
//  非移动端允许使用gl库
#ifndef ANDROID_BUILDBY_WIN_QMAKE
//  不要直接包含windows.h
//  里面有个min的宏定义
//  会让std::numeric<?>::min()失效
#define WINGDIAPI __declspec(dllimport)
#define APIENTRY _stdcall
#define CALLBACK _stdcall
//  #include <Windows.h>
//#include <gl/GL.h>
#include <gl/GLU.h>
#include <QGLWidget>
#endif // !ANDROID_BUILDBY_WIN_QMAKE

//  包含一下Qt头文件
#include <QStyleFactory>
#include <QVector>
#include <QString>
#include <QList>
#include <QHash>
#include <QMap>

#include <QSize>
#include <QPointer>
#include <QDebug>
#include <QColor>
#include <QPainter>
#include <QMessageBox>
#include <QErrorMessage>

#include <QProgressBar>
#include <QEvent>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QResizeEvent>
#include <QPaintEvent>
#include <QGraphicsSceneMouseEvent>

#include <QOpenGLWindow>
#include <QMatrix4x4>
#include <QVector3D>
#include <QImage>
#include <QOpenGLTexture>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLContext>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLExtraFunctions>
#include <QPropertyAnimation>
#include <QPauseAnimation>
#include <QSurfaceFormat>
#include <QSequentialAnimationGroup>
#include <QTimer>
#include <QStyle>
#include <QIcon>
#include <QToolButton>
#include <QLabel>
#include <QMenu>
#include <QStatusBar>
#include <QMenuBar>
#include <QToolBar>
#include <QAction>
#include <QFileDialog>
#include <QDialog>
#include <QMdiSubWindow>

#include <QScrollArea>
#include <QWidget>
#include <QDockWidget>
#include <QMdiArea>
#include <QGraphicsItem>
#include <QGraphicsPathItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMainWindow>
#include <QTranslator>
#include <QApplication>
#include <QDesktopWidget>

#include <QLayout>
#include <QGridLayout>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>

#endif // INCLUDE_QT5

#ifdef ANDROID_BUILDBY_WIN_QMAKE
bool requestPermission(QString moduleName);
#endif // ANDROID_BUILDBY_WIN_QMAKE

int throwErrorBox(const char* windowTitle, const char* message);

QSize getDesktopSize();

int loadBabelPlugin();
