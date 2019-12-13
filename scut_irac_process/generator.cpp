#include "generator.h"
#include <sstream>
#include <fstream>
#include <QLabel>

Generator::Generator() {

}

Generator::~Generator() {
}

/*处理已经完成归一化的原始数据，返回表示完整字符的QPainterPath*/
QPainterPath Generator::getPath(const vector<vector<pair<int, int>>>& script) {
	QPainterPath result;
	for (auto& i : script) {
		QPainterPath tmp;
		tmp.moveTo(i.begin()->first, i.begin()->second);
		for (auto& j : i) {
			tmp.lineTo(j.first, j.second);
		}
		result.addPath(tmp);
	}
	//qDebug() << result.boundingRect();
	return result;
}

QImage Generator::getSingleImage(const QPainterPath& script, int width, int channels) {
	assert(channels == 1 || channels == 3);
	QImage result;
	int maxX, maxY, penWidth, extW, maxW;
	maxX = maxY = ((numeric_limits<int>::min)());
	for (int i = 0; i < script.elementCount(); i++) {
		int x = script.elementAt(i).x;//  CAUTION
		int y = script.elementAt(i).y;
		maxX = maxX > x ? maxX : x;
		maxY = maxY > y ? maxY : y;
	}
	//cout << maxX << "," << maxY << endl;
	penWidth = 5;

	//  让空白宽度呈现高斯分布
	//unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	//std::default_random_engine gen(seed);
	//std::normal_distribution<double> dis(max(maxX, maxY) / 3, 10);
	//extW = dis(gen);
	extW = rand() % max(maxX, maxY);


	maxW = extW + max(maxX, maxY) + penWidth;
	if (channels == 1) {
		result = QImage(maxW, maxW, QImage::Format::Format_Grayscale8);
	}
	else {
		result = QImage(maxW, maxW, QImage::Format::Format_RGB888);
	}
	if (rand() % 2 == 0)
		result.fill(Qt::white);
	else {
		int minusrgb = 50;
		result.fill(qRgb(255 - rand() % minusrgb, 255 - rand() % minusrgb, 255 - rand() % minusrgb));
	}
	QPainter painter(&result);

	int startX = rand() % (maxW - maxX) / 2,
		startY = rand() % (maxW - maxY) / 2;
	int addon = penWidth;
	if (startX < penWidth + addon)startX = penWidth + addon;
	if (startY < penWidth + addon)startY = penWidth + addon;
	if (startX > maxW - penWidth - addon - maxX)startX = maxW - penWidth - addon - maxX;
	if (startY > maxW - penWidth - addon - maxY)startY = maxW - penWidth - addon - maxY;

	painter.translate(startX, startY);
	int angle = (rand() % 24) - 8;
	painter.rotate(angle);

	painter.setPen(QPen(Qt::black, penWidth));
	painter.drawPath(script);

	//int end = rand() % (maxW * 3);
	//srand(rand() % 65536);
	//for (int i = 0; i < end; i++) {
	//	painter.setPen(QPen(Qt::black, rand() % 2 + 1));
	//	int x = rand() % maxW, y = rand() % (maxW - 1);
	//	painter.drawPoint(x, y);
	//}

	return result.scaled(width, width, Qt::IgnoreAspectRatio);
}

bool Generator::addToImage(QImage& src, const QPainterPath& script) {
	return false;
}

int Generator::charCounter = 0;
int Generator::fileCounter = 0;
int Generator::maxRandCounter = 0;
int Generator::blockCharCounter = 0;
int Generator::maxRandBlockCharCounter = 0;
const QRect Generator::baseOutline = QRect(0, 0, 416, 416);
const int Generator::maxCounter = 20;
const int Generator::maxBlockCharCounter = 10;
QList<QRect> Generator::existedRects = QList<QRect>();
/*调度整个数据集上的添加操作*/
bool Generator::IsLower = false;
QRect Generator::refRect;
vector<pair<int, vector<vector<pair<int, int>>>>> tmpData;

void Generator::merge(
	const vector<vector<vector<pair<int, int>>>>& _imgs,
	const vector<int>& _intLabels,
	const char* dir
) {
	const int n = _imgs.size();
	for (int i = 0; i < n; i++) {
		tmpData.push_back(
			pair<int, vector<vector<pair<int, int>>>>(
				_intLabels.at(i), _imgs.at(i)
				)
		);
	}
	shuffle(tmpData.begin(), tmpData.end(), std::default_random_engine(1));
	shuffle(tmpData.begin(), tmpData.end(), std::default_random_engine(2));
	shuffle(tmpData.begin(), tmpData.end(), std::default_random_engine(3));
	shuffle(tmpData.begin(), tmpData.end(), std::default_random_engine(4));
	shuffle(tmpData.begin(), tmpData.end(), std::default_random_engine(5));
	vector<vector<vector<pair<int, int>>>> imgs;
	vector<int> intLabels;
	for (auto& i : tmpData) {
		imgs.push_back(i.second);
		intLabels.push_back(i.first);
	}
	tmpData.clear();
	QPainterPath character; int label;
	ofstream labelWriter;
	charCounter = 0;  blockCharCounter = 0; IsLower = false;
	maxRandCounter = rand() % (maxCounter - 5) + 5;
	maxRandBlockCharCounter = rand() % (maxBlockCharCounter - 5) + 5;
	string filePrefix; char ftmp[7];
	sprintf(ftmp, "%06d", fileCounter);
	filePrefix = ftmp;
	labelWriter.open(string(dir) + filePrefix + ".txt");
	QImage pic(baseOutline.size(), QImage::Format::Format_RGB32);
	pic.fill(Qt::white);
	QPoint startPos;
	for (size_t i = 0; i < n; i++) {
		if (charCounter > maxRandCounter) {
			cout << "charCounter = " << charCounter << endl;
			//system("pause");
			existedRects.clear();
			//QLabel* label = new QLabel();
			//label->setPixmap(QPixmap::fromImage(pic));
			//label->show();
			pic.save(QString(dir) + filePrefix.c_str() + QString(".jpg"));
			pic.fill(Qt::white);
			labelWriter.close();
			charCounter = 0; blockCharCounter = 0;
			maxRandCounter = rand() % (maxCounter - 5) + 5;
			existedRects.clear();
			fileCounter++;
			sprintf(ftmp, "%06d", fileCounter);
			filePrefix = ftmp;
			//system("pause");
			labelWriter.open(string(dir) + filePrefix + ".txt");
		}
		character = getPath(imgs.at(i));
		label = intLabels.at(i);
		if (randGenStart(character.boundingRect().size().toSize(), startPos)) {
			QPainter painter(&pic);
			int pw = 2 + rand() % 3;
			painter.setPen(QPen(Qt::black, pw));
			painter.translate(startPos);
			painter.drawPath(character);
			labelWriter << label << " "
				<< (double)(startPos.x() - pw + character.boundingRect().width() / 2.0) / baseOutline.width() << " "
				<< (double)(startPos.y() - pw + character.boundingRect().height() / 2.0) / baseOutline.height() << " "
				<< (double)(character.boundingRect().width() + 2 * pw) / baseOutline.width() << " "
				<< (double)(character.boundingRect().height() + 2 * pw) / baseOutline.height() << endl;
		}
		else {
			//  不能让块内碰撞消耗掉这100次机会，检测到块内碰撞，立即重置块大小
			if (blockCharCounter != 0) {
				blockCharCounter = 0; i--; charCounter--;
			}
			else {
				//  如果碰撞检测失败了100次，把charCounter置最大值，i--，进入下一次循环
				charCounter = 0x3f3f3f3f; i--; blockCharCounter = 0;
			}
		}
		charCounter++;
	}
	/*添加的策略：以第一个字符为参照点，将剩余字符依次向右、随机偏下地平移*/
	/*添加完成后，将所有坐标归一化*/
	/*决定平移起点的因素是前面的字符的右边界的精确参数*/
	/*不对墨迹进行resize*/
	cout << "charCounter = " << charCounter << endl;
	//system("pause");
	existedRects.clear();
	//QLabel* label = new QLabel();
	//label->setPixmap(QPixmap::fromImage(pic));
	//label->show();
	pic.save(QString(dir) + filePrefix.c_str() + QString(".jpg"));
	pic.fill(Qt::white);
	labelWriter.close();
	charCounter = 0; blockCharCounter = 0;
	maxRandCounter = rand() % (maxCounter - 5) + 5;
	existedRects.clear();
	fileCounter++;
}

bool Generator::IsValidNewRect(const QRect& newRect) {
	if (!isIn(baseOutline, newRect)) {
		return false;
	}
	for (auto& i : existedRects) {
		if (newRect.intersects(i)) {
			return false;
		}
	}
	return true;
}

bool Generator::randGenStart(const QSize& fixedSize, QPoint& startPos, const int maxTry) {
	int w = baseOutline.width() - fixedSize.width(),
		h = baseOutline.height() - fixedSize.height();
	QRect newRect;
	for (int i = 0; i < maxTry; i++) {
		int startX, startY;
		if (blockCharCounter == 0) {
			startX = rand() % w; startY = rand() % h;
		}
		else {
			/*
			-----------------
			|		|
			|		|--------
			|		|		|
			|		|		|
			----------------|
			*/
			//  控制起点位置来调整数据分布
			//  依据 refRect 的位置
			if (IsLower) {//  如果上一个字符被缩小了
				//  获取基准位置
				int baseX = refRect.topRight().x();
				int baseY = refRect.center().y() - refRect.height();
				IsLower = rand() % 2;//以0.5的概率向下移动下一个字符
				if (IsLower) {
					startX = baseX + rand() % 10 + 15;
					startY = baseY + refRect.height() + rand() % 20 - 10;
				}
				else {
					startX = baseX + rand() % 10 + 15;
					startY = baseY + rand() % 20 - 10;
				}
			}
			else {						//  基准是正常字符
				//  获取基准位置
				int baseX = refRect.topRight().x();
				int baseY = refRect.topRight().y();
				IsLower = rand() % 2;	//以0.5的概率向下移动下一个字符
				if (IsLower) {
					startX = baseX + rand() % 10 + 15;
					startY = baseY - refRect.height() / 2 + rand() % 20 - 10;
				}
				else {
					startX = baseX + rand() % 10 + 15;
					startY = baseY + rand() % 20 - 10;
				}
			}
		}
		newRect.setX(startX); newRect.setY(startY);
		newRect.setWidth(fixedSize.width());
		newRect.setHeight(fixedSize.height());
		if (IsValidNewRect(newRect)) {
			startPos = QPoint(startX, startY);
			//qDebug() << QRect(startPos.x(), startPos.y(), fixedSize.width(), fixedSize.height()) << ", " << newRect;
			refRect = QRect(startPos.x(), startPos.y(), fixedSize.width(), fixedSize.height());
			existedRects.push_back(refRect);
			blockCharCounter++;
			if (blockCharCounter > maxRandBlockCharCounter) {
				blockCharCounter = 0;
				maxRandBlockCharCounter = rand() % (maxBlockCharCounter - 5) + 5;
			}
			return true;
		}
	}
	return false;
}

bool Generator::isIn(const QRect& big, const QRect& small) {
	const QPoint s1 = small.topLeft(), s2 = small.topRight(),
		s3 = small.bottomLeft(), s4 = small.bottomRight();
	if (big.contains(s1) && big.contains(s2) && big.contains(s3) && big.contains(s4)) {
		return true;
	}
	return false;
}


