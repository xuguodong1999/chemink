#pragma once
#include <QPainterPath>
#include <QString>
#include <QRect>
#include <QPoint>
#include <vector>
#include <map>
using namespace std;

//  以矢量形式冗余地存放字符数据

class CharInfo {
public:
	CharInfo();
	QPainterPath path;	//  路径坐标，写入后不得更新
	int w, h;
	QRect boundingRect;	//  边框坐标，根据所有信息更新
	QPoint blockPos;	//  在字符块中的位置，制作字符块的时候更新
	QPoint imgPos;		//  在画布上的位置，写入字符块的时候使用
	qreal k;			//  缩放系数，以左上角为中心缩放，写入环节使用
	QString label;		//  标签
	void set(const vector<vector<pair<int, int>>>& script,string& _label);
	void collectRect();
};

/*
处理流程：
字符-(向右坍缩、拼接)->
字符块-(在画布上做碰撞检测)->
写入画布-(达到一定数量了吗)->继续 or 结束
*/

class CharBlock {
public:
	QList<CharInfo> b;
	QPoint imgPos;		//  字符块在画布上的位置
	void setImgPos(const QPoint&p);
	QRect boundingRect;	//  字符块的最小边框
	void append(const CharInfo& newChar);
};