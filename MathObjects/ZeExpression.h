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

struct ZeTree
{
    short type;
    QString name;
    double value;
    ZeTree *left;
    ZeTree *right;
};


enum {
    OP_LOW,
    OP_HIGH,
    POW ,
    FUNC ,
    PTHO ,
    PTHF ,

    NUMBER ,
    VAR ,

    VARS_START,

    VAR_X ,
    VAR_T ,
    VAR_N ,
    PAR_K ,

    PLUS ,
    MINUS ,
    MULTIPLY ,
    DIVIDE ,

    SEQUENCES_START ,

    SEQ_U ,
    SEQ_V ,
    SEQ_L ,
    SEQ_W ,
    SEQ_Q ,
    SEQ_Z ,

    SEQUENCES_END ,

    INTEGRATION_FUNC_START ,

    INTGR_F ,
    INTGR_G ,
    INTGR_H ,
    INTGR_P ,
    INTGR_R ,
    INTGR_M ,

    INTEGRATION_FUNC_END ,

    DERIV_START ,

    DERIV_F ,
    DERIV_G ,
    DERIV_H ,
    DERIV_P ,
    DERIV_R ,
    DERIV_M ,

    DERIV_END ,

    FUNC_START ,

    FUNC_F ,
    FUNC_G ,
    FUNC_H ,
    FUNC_P ,
    FUNC_R ,
    FUNC_M ,

    FUNC_END,

    REF_FUNC_START,

    ACOS ,
    ASIN ,
    ATAN ,
    COS ,
    SIN ,
    TAN ,
    SQRT ,
    LOG ,
    LN ,
    ABS ,
    EXP ,
    FLOOR ,
    CEIL ,
    COSH ,
    SINH ,
    TANH ,
    E , // E for 1.2E10 or 1.2E+10 also 1.2E-3
    e , // same as E
    ACOSH,
    ASINH,
    ATANH,
    ERF,
    ERFC,
    GAMMA,
    GAMMA2, //the same function but with another name: "gamma" or "Γ"
    CH,
    SH,
    TH,
    ACH,
    ASH,
    ATH,


    REF_FUNC_END ,

    ADDITIONNAL_VARS_START

};

class ZeSet;

class ZeFunction;

class ZeExpression
{
public:

    explicit ZeExpression(ZeSet *set = nullptr);
    explicit ZeExpression(QString expr, ZeSet *set = nullptr);

    void setExpression(QString expr);
    void setMathObjectsSet(ZeSet *set);
    bool isValid();
    QString getErrorMessage();

    double eval();



protected:

    QList<int> getCalledFuncs(QString expr);
    QList<int> getCalledSeqs(QString expr);
    void deleteZeTree(ZeTree *tree);    

    double calculateExpression(QString expr, bool &ok, double k_val = 0);

    bool checkCalledFuncsValidity(QString expr);

    double calculateFromTree(ZeTree *tree, double x = 0);

    ZeTree* getTreeFromExpr(QString expr, bool &ok, QStringList additionnalVars = QStringList());

    QStringList splitExpression(QString expr);

    void checkExpression();
    QStringList splitExpression(QString expr);
    void insertMultiplySigns(QString &formula);
    void refreshAuthorizedVars();
    ZeTree* createZeTree(int debut, int fin);



    QStringList refFunctions, functions, sequences, antiderivatives, derivatives, constants, vars, customVars;
    QList<double> constantsVals;

    QList<QChar> operators;
    QList<short> decompPriorites, decompTypes, operatorsPriority, operatorsTypes;
    QList<double> decompValues;

    bool valid;
    QString expression;
    ZeSet *mathObjects;
    ZeTree *currentTree;
};

#endif // EXPRCALCULATOR_H
