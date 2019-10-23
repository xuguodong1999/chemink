#include "glmolarea.h"
#include <QKeyEvent>
#include <QMouseEvent>
#include <QWheelEvent>
#include <iostream>
#include <iomanip>
#include <QDebug>
using namespace std;

GlMolArea::GlMolArea(const char* data, molformat f, QWidget* parent) {
	if (parent != nullptr)
		setParent(parent);
	setAttribute(Qt::WA_AcceptTouchEvents, true);	//  接受触摸事件
	//  setWindowFlags(Qt::FramelessWindowHint);		//  无边框样式

	depthZ = -40.0f;
	mol = nullptr;
	style = CPK;
	rotateX = rotateY = rotateZ = 0;
	setCursor(Qt::OpenHandCursor);
    quadric = gluNewQuadric();

	setMolecule(data, f);
	cout << "分子图的边数：" << gra.size() << endl;
}

GlMolArea::~GlMolArea() { ; }

void GlMolArea::setMolecule(const char* data, molformat f) {
	if (mol != nullptr)
		delete mol;
	mol = new GlMol();
	switch (f) {
	case PDB:
		mol->loadPdb(data);
		break;
	case MOL2:
		mol->loadMol2(data);
	default:
		break;
	}
	gra.clear();
	float midX, midY, midZ;	//  分子的几何中心
	float xmin, xmax, ymin, ymax, zmin, zmax;
	xmin = ymin = zmin = std::numeric_limits<int>::max();
	xmax = ymax = zmax = std::numeric_limits<int>::min();
	size_t n = mol->getAtomNum();
	for (int i = 0; i < n; i++) {
		GlAtom& atom = mol->atom(i);
		xmin = atom.x() < xmin ? atom.x() : xmin;
		ymin = atom.y() < ymin ? atom.y() : ymin;
		zmin = atom.z() < zmin ? atom.z() : zmin;
		xmax = atom.x() > xmax ? atom.x() : xmax;
		ymax = atom.y() > ymax ? atom.y() : ymax;
		zmax = atom.z() > zmax ? atom.z() : zmax;
	}
	midX = (xmin + xmax) / 2; midY = (ymin + ymax) / 2; midZ = (zmin + zmax) / 2;
	for (int i = 0; i < n; i++) {
		GlAtom& atom = mol->atom(i);
		atom.x() -= midX;
		atom.y() -= midY;
		atom.z() -= midZ;
	}
	n = mol->getBondNum();
	GlBondLink b1, b2;
	for (int i = 0; i < n; i++) {
		GlBond& bond = mol->bond(i);
		cout << bond.bondLevel << endl;
		b1.atomId = bond.a;
		b2.atomId = bond.b;
		b1.stickCounter = b2.stickCounter = bond.bondLevel;
		float div = 1.0 * mol->atom(bond.a).covalentRadius / mol->atom(bond.b).covalentRadius;
		float b1b2 = GlPoint3D::getDistance(mol->atom(bond.a), mol->atom(bond.b));
		b1.height = b1b2 * div / (1 + div);
		b2.height = b1b2 - b1.height;

		b1.angle = GlPoint3D::getRotationAngle(mol->atom(bond.a), mol->atom(bond.b), b1.rotVec);
		b2.angle = GlPoint3D::getRotationAngle(mol->atom(bond.b), mol->atom(bond.a), b2.rotVec);

		gra.push_back(b1); gra.push_back(b2);
	}

	updateGL();
}

const float GlMolArea::cpkTable[16][4] = {	// R G B A
	{1.00000, 1.00000, 1.00000, 1.00000},	// White 
	{1.00000, 0.75294, 0.79608, 1.00000},	// Pink 
	{0.85490, 0.64706, 0.12549, 1.00000},	// Golden Rod
	{0.56078, 0.56078, 1.00000, 1.00000},	// Sky Blue 
	{0.94118, 0.00000, 0.00000, 1.00000},	// Red 
	{1.00000, 0.78431, 0.00000, 1.00000},	// Yellow 
	{0.62745, 0.12549, 0.94118, 1.00000},	// Purple
	{1.00000, 0.64706, 0.00000, 1.00000},	// Orange 
	{0.00000, 0.00000, 1.00000, 1.00000},	// Blue 
	{0.50196, 0.50196, 0.56471, 1.00000},	// Dark Grey 
	{0.64706, 0.16471, 0.16471, 1.00000},	// Brown 
	{1.00000, 0.07843, 0.57647, 1.00000},	// Deep Pink 
	{0.00000, 1.00000, 0.00000, 1.00000},	// Green 
	{0.69804, 0.13333, 0.13333, 1.00000},	// Fire Brick 
	{0.13333, 0.54510, 0.13333, 1.00000},	// Forest Green 
	{0.78431, 0.78431, 0.78431, 1.00000}	// Light Grey
};

const float GlMolArea::depthStep = 5;

void GlMolArea::initializeGL() {
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
	//  glTranslatef(0.0f, 0.0f, 0.0f);
	gluLookAt(0, 0, depthZ, 0, 0, 0, 1.0, 0.0, 0.0);
}

void GlMolArea::paintGL() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if (mol == nullptr)
		return;
	glPushMatrix();
	glRotatef(rotateY, 0.0, 1.0, 0.0);
	glRotatef(rotateX, 1.0, 0.0, 0.0);
	if (style != LINES) {
		renderAtomBall(style);
		if (style != PROP) {
			renderBondTube();
		}
	}
	else {
		renderBondLine();
	}
	glPopMatrix();
}

void GlMolArea::resizeGL(int width, int height) {
	double side = std::min(width, height);
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-width / side, width / side, -height / side, height / side, 5.0, 200.0);
	glMatrixMode(GL_MODELVIEW);
}

void GlMolArea::renderAtomBall(mode _mode) {
	size_t n = mol->getAtomNum();
	//cout << midX << "," << midY << "." << midZ << endl;
	//glTranslatef(midX, midY, midZ);
	for (size_t i = 0; i < n; i++) {
		GlAtom& atom = mol->atom(i);
		float radius = atom.covalentRadius;
		switch (_mode) {
		case CPK:
			radius /= 300.0;
			radius += 0.14;
			break;
		case LINES:
			break;
		case PROP:
			radius /= 10.0;
			break;
		default:
			cout << "不支持此类模型，mode = " << _mode << endl;
		}
		glPushMatrix();
		glTranslatef(atom.x(), atom.y(), atom.z());
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, cpkTable[atom.atomicNumber]);
		gluSphere(quadric, radius, 100, 100);
		glPopMatrix();
	}
}

void GlMolArea::renderBondTube() {
	size_t n = gra.size();
	GLfloat glf1 = 0.1, glf2 = 0.05, glf3;
	for (size_t i = 0; i < n; i++) {
		GlBondLink& bondLink = gra.at(i);
		GlAtom& atom = mol->atom(bondLink.atomId);
		if (bondLink.stickCounter % 2 == 0) {
			for (int j = 0; j < bondLink.stickCounter; j++) {
				glPushMatrix();
				glf3 = glf2 - (j % 2) * glf1;
				glTranslatef(atom.x() + glf3, atom.y() + glf3, atom.z() + glf3);
				glRotatef(bondLink.angle, bondLink.rotVec[0], bondLink.rotVec[1], bondLink.rotVec[2]);
				glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, cpkTable[atom.atomicNumber]);
				gluCylinder(quadric, 0.07, 0.07, bondLink.height, 100, 100);
				glPopMatrix();
				glf1 *= 1.1; glf2 *= 1.1;
			}
			glf1 = 0.1; glf2 = 0.05;
		}
		else {
			glPushMatrix();
			glTranslatef(atom.x(), atom.y(), atom.z());
			glRotatef(bondLink.angle, bondLink.rotVec[0], bondLink.rotVec[1], bondLink.rotVec[2]);
			glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, cpkTable[atom.atomicNumber]);
			gluCylinder(quadric, 0.07, 0.07, bondLink.height, 100, 100);
			glPopMatrix();
			for (int j = 0; j < bondLink.stickCounter - 1; j++) {
				glPushMatrix();
				glf3 = glf2 - (j % 2) * glf1;
				glTranslatef(atom.x() + glf3, atom.y() + glf3, atom.z() + glf3);
				glRotatef(bondLink.angle, bondLink.rotVec[0], bondLink.rotVec[1], bondLink.rotVec[2]);
				glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, cpkTable[atom.atomicNumber]);
				gluCylinder(quadric, 0.07, 0.07, bondLink.height, 100, 100);
				glPopMatrix();
				glf1 *= 1.1; glf2 *= 1.1;
			}
			glf1 = 0.1; glf2 = 0.05;
		}
	}
}

void GlMolArea::renderBondLine() {//  渲染键线式
	glDisable(GL_LIGHTING);
	size_t n = gra.size();
	for (size_t i = 0; i < n; i++) {
		GlBondLink& bondLink = gra.at(i);
		GlAtom& atom = mol->atom(bondLink.atomId);
		glPushMatrix();
		glTranslatef(atom.x(), atom.y(), atom.z());
		glRotatef(bondLink.angle, bondLink.rotVec[0], bondLink.rotVec[1], bondLink.rotVec[2]);
		glBegin(GL_LINES);
		glColor3fv(cpkTable[atom.atomicNumber]);
		glVertex3d(0, 0, 0);
		glVertex3f(0, 0, bondLink.height);
		glEnd();
		glPopMatrix();
	}
	glEnable(GL_LIGHTING);
}

void GlMolArea::mousePressEvent(QMouseEvent* event) {
	if (event->buttons() == Qt::LeftButton) {
		setCursor(Qt::ClosedHandCursor);
		lastPos = event->pos();
	}
}

void GlMolArea::mouseMoveEvent(QMouseEvent* event) {
	if (event->buttons() == Qt::LeftButton) {
		int dx = event->x() - lastPos.x();
		int dy = event->y() - lastPos.y();
		rotateX += dx; rotateX += 360; rotateX %= 360;
		rotateY += dy; rotateY += 360; rotateY %= 360;
		lastPos = event->pos();
		updateGL();
	}
}

void GlMolArea::mouseReleaseEvent(QMouseEvent* event) {
	if (event->button() == Qt::LeftButton) {
		setCursor(Qt::OpenHandCursor);
	}
}

void GlMolArea::wheelEvent(QWheelEvent* event) {
	depthZ += event->delta() / 50.0f;
	updateDepth();
}

void GlMolArea::setMode(mode _style) {
	if (_style != style) {
		style = _style;
		updateGL();
	}
}

inline void GlMolArea::updateDepth() {
	glLoadIdentity();
	gluLookAt(0, 0, depthZ, 0, 0, 0, 1.0, 0.0, 0.0);
	updateGL();
}

void GlMolArea::keyPressEvent(QKeyEvent* event) {
	switch (event->key()) {
	case Qt::Key_Up:
		depthZ += depthStep;
		updateDepth();
		break;
	case Qt::Key_Down:
		depthZ -= depthStep;
		glLoadIdentity();
		glTranslatef(0.0f, 0.0f, depthZ);
		updateGL();
		break;
	default:
		break;
	}
}
