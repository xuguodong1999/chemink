#-------------------------------------------------
#
# Project created by QtCreator
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Qt_Chemical_Handwriting_Board
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
    TaskDistributionInterface.cpp \
    ChemicalEquationInterpreter/Element.cpp \
    ChemicalEquationInterpreter/Equation.cpp \
    ChemicalEquationInterpreter/Fraction.cpp \
    ChemicalEquationInterpreter/Function.cpp \
    ChemicalEquationInterpreter/Matter.cpp \
    DrawingBoard/MyScript.cpp \
    DrawingBoard/KeyBoard.cpp \
    DrawingBoard/SetBoard.cpp \
    MojoCNN/dictionary.cpp \
    DrawingBoard/DrawingBoard.cpp

HEADERS += \
    TaskDistributionInterface.h \
    ChemicalEquationInterpreter/config.h \
    ChemicalEquationInterpreter/Element.h \
    ChemicalEquationInterpreter/Equation.h \
    ChemicalEquationInterpreter/Fraction.h \
    ChemicalEquationInterpreter/Function.h \
    ChemicalEquationInterpreter/Matter.h \
    DrawingBoard/MyScript.h \
    DrawingBoard/KeyBoard.h \
    DrawingBoard/SetBoard.h \
    DrawingBoard/DrawingBoard.h \
    MojoCNN/activation.h \
    MojoCNN/core_math.h \
    MojoCNN/cost.h \
    MojoCNN/layer.h \
    MojoCNN/mojo.h \
    MojoCNN/network.h \
    MojoCNN/solver.h \
    MojoCNN/dictionary.h \
    MojoCNN/PatternRecognition.h \
    MojoCNN/util.h

FORMS += \
    TaskDistributionInterface.ui \
    DrawingBoard/KeyBoard.ui \
    DrawingBoard/SetBoard.ui \
    DrawingBoard/DrawingBoard.ui

CONFIG += mobility
MOBILITY = 

