/****************************************************************************
**  Copyright (c) 2016, Adel Kara Slimane <adel.ks@zegrapher.com>
**
**  This file is part of ZeGrapher's source code.
**
**  ZeGrapher is free software: you may copy, redistribute and/or modify it
**  under the terms of the GNU General Public License as published by the
**  Free Software Foundation, either version 3 of the License, or (at your
**  option) any later version.
**
**  This file is distributed in the hope that it will be useful, but
**  WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
**  General Public License for more details.
**
**  You should have received a copy of the GNU General Public License
**  along with this program.  If not, see <http://www.gnu.org/licenses/>.
**
****************************************************************************/



#ifndef FUNCCALCULATOR_H
#define FUNCCALCULATOR_H

#include "Structures.h"
#include "colorsaver.h"

#include "./ZeExpression.h"

class ZeFunction : public QObject
{
    Q_OBJECT

public:
    ZeFunction(QString funcName, QString mainVarName, ZeSet *mathObjects);

    void setIntegrationPointsList(QList<Point> list);

    bool checkFuncCallingInclusions();

    double evalAntiderivative(double b, Point A, double k_val = 0);
    double eval(QHash<QString, double> vals);
    double getDerivativeValue(double x, double k_val = 0);


    bool canBeCalled();
    bool validateExpression(QString expr);    
    bool isFuncValid();
    bool getDrawState();
    bool isFuncParametric();   

    Range getParametricRange();

public slots:    
    void setDrawState(bool draw);

protected:
    double calculateFromTree(ZeTree *tree, double x);
    void addRefFuncsPointers();     

    int funcNum;
    double k;
    bool isExprValidated, isParametric, areCalledFuncsGood, areIntegrationPointsGood, drawState, callLock;
    TreeCreator treeCreator;
    ZeTree *funcTree;
    QString expression, name;
    QList<ZeFunction*> funcCalculatorsList;
    Range kRange;    
    ColorSaver *colorSaver;
    QLabel *errorMessageLabel;

    QList<Point> integrationPoints;
    QList<double (*)(double)> refFuncs;    
};

#endif // FUNCCALCULATOR_H
