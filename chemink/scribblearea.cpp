#include "chemink.h"
#include "scribblearea.h"
#include <QPainter>
#include <QApplication>

using namespace std;

#define drawline(_from, _to) \
    QPainter painter(instBuffer);\
    painter.setPen(pen);\
    if (_from != _to) \
        painter.drawLine(_from, _to);\
    else \
        painter.drawPoint(_from);\
    int rad = (pen.width() / 2) + 2;\
    update(QRect(_from, _to).normalized().adjusted(-rad, -rad, +rad, +rad));

#define rubberline(_from, _to) \
    QPainter painter(instBuffer);\
    painter.setPen(rubber);\
    if (_from != _to) \
        painter.drawLine(_from, _to);\
    else \
        painter.drawPoint(_from);\
    int rad = (rubber.width() / 2) + 2;\
    update(QRect(_from, _to).normalized().adjusted(-rad, -rad, +rad, +rad));

#define BUTTON_STYLESHEET \
    "QToolButton{ border-image:url(:/img/%1-476.png)}" \
    "QToolButton:hover{border-image:url(:/img/%1-576.png)}" \
    "QToolButton:pressed{border-image:url(:/img/%1-676.png)}"

ScribbleArea::ScribbleArea(QWidget *parent) :
        isPen(true), justResized(false), QWidget(parent), instBuffer(new QPixmap(7680, 4320)),
        countCall(0), allowIn(false) {
    //setWindowFlags(Qt::FramelessWindowHint);
    instBuffer->fill(Qt::white);
    pen.setColor(Qt::black);
    pen.setCapStyle(Qt::PenCapStyle::RoundCap);
    pen.setWidth(7);
    rubber.setColor(Qt::white);
    rubber.setCapStyle(Qt::PenCapStyle::RoundCap);
    rubber.setWidth(20);

    rubberBtn = new QToolButton(this);
    pencilBtn = new QToolButton(this);
    setBtn = new QToolButton(this);
    clsBtn = new QToolButton(this);
    connect(clsBtn, &QToolButton::clicked,
            this, [=](bool checked) {
                rs.clear();
                repaintRawScript();
            }
    );
    screenLabel = new QLabel(this);
    connect(rubberBtn, &QToolButton::clicked,
            this, [=](bool checked) {
                isPen = false;
                QPixmap pixmap(":/img/rubber-476.png");
                setCursor(QCursor(pixmap.scaled(50, 50), 20, 45));
            }
    );
    rubberBtn->setWindowFlags(Qt::FramelessWindowHint);
    rubberBtn->setCheckable(false);
    connect(pencilBtn, &QToolButton::clicked,
            this, [=](bool checked) {
                isPen = true;
                QPixmap pixmap(":/img/pencil-476.png");
                setCursor(QCursor(pixmap.scaled(50, 50), 0, 40));
            }
    );
    pencilBtn->setWindowFlags(Qt::FramelessWindowHint);
    pencilBtn->setCheckable(false);
    pencilBtn->clicked(true);

    connect(
            this, &ScribbleArea::updateInScribbleArea,
            this, [=]() {
                countCall++;                //  请求耗时负载的次数加一
                std::thread *threadContainer = new std::thread([=]() {
                                                                   if (countCall >= 2) {    //  如果新来很多请求，将未执行请求合并为2个
                                                                       countCall = 2;
                                                                       return;
                                                                   }
                                                                   const vector<vector<string>> &res = rs.recognize();
                                                                   QString renderData("<html><h1>");
                                                                   for (auto &word : res.at(0)) {
                                                                       if (SpellCorrector::getWordType(word) == SpellCorrector::EXPLICTCUT) {
                                                                           renderData.append(word.c_str());
                                                                       } else {
                                                                           renderData.append(chembalancer.addHtml(string(word)).c_str());
                                                                       }
                                                                   }
                                                                   renderData.append("</h1></html>");
                                                                   QMetaObject::invokeMethod(screenLabel, "setText",
                                                                                             Q_ARG(const QString&, renderData));
                                                                   //  耗时负载结束
                                                                   if (countCall == 2) {    //  如果执行完当前请求，还有请求，就再执行一次
                                                                       countCall = 1;
                                                                       updateInScribbleArea();
                                                                   }
                                                                   countCall = 0;            //  计数器归零前，已经执行过多余的请求a
                                                               }
                );
                threadContainer->detach();    //转后台线程，由运行库负责回收
            }
    );
    rubberBtn->setStyleSheet(QString(BUTTON_STYLESHEET).arg("rubber"));
    pencilBtn->setStyleSheet(QString(BUTTON_STYLESHEET).arg("pencil"));
    setBtn->setStyleSheet(QString(BUTTON_STYLESHEET).arg("set"));
    clsBtn->setStyleSheet(QString(BUTTON_STYLESHEET).arg("cls"));
    rs.setPanel(this);
    rs.setImage(instBuffer);


    browser.setWindowModality(Qt::ApplicationModal);
    browser.hide();
    connect(setBtn, &QToolButton::clicked,
            this, [=](bool checked) {
                const vector<vector<string>> &res = rs.recognize();
                browser.setEleBox(res);
                browser.move(x(), y());
                //browser.resize(size() / 2);//  TODO:解决Browser::TolBtn的label文字居中问题？
                browser.resize(size());
                browser.show();
                this->hide();
            }
    );
    connect(&browser, &Browser::iAmClosed, this,
            [=]() {
                browser.hide();
                this->show();
            }
    );

    setFocus();
}

void ScribbleArea::mousePressEvent(QMouseEvent *event) {
    switch (event->button()) {
        case Qt::LeftButton:
            lastPos = QPoint((numeric_limits<int>::min)(), (numeric_limits<int>::min)());
            curPos = event->pos();
            if (isPen) {
                drawline(curPos, curPos);
                rs.curStroke = RawScript::PainterPath(curPos);
            } else {
                rubberline(curPos, curPos);
                rs.popStroke(curPos);
                rubberPath = QPainterPath(curPos);
            }
            break;
        default:
            break;
    }
}

void ScribbleArea::mouseMoveEvent(QMouseEvent *event) {
    switch (event->buttons()) {
        case Qt::LeftButton:
            lastPos = curPos;
            curPos = event->pos();
            if (isPen) {
                drawline(lastPos, curPos);
                rs.curStroke.lineTo(curPos);
            } else {
                rubberline(lastPos, curPos);
                rubberPath.lineTo(curPos);
            }
            autoResize();
            break;
        default:
            break;
    }

}

void ScribbleArea::mouseReleaseEvent(QMouseEvent *event) {
    switch (event->button()) {
        case Qt::LeftButton:
            if (isPen) {
                rs.pushStroke();
            } else {
                rs.popStroke(curPos);
                rs.popStroke(rubberPath);
                repaintRawScript();
                break;
            }
            emit updateInScribbleArea();
            break;
        default:
            break;
    }
}

void ScribbleArea::paintEvent(QPaintEvent *event) {
    //  qDebug() << "paint...";
    const QRect &rect = event->rect();
    QPainter painter(this);
    painter.drawPixmap(rect, *instBuffer, rect);
}

void ScribbleArea::updateLayout() {
    int w = scaleK * 4;
    int b = 10;
    rubberBtn->resize(w, w);
    setBtn->resize(w, w);
    pencilBtn->resize(w, w);
    clsBtn->resize(w, w);
    pencilBtn->move(b, b);
    rubberBtn->move(b, 2 * b + w);
    setBtn->move(b, 3 * b + 2 * w);
    clsBtn->move(b, 4 * b + 3 * w);
    screenLabel->move(2 * b + w, b);
    screenLabel->resize(width() - 3 * b - w, 1.5 * w);
    //QPixmap screenPixmap(screenLabel->size());
    //screenPixmap.fill(qRgb(225,235,235));
    //screenLabel->setPixmap(screenPixmap);
    screenLabel->setStyleSheet("background-color:rgb(225,235,235)");
}

void ScribbleArea::repaintRawScript() {
    if (instBuffer != nullptr) {
        delete instBuffer;
    }
    instBuffer = new QPixmap(7680, 4320);
    rs.setImage(instBuffer);
    instBuffer->fill(Qt::white);
    QPainter painter(instBuffer);
    painter.setPen(pen);
    for (auto &i : rs.getScript()) {
        painter.drawPath(i);
    }
    emit updateInScribbleArea();
    update();
}

void ScribbleArea::autoResize() {
    if (upBorder.contains(curPos)) {
        if (y() - scaleK > 0 && scaleK + height() < getDesktopSize().height() - 100) {
            move(x(), y() - scaleK);
            resize(width(), scaleK + height());
        }
    } else if (downBorder.contains(curPos)) {
        if (scaleK + height() < getDesktopSize().height() - 100) {
            resize(width(), scaleK + height());
        }
    }
    if (leftBorder.contains(curPos)) {
        if (x() - scaleK > 0 && width() + scaleK < getDesktopSize().width()) {
            move(x() - scaleK, y());
            resize(width() + scaleK, height());
        }
    } else if (rightBorder.contains(curPos)) {
        if (x() > 0 && x() + width() + scaleK < getDesktopSize().width()) {
            resize(width() + scaleK, height());
        }
    }
}

void ScribbleArea::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
    scaleK = width() / 64;
    updateLayout();
    upBorder.setCoords(0, 0, width(), scaleK);
    downBorder.setCoords(0, height() - scaleK, width(), height());
    leftBorder.setCoords(0, 0, scaleK, height());
    rightBorder.setCoords(width() - scaleK, 0, width() - scaleK, height());
    if (instBuffer->isNull()) {
        *instBuffer = QPixmap(size());
        instBuffer->fill(Qt::white);
    } else {
        if (instBuffer->width() < width() || instBuffer->height() < height()) {
            QPixmap *newPixmap = new QPixmap(size());
            newPixmap->fill(Qt::white);
            QPainter painter(newPixmap);
            painter.drawPixmap(0, 0, width(), height(), *instBuffer);
            delete instBuffer;
            instBuffer = newPixmap;
            rs.setImage(instBuffer);
        }
    }
    update();
}

void ScribbleArea::moveEvent(QMoveEvent *event) {
    int lastX = x(), lastY = y();
    QWidget::moveEvent(event);
    if (justResized) {
        justResized = false;
    }
    updateLayout();
}

void ScribbleArea::wheelEvent(QWheelEvent *event) {
    if (QApplication::keyboardModifiers() == Qt::CTRL) {
        QPoint numPixels = event->pixelDelta();
        QPoint numDegrees = event->angleDelta() / 8;
        if (!numPixels.isNull()) {
            if (isPen) {
                int w = pen.width() + numPixels.y();
                if (w <= 0)w = 1;
                pen.setWidth(w);
            } else {
                int w = rubber.width() + numPixels.y();
                if (w <= 0)w = 1;
                rubber.setWidth(w);
            }
        } else if (!numDegrees.isNull()) {
            QPoint numSteps = numDegrees / 15;
            if (isPen) {
                int w = pen.width() + numSteps.y();
                if (w <= 0)w = 1;
                pen.setWidth(w);
            } else {
                int w = rubber.width() + numSteps.y();
                if (w <= 0)w = 1;
                rubber.setWidth(w);
            }
        }
        event->accept();
    }
}

void ScribbleArea::closeEvent(QCloseEvent *event) {
    rubberBtn->close();
    event->accept();
}
