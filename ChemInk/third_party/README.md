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
代码仓库：https://github.com/openbabel/openbabel\
最低编译要求：跑通obgen模块

2. OpenCV， 计算机视觉库，在本项目里提供YoloV3-tiny目标检测模型的推理，\
代码仓库-opencv：https://github.com/opencv/opencv\
代码仓库-opencv-contrib：https://github.com/opencv/opencv_contrib\
最低编译要求：和 OpenCV Contrib 联合编译，跑通cv::dnn模块

3. Qt-5.13.1， c++ GUI 框架，在本项目里，提供所有前端界面，提供图像、音视频处理功能，\
代码仓库：http://download.qt.io\
建议编译方式：进行静态编译\
代替方案：使用官网的预编译的动态链接库

## 预编译的库（可选，是某些功能的冗余/更优实现）

1. OpenVINO， 英特尔的深度学习库，曾在本项目里提供YoloV3-tiny目标检测模型的推理，仅支持intel-cpu，intel-gpu，intel-ncs2等计算设备，其中，在使用Myriad神经计算棒的情况下，几乎不消耗本机的算力，程序的CPU占用接近普通画板的水平
