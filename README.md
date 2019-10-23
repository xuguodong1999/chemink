# ChemInk

## still under development

## current state

1. 已经完成（的组件）：基于字典按位严格匹配的输入纠错，SMILES转球棍模型，目标检测模型，支持随机笔画擦除的画板，饱和烷烃的所有非光学同分异构体的拓扑生成，单字符卷积分类器，化学方程式配平。

2. （遥遥无期的）改进：输入纠错，尝试语言模型（字典纠错用在ppt软件上是够了的）

3. TODO: 设计对笔迹数据做多次目标检测的交叉验证逻辑，设计用户交互界面，设计交互逻辑，线程管理

4. BUG TO FIX: 在vs-v142-release编译器下，cv::dnn::blobFromImage输入参数cv::Size不支持变化？

## something

1. 编码转换：VisualStudio要求国标码，类unix平台要求UTF-8编码，建议转换方法：\
enconv -L zh_CN -x UTF-8/gb2312 ./ChemInk/src/\*.cpp \
enconv -L zh_CN -x UTF-8/gb2312 ./ChemInk/src/\*.h

2. 图形方面涉及glu，macOS需要额外安装freeglut，并且把涉及到的<GL/glu.h>改为<GL/freeglut.h>

3. 因为*2*，代码目前无法向android平台编译，NDK用的是GLES

## useful link

[第三方库](./ChemInk/third_party/README.md)

[2019年3月的完整代码](./version-2019-3-2.src/README.md)

[数据集生成](https://github.com/Xuguodong1999/chds-generator/blob/master/README.md)
