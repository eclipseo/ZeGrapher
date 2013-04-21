/****************************************************************************
**  Copyright (c) 2013, Adel Kara Slimane, the ZeGrapher project <contact@zegrapher.com>
**
**  This file is part of the ZeGrapher project, version 2.1.
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

#ifndef COLUMNSELECTORWIDGET_H
#define COLUMNSELECTORWIDGET_H

#include <QtGui>

#define XSELECTOR 0
#define YSELECTOR 1
#define SELECTOR 2

#define POLAR 0
#define CARTESIAN 1

#define ANIMATION_TIME 390 //in ms
#define ANIMATION_PERIOD 15

struct Animation
{
    int progress, departureAbscissa, arrivalAbscissa, arrivalIndex;
};

struct Selector
{
    QImage image;
    bool betweenColumns, draw;
    int index;
    QPoint pos;
    Animation animation;
};

class ColumnSelectorWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ColumnSelectorWidget(int count);
    void setCoordinateSystem(short sys);
    void updateSelectorsPos();

public slots:
    void setColumnCount(int count);
    void askedForSelector();
    
signals:
    void newXIndex(int index);
    void newYIndex(int index);
    void newSelectorPos(bool betweenColumns, int index);
    void askForSelector();

protected slots:
    void updateAnimationProgress();

protected:
    void drawSelectors();


    Selector* otherSelector(Selector* notThisOne);

    void paintEvent(QPaintEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

    QPainter painter;
    QTimer timer;
    int columnCount;
    short coordinateSystem;
    bool hasSelector;
    Selector xselector, yselector, selector;
    Selector *draggedSelector;
    QList<Selector*>selectors;
    QList<Selector*>animatedSelectors;
    
};

#endif // COLUMNSELECTORWIDGET_H
