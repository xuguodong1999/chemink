#pragma once

#include "glumolwindow.h"
#include "chembalancer.h"
#include "spellcorrector.h"
#include "forcefield.h"        //  力场工具
#include <QMap>
#include <QList>
#include <QLabel>
#include <QWidget>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QToolButton>
#include <QResizeEvent>
#include <QCloseEvent>

class LabelButton : public QLabel {
Q_OBJECT
public:
    LabelButton(QWidget *parent = nullptr);

signals:

    void btnClicked(LabelButton *address);

public:
    QString plainText, htmlText;
    int prefix;
protected:
    void mousePressEvent(QMouseEvent *event) override;
};

class Browser : public QWidget {
Q_OBJECT
public:
    Browser(QWidget *parent = nullptr);

    /*为所有包含语义的标签元素创建信号和槽*/
    void setEleBox(const std::vector<std::vector<std::string>> &_elements);

signals:

    void iAmClosed();

private:
    QGridLayout *mainLayout;
    QHBoxLayout *eleBoxLayout;
    GluMolWindow *molWindow;
    QToolButton *leftBtn, *rightBtn;
    QList<QPair<QString, LabelButton *>> elements;
    std::vector<std::vector<std::string>> *eledata;
    ChemBalancer chembalancer;
    Molecule mol;
    ForceField forcefield;
    QMap<LabelButton *, std::vector<std::string>> smilesDatas;
    std::vector<std::string>::iterator smilesIter;
    LabelButton *clickedBtn;

    void updatelabelText();

    QLabel *screen;
private slots:

    void updateMolWindow(const char *smiles);

    void handleBtnClickEvent(LabelButton *address);

protected:
    void closeEvent(QCloseEvent *event) override;

    void resizeEvent(QResizeEvent *event) override;
};
