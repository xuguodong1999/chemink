#pragma once
#include "stdafx.h"
#include "scribblescene.h"

//  实现微软PPT侧边预览栏目的效果
//  QScrollArea只能管理一个滚动条，除此以外啥都不能干
//  需要自己做布局管理、做信号管理、做子窗体管理
//  需要的接口：
//  1、发信号：接受一个窗体的指针，存储，内部创建新的子窗体，生成预览图，
//  创建一个信号，点击这个窗体，发送这个信号，指示多文档区域里，哪个文档被点击了
//  2、接收信号：接受一个窗体的指针，遍历存储区，找到并选中它，或回复没找到
//  3、覆写resize事件，做到左右伸缩窗口时，预览图自适应调整
//  4、子窗体编号有多文档生成者提供，生成者同时负责删除这个编号
class PreviewScrollArea :public QScrollArea {
	Q_OBJECT
public:
	PreviewScrollArea(QWidget* parent = nullptr);
	void setSceneMapPointer(QMap<int, ScribbleScene*>* _sceneMapPointer);
protected:
	void resizeEvent(QResizeEvent* event)override;

private:
	QMap<int, ScribbleScene*>* sceneMapPointer;
	QMap<int, QLabel*> previewMap;
public slots:
	void updatePreview(const std::vector<int>& modifiedViewIds);
};