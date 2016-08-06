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


#ifndef EXPRCALCULATOR_H
#define EXPRCALCULATOR_H

#include "Structures.h"

class ZeSet;

class ZeFunction;

class ZeExpression
{
public:

    explicit ZeExpression(ZeSet *set = nullptr);
    explicit ZeExpression(QString expr, ZeSet *set = nullptr);
    ZeExpression(const ZeExpression &other);

    void setExpression(QString expr);
    void setMathObjectsSet(ZeSet *set);
    bool isValid();
    QString getErrorMessage();

    double eval();



protected:

    QStringList getCalledFuncs(QString expr);
    QStringList getCalledSeqs(QString expr);
    void deleteZeTree(ZeTree *tree);    

    double calculateExpression(QString expr, bool &ok, double k_val = 0);

    bool checkCalledFuncsValidity(QString expr);

    double calculateFromTree(ZeTree *tree, double x = 0);

    ZeTree* getTreeFromExpr(QString expr, bool &ok, QStringList additionnalVars = QStringList());

    QStringList splitExpression(QString expr);

    void checkExpression();
    QStringList splitExpression(QString expr);

    void refreshAuthorizedVars();
    ZeTree* createZeTree(QList<Element> split);

    void addStandardFuncs();

    enum struct ElementType
    {
        OP_LOW ,
        OP_HIGH ,
        POW ,
        FUNC ,
        DERIV_FUNC ,
        INTGR_FUNC ,
        O_PTH ,
        C_PTH ,

        NUMBER ,
        CONSTANT,
        VAR

    };

    enum struct NodeType
    {
        PLUS ,
        MINUS ,
        MULTIPLY,
        DIVIDE,
        POW ,

        FUNC ,
        DERIV_FUNC ,
        INTGR_FUNC ,

        NUMBER ,
        CONSTANT,
        VAR ,
        NONE
    };

    struct Element
    {
        ElementType elemType;
        NodeType nodeType;
        QString val;
    };   

    struct ZeTree
    {
        NodeType type;
        QString name;
        double value;
        ZeTree *left;
        ZeTree *right;
    };


    QStringList calledFunctions, calledSequences;

    QList<Element> deconstruction;

    QHash<QString, double (*)(double)> standardFunctions;
    QHash<QString, double> varsAndVals;

    bool valid;
    QString expression, errorMessage;
    ZeSet *mathObjects;
    ZeTree *tree;
};

#endif // EXPRCALCULATOR_H
