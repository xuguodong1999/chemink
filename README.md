# ChemInk 化学公式的手写分析与应用系统

## 原理简述

1. 用Yolov3-tiny目标检测模型做字符切割，在很小的程度上支持连笔

2. 用DenseNet结构的卷积网络对检测框中的字符进行二次分类

3. 用字典按位加权的方法从语言层面进一步筛选合理结果

4. 用判断无根树同构的方法生成饱和烷烃的所有非光学同分异构体

5. 用化合价表设计了相对完备的方程式配平方法

6. 用旋转基本图元的方法进行简单的球棍模型渲染

7. 由opencv推理目标检测网络、fdeep推理单字符卷积网络，由glu提供三维基本图元，由Qt提供图形框架

## 阶段性完结【演示】

![结果演示](./ChemInk/结题考核演示.gif)

## 关于这个仓库

1. 包含全部源码

2. 包含densenet和yolov3-tiny的猴版权重文件（就是没有达到SOTA，比较low，要求不能写得很奔放）

3. 不包含第三方库，请见[第三方库编译说明](./ChemInk/third_party/README.md)

4. 不包含数据集和训练脚本

## 一些备注

1. 编码转换：VisualStudio要求国标码，类unix平台要求UTF-8编码，建议转换方法：\
enconv -L zh_CN -x UTF-8/gb2312 ./ChemInk/src/\*.cpp \
enconv -L zh_CN -x UTF-8/gb2312 ./ChemInk/src/\*.h

2. 图形方面涉及glu，macOS需要额外安装freeglut，并且把涉及到的<GL/glu.h>改为<GL/freeglut.h>

3. 代码目前无法向android平台编译，没有解决OpenBabel和Glu的依赖问题

## 一些链接

[第三方库](./ChemInk/third_party/README.md)

[2019年3月的完整代码](./RectBasedDemo/README.md)

