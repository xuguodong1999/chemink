#pragma once
//  字符纠正
#include "spellcorrector.h"
//  目标检测
#include "objdetector.h"
//  单例分类
#include "classifier.h"
#include <QPainterPath>
#include <QWidget>
#include <QList>
#include <QPixmap>

//  利用Qt的容器做笔迹管理
class RawScript : public QObject {
Q_OBJECT
public:
    class PainterPath : public QPainterPath {
        //  TODO:重写boundingRect等函数
    public:
        PainterPath();

        PainterPath(const QPoint &start);

    private:
    protected:
    };

    RawScript();

    ~RawScript();

    void setPanel(const QWidget *_panel);

    void setImage(const QPixmap *_pixmap);

    PainterPath curStroke;

    //  把curStroke存入QList，清空curStroke
    void pushStroke();

    //  清空所有笔画，清空缓存
    void clear();

    //  删除script的最后一画
    void popStroke();

    //  删除和rubberPath相交的笔画
    void popStroke(const QPainterPath &rubberPath);

    //  删除script中包含在举行innerPoint±scale里的点
    void popStroke(const QPoint &innerPoint, QPoint scale = QPoint(5, 5));

    //  返回script的基类模板的引用
    const QList<PainterPath> &getScript();

    const std::vector<std::vector<std::string>> &recognize();

private:
    //  约定，第一层，即result[0]放最佳答案，即n个单词
    //  设一共识别出k个字符，后面有k个QList，每个QList存放ki个备选结果供修改
    std::vector<std::vector<std::string>> results;
    QList<PainterPath> script;
    SpellCorrector corrector;
    Classifier classifier;
    ObjDetector detector;
    const QSize yoloBaseSize = QSize(512, 416);
    const QSize fdeepBaseSize = QSize(64, 64);
    const double K_leak = 0.5, K_allowIn = 0.1;
    //  返回l、m、r，表示b在a的左侧、包含、在右侧
    //  char diff(const QRectF& a, const QRectF& b);
    QWidget *panel;
    QPixmap *pixmap;
    QPixmap blankImg;
};
