#pragma once

#include "molecule.h"
#include <QMouseEvent>
#include <QWheelEvent>
#include <QGLWidget>
#include <QPoint>
#include <Eigen/Core>

class GLUquadric;
//  渲染一个模型需要的数据
//  为不同封装程度的三维图形库设计专门的数据结构
class GluMolecule {
public:
	GluMolecule(const Molecule& molecule);
	//  球心坐标、球半径、球面颜色
	std::vector<std::tuple<Eigen::Vector3d, double, float*>> nodes;
	Eigen::Vector3d& getSphereCenter(int i);
	double& getSphereRadius(int i);
	float*& getSphereColor(int i);
	//  圆柱指向和默认指向的法向量，旋转角度，长度，出发原子编号
	//  为什么要方向？调用的glu三维图元存在默认方向——Z轴正方向
	//  因为用了glu的默认图元所以效率很低所以以空间换时间
	std::vector<std::tuple<Eigen::Vector3d, double, double, int>> links;
	Eigen::Vector3d& getCylinderDirection(int i);
	double& getCylinderRotateAngle(int i);
	double& getCylinderLength(int i);
	int& getCylinderAtom(int i);
};
//  只负责渲染，交互由上层的view处理，视角变换在view里面变化
class GluMolWindow :public QGLWidget {
	Q_OBJECT
public:
	GluMolWindow(QWidget* parent = nullptr);
	~GluMolWindow()override;
	void createMol3d(const Molecule& _molecule);
private:
	GLUquadric* quadric;
	GluMolecule* molecule;
	double rX, rY, dZ;
	QPoint lastPoint;
	void zoomIn();
	void zoomOut();
protected:
	void mousePressEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;
	//void keyPressEvent(QKeyEvent* event)override;
	void wheelEvent(QWheelEvent* event)override;
	//void closeEvent(QCloseEvent* event)override;
	void initializeGL()override;
	void paintGL()override;
	void resizeGL(int width, int height)override;
};
