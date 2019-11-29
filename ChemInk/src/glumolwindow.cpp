#include "stdafx.h"
#include "glumolwindow.h"
using namespace std;
using namespace Eigen;

static double angleVec3d(const Vector3d& v1, const Vector3d& v2) {
	// a·b=|a|*|b|*cos(angle)
	const float PI = acos(-1.0);
	return acos(v1.dot(v2) / (v1.norm() * v2.norm())) * 180 / PI;
}
const Vector3d zAxis(0, 0, 1);
const Vector3d yAxis(0, 1, 0);
const Vector3d xAxis(1, 0, 0);
const int slice = 100;
const double cylinderRadius = 0.07;
Vector3d& GluMolecule::getSphereCenter(int i) {
	return std::get<0>(nodes.at(i));
}
double& GluMolecule::getSphereRadius(int i) {
	return std::get<1>(nodes.at(i));
}
float*& GluMolecule::getSphereColor(int i) {
	return std::get<2>(nodes.at(i));
}
Vector3d& GluMolecule::getCylinderDirection(int i) {
	return std::get<0>(links.at(i));
}
double& GluMolecule::getCylinderRotateAngle(int i) {
	return std::get<1>(links.at(i));
}
double& GluMolecule::getCylinderLength(int i) {
	return std::get<2>(links.at(i));
}
int& GluMolecule::getCylinderAtom(int i) {
	return std::get<3>(links.at(i));
}
GluMolecule::GluMolecule(const Molecule& molecule) {
	size_t atomCounter(molecule.getAtomSize());
	nodes.resize(atomCounter);
	Vector3d sum(0, 0, 0);
	for (size_t i = 0; i < atomCounter; i++) {
		const Atom& atom = molecule.getAtom(i);
		getSphereCenter(i) = atom.center;
		sum += atom.center;
		getSphereRadius(i) = atom.covalentRadius;
		getSphereColor(i) = atom.colorVec;
	}
	sum /= atomCounter;
	for (size_t i = 0; i < atomCounter; i++) {
		getSphereCenter(i) -= sum;
	}

	int bondSize(molecule.getBondSize()), bondCounter(0);
	for (size_t i = 0; i < bondSize; i++) {
		bondCounter += molecule.getBond(i).level;
	}
	bondCounter *= 2;//  从起始位置出发渲染半根圆柱
	links.resize(bondCounter);
	size_t index = 0;
	for (size_t i = 0; i < bondSize; i++) {
		const Bond& bond = molecule.getBond(i);
		const Atom& from = molecule.getAtom(bond.aids.at(0));
		const Atom& to = molecule.getAtom(bond.aids.at(1));
		//   分两步构建圆柱
		Vector3d centerAxis = to.center - from.center;
		double k = from.covalentRadius / (from.covalentRadius + to.covalentRadius);
		double len = centerAxis.norm();
		getCylinderDirection(index) = zAxis.cross(centerAxis);// centerAxis.cross(zAxis);
		getCylinderRotateAngle(index) = angleVec3d(zAxis, centerAxis);
		getCylinderLength(index) = len * k;
		getCylinderAtom(index) = bond.aids.at(0);
		index++;
		//  to --> from
		centerAxis = -centerAxis;
		k = 1.0 - k;
		getCylinderDirection(index) = zAxis.cross(centerAxis); //centerAxis.cross(zAxis);
		getCylinderRotateAngle(index) = angleVec3d(zAxis, centerAxis);
		getCylinderLength(index) = len * k;
		getCylinderAtom(index) = bond.aids.at(1);
		index++;
		//  TODO: 处理多重圆柱、弯曲圆柱的问题
		//switch ((int)bond.level) {
		//case 1:
		//	std::get<0>(links.at(index)) = centerAxis.cross(zAxis);
		//	std::get<1>(links.at(index)) = centerAxis;
		//	std::get<2>(links.at(index)) = centerAxis.norm() * k;
		//	std::get<3>(links.at(index++)) = cylinderRadius;
		//	centerAxis = -centerAxis;
		//	k = 1.0 - k;
		//	std::get<0>(links.at(index)) = centerAxis.cross(zAxis);
		//	std::get<1>(links.at(index)) = centerAxis;
		//	std::get<2>(links.at(index)) = centerAxis.norm() * k;
		//	std::get<3>(links.at(index++)) = cylinderRadius;
		//	break;
		//case 2:
		//	//  平移1

		//	//  平移2

		//	break;
		//case 3:
		//	break;
		//case 4:
		//default:
		//	break;
		//}
	}

}

//void GluMolWindow::mousePressEvent(QMouseEvent* event) {
//}
//void GluMolWindow::mouseMoveEvent(QMouseEvent* event) {
//}
//void GluMolWindow::mouseReleaseEvent(QMouseEvent* event) {
//}
//void GluMolWindow::keyPressEvent(QKeyEvent* event) {
//}
void GluMolWindow::wheelEvent(QWheelEvent* event) {
	if (QApplication::keyboardModifiers() != Qt::ControlModifier)
		return;
	if (event->delta() < 0) {
		zoomIn();
	}
	else {
		zoomOut();
	}
	glLoadIdentity();
	gluLookAt(
		0, 0, -dZ,
		0, 0, 0,
		1.0, 0.0, 0.0
	);
	update();
}
//void GluMolWindow::closeEvent(QCloseEvent* event) {
//}

GluMolWindow::GluMolWindow(QWidget* parent)
	:QGLWidget(parent), molecule(nullptr), quadric(gluNewQuadric()), rX(0), rY(0), dZ(20.0)
{

}
GluMolWindow::~GluMolWindow() {
	if (molecule != nullptr) {
		delete molecule;
		molecule = nullptr;
	}
}
void GluMolWindow::zoomIn()
{
	dZ++;
}
void GluMolWindow::zoomOut()
{
	dZ--;
}

void GluMolWindow::mousePressEvent(QMouseEvent* event) {
	setCursor(Qt::ClosedHandCursor);
	lastPoint = event->pos();
	return QGLWidget::mousePressEvent(event);
}
void GluMolWindow::mouseMoveEvent(QMouseEvent* event) {
	QPoint dp = event->pos() - lastPoint;
	//cout << "(" << rX << "," << rY << ")-->";
	rX += dp.x();
	rY += dp.y();
	//cout << "(" << rX << "," << rY << endl;
	lastPoint = event->pos();
	updateGL();
	return QGLWidget::mouseMoveEvent(event);
}
void GluMolWindow::mouseReleaseEvent(QMouseEvent* event) {
	setCursor(Qt::OpenHandCursor);
	return QGLWidget::mouseReleaseEvent(event);
}

void  GluMolWindow::createMol3d(const Molecule& _molecule) {
	if (molecule != nullptr) {
		delete molecule;
	}
	molecule = new GluMolecule(_molecule);
	updateGL();
}
void GluMolWindow::initializeGL() {

	//材质反光性设置
	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };  //镜面反射参数
	GLfloat mat_shininess[] = { 50.0 };               //高光指数
	GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };
	GLfloat white_light[] = { 1.0, 1.0, 1.0, 1.0 };   //灯位置(1,1,1), 最后1-开关
	GLfloat Light_Model_Ambient[] = { 0.2, 0.2, 0.2, 1.0 }; //环境光参数

	glClearColor(0.0, 0.0, 0.0, 0.0);  //背景色
	glShadeModel(GL_SMOOTH);           //多变性填充模式

	//材质属性
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

	//灯光设置
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, white_light);   //散射光属性
	glLightfv(GL_LIGHT0, GL_SPECULAR, white_light);  //镜面反射光
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, Light_Model_Ambient);  //环境光参数
	//  glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 10000000.0);//  光线衰减

	glEnable(GL_LIGHTING);   //开关:使用光
	glEnable(GL_LIGHT0);     //打开0#灯
	glEnable(GL_DEPTH_TEST); //打开深度测试

	glLoadIdentity();
	gluLookAt(
		0, 0, -dZ,
		0, 0, 0,
		1.0, 0.0, 0.0
	);

}
void GluMolWindow::paintGL() {
	if (molecule == nullptr)
		return;
	//qDebug() << "glu start paint" << endl;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	//if (rY > 360)rY -= 360;
	//else if (rY < 0)rY += 360;
	//if(rY<180)
	//	glRotatef(rX, xAxis[0], xAxis[1], xAxis[2]);
	//else
	//	glRotatef(-rX, xAxis[0], xAxis[1], xAxis[2]);
	glRotatef(rY, yAxis[0], yAxis[1], yAxis[2]);
	glRotatef(rX, xAxis[0], xAxis[1], xAxis[2]);
	//glBegin(GL_LINE_STRIP);
	//glColor3f(1.0, 0, 0);
	//glVertex3i(0, 0, 0);
	//glVertex3i(2, 0, 0);
	//glEnd();
	//glBegin(GL_LINE_STRIP);
	//glColor3f(1.0, 0.0, 0);
	//glVertex3i(0, 0, 0);
	//glVertex3i(0, 4, 0);
	//glEnd();
	//glBegin(GL_LINE_STRIP);
	//glColor3f(1, 0, 0.0);
	//glVertex3i(0, 0, 0);
	//glVertex3i(0, 0, 8);
	//glEnd();
	//  渲染的时候所有图元的位置、大小都已确定
	for (size_t i = 0; i < molecule->nodes.size(); i++) {
		glPushMatrix();
		Vector3d& center = molecule->getSphereCenter(i);
		glTranslatef(center[0], center[1], center[2]);
		//qDebug() << molecule->getSphereColor(i)[0] << "," << molecule->getSphereColor(i)[1]
		//	<< "," << molecule->getSphereColor(i)[2] << endl;
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, molecule->getSphereColor(i));
		//qDebug() << "molecule->getSphereRadius(i)/300.0+0.14" << molecule->getSphereRadius(i) / 300.0 + 0.14;
		gluSphere(quadric, molecule->getSphereRadius(i) / 300.0 + 0.14, slice, slice);
		glPopMatrix();
	}
	for (size_t i = 0; i < molecule->links.size(); i++) {
		Vector3d& center = molecule->getSphereCenter(molecule->getCylinderAtom(i));
		glPushMatrix();
		glTranslatef(center[0], center[1], center[2]);
		Vector3d& norm = molecule->getCylinderDirection(i);
		glRotatef(molecule->getCylinderRotateAngle(i), norm[0], norm[1], norm[2]);
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, molecule->getSphereColor(molecule->getCylinderAtom(i)));
		//qDebug() << "molecule->getCylinderLength(i)=" << molecule->getCylinderLength(i);
		gluCylinder(quadric, cylinderRadius, cylinderRadius, molecule->getCylinderLength(i), slice, slice);
		glPopMatrix();
	}
	glPopMatrix();
	//qDebug() << "glu end paint" << endl;
}
void GluMolWindow::resizeGL(int width, int height) {
	double side = (std::min)(width, height);
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-width / side, width / side, -height / side, height / side, 5.0, 200.0);
	glMatrixMode(GL_MODELVIEW);
	//return QGLWidget::resizeGL(width, height);
}