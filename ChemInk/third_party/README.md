# ChemInk依赖的第三方c++库

## 以头文件形式使用的库

1. FrugallyDeep， 深度学习推理库，用于部署keras模型，在本项目中提供DenseNet的推理任务，用于单字符的识别，\
代码仓库：https://github.com/Dobiasd/frugally-deep

2. Eigen3， FrugallyDeep依赖这个库，另外，本项目使用了Eigen提供的部分数学函数\
项目网站：https://www.eigen.tuxfamily.org

3. json， FrugallyDeep依赖这个库\
代码仓库：https://github.com/nlohmann/json

4. FunctionalPlus， FrugallyDeep依赖这个库\
代码仓库：https://github.com/Dobiasd/FunctionalPlus

## 需要编译的库

1. OpenBabel， 化学工具库，在本项目里提供分子力场，用于生成能量最小化的分子结构，\
代码仓库：https://github.com/openbabel/openbabel

2. OpenCV>=4.1.1， 计算机视觉库，在本项目里提供YoloV3-tiny目标检测模型的推理，\
代码仓库-opencv：https://github.com/opencv/opencv\
代码仓库-opencv-contrib：https://github.com/opencv/opencv_contrib

3. Qt>=5.12， c++ GUI 框架，在本项目里，提供所有前端界面，提供图像、音视频处理功能，\
代码仓库：http://download.qt.io

## MKLINK 脚本

[mklink_3rdparty.bat](./mklink_3rdparty.bat)

1. 编辑mklink_3rdparty.bat文件，修改"C:\\3rdlib\\"为你的库目录前缀

2. 以管理员身份在third_party下执行mklink_3rdparty.bat

3. third_party已经包含以头文件形式使用的库，需额外导入opencv和openbabel

## 目录组织

[Structure.md](./Structure.md)