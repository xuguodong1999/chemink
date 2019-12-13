/*  链接策略说明：
库目录通过VS的GUI或者直接编辑vcxproj来设置
要链接的静态库在stdafx.cpp文件用#pragma comment(lib,"xxx.lib")链接
*/

//  链接openbabel-2.4.1静态库
#define LINK_BABEL2

//  链接opencv4.1.1静态库
#define LINK_CV411

//  链接Qt5静态库
//  #define LINK_STATIC_QT5

#include <QSize>

int throwErrorBox(const char* windowTitle, const char* message);

QSize getDesktopSize();

int loadBabelPlugin();
