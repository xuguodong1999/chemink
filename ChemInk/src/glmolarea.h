#pragma once
#include "glcheminfo.h"
#include <QGLWidget>
#include <string>
#include <vector>
//  #include "windows.h"
#include <GL/glu.h>

enum molformat {
	PDB,
	MOL2
};

enum mode {
	LINES,	//  键线式
	CPK,	//  球棍模型，Corey-Pauling-Koltun
	PROP	//  比例模型
};

class GlMolArea : public QGLWidget {
	Q_OBJECT
public:
	GlMolArea(const char* data, molformat f = PDB, QWidget* parent = nullptr);
    ~GlMolArea()override;
	GlMol* mol;//  大分子数据量会很大，不重复复制数据，注意析构
	void setMode(mode _style);
	void setMolecule(const char* data, molformat f = PDB);

private:
	static const float cpkTable[16][4];

	static const float depthStep;
	inline void updateDepth();
	float depthZ;
	int rotateX, rotateY, rotateZ;
	QPoint lastPos;
	mode style;

	std::vector<GlBondLink> gra;
    GLUquadricObj* quadric;
	void renderBondTube();
	void renderBondLine();
	void renderAtomBall(mode _mode);

protected:
	void initializeGL()override;
	void paintGL()override;
	void resizeGL(int width, int height)override;
	void mousePressEvent(QMouseEvent* event)override;
	void mouseMoveEvent(QMouseEvent* event)override;
	void mouseReleaseEvent(QMouseEvent* event)override;
	void wheelEvent(QWheelEvent* event)override;
	void keyPressEvent(QKeyEvent* event)override;


};

