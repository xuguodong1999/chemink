#pragma once
#include <QPainterPath>
#include <QImage>
#include <QPainter>
#include <chrono>
#include <iostream>
#include <QDebug>
#include <QList>
#include <QPair>
#include <vector>
#include <cassert>
#include <random>
#include <map>

using namespace std;

#define CHARINFO QPair<QPainterPath>

class Generator {
public:
	Generator();
	~Generator();
	/*处理已经完成归一化的原始数据，返回表示完整字符的QPainterPath*/
	static QPainterPath getPath(const vector<vector<pair<int, int>>>& script);
	/*将包含若干painterpath的painterpath写入属性为width，channels的QImage并返回*/
	static QImage getSingleImage(const QPainterPath& script, int width = 64, int channels = 3);
	/*将script表示的字符嵌入src，空间不足则返回false*/
	static bool addToImage(QImage& src, const QPainterPath& script);

	/*调度整个数据集上的添加操作*/
	static void merge(
		const vector<vector<vector<pair<int, int>>>>& _imgs,
		const vector<int>& _intLabels,
		const char* dir = "E:/test/"
	);

	static int
		charCounter,				//  当前抽取的字符计数
		fileCounter,				//  已生成的文件计数
		blockCharCounter,			//  行字符块中的字符计数
		maxRandBlockCharCounter,	//  随机行字符块中，字符数量上限
		maxRandCounter;				//  随机最大上限，受制于maxCounter

	static const int
		maxCounter,					//  一张图最多这么多字符
		maxBlockCharCounter;				//  一个块最多这么多字符

	static const QRect baseOutline;		//  外边框

	static QList<QRect> existedRects;	//  所有参与碰撞检测的边框

	static bool IsValidNewRect(const QRect& newRect);

	static bool isIn(const QRect& big, const QRect& small);

	static QRect refRect;//  针对产生下一个新边框的参考位置，一旦产生新边框，则更新这个位置
	static bool IsLower;



	static bool randGenStart(const QSize& fixedSize, QPoint& startPos, const int maxTry = 100);
};