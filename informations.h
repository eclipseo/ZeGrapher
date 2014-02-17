/****************************************************************************
**  Copyright (c) 2013, Adel Kara Slimane, the ZeGrapher project <contact@zegrapher.com>
**
**  This file is part of the ZeGrapher project, version 2.0.
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


#ifndef INFORMATIONS_H
#define INFORMATIONS_H

#include <Structures.h>
#include "Widgets/pareqwidget.h"
#include "Calculus/seqcalculator.h"
#include "Calculus/funccalculator.h"
#include "Widgets/straightlinewidget.h"
#include "Widgets/tangentwidget.h"
#include "Calculus/colorsaver.h"

class Informations: public QObject
{
    Q_OBJECT

public:
    Informations();

    GraphRange getRange();
    bool getGridState();
    bool isOrthonormal();
    Options getOptions();  

    void setParEqsListPointer(QList<ParEqWidget*> *list);
    QList<ParEqWidget*>* getParEqsList();

    void setTangentsListPointer(QList<TangentWidget*> *list);
    QList<TangentWidget*>* getTangentsList();

    void setStraightLinesListPointer(QList<StraightLineWidget*> *list);
    QList<StraightLineWidget*>* getStraightLinesList(); 

    void checkParametricEquations();

    void setSequencesList(QList<SeqCalculator*> list);
    QList<SeqCalculator*> getSeqsList();

    void setFunctionsList(QList<FuncCalculator*> list);
    QList<FuncCalculator*> getFuncsList();

public slots:
    void emitUpdateSignal();
    void emitDrawStateUpdate();
    void emitAnimationUpdate();

signals:

    void newOrthonormalityState(bool orth);
    void updateOccured();
    void drawStateUpdateOccured();
    void animationUpdate();

public slots:

    void setRange(const GraphRange &newFenetre);
    void setGridState(bool etat);
    void setOrthonormal(bool state);
    void setOptions(Options opt);  

private:

    QList<TangentWidget*> *tangents;
    QList<StraightLineWidget*> *lines;

    QList<FuncCalculator*> functions;
    QList<SeqCalculator*> sequences;

    GraphRange range;
    Options parametres;   
    bool orthonormal, gridState, updatingLock;   
    QList<ParEqWidget*> *parEqWidgets;
};

#endif // INFORMATIONS_H
