/****************************************************************************
**  Copyright (c) 2019, Adel Kara Slimane <adel.ks@zegrapher.com>
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


#include "GraphDraw/mainview.h"

MainView::MainView(Information *info): BaseGraphDraw(info)
{
    orientation = QPageLayout::Landscape;
    moveType = NOTHING;
    setMouseTracking(true);
    userScalingFactor = 1;
    sheetMarginCm = 0;
    imageMarginPx = 0;

    minRelSize = RELATIVE_MIN_SIZE;

    screenDPI = qGuiApp->primaryScreen()->physicalDotsPerInch();

    sheetFigureRectRelative.setHeight(1);
    sheetFigureRectRelative.setWidth(1);
    sheetFigureRectRelative.moveTopLeft(QPointF(0, 0));

    imageFigureRectRelative.setHeight(1);
    imageFigureRectRelative.setWidth(1);
    imageFigureRectRelative.moveTopLeft(QPointF(0, 0));
}

void MainView::onSizeUnitChange()
{
    updateTargetSupportSizePx();
}

void MainView::updateTargetSupportSizePx()
{
    if(information->getGraphSizeSettings().sizeUnit == ZeSizeSettings::CENTIMETER)
    {
        // TODO: determine orientation from the sheet size: whether or not sheetsizecm height > width
        QSizeF sheetSizeMm(sheetSizeCm.width() * 10, sheetSizeCm.height() * 10);
        QPageLayout layout(QPageSize(sheetSizeMm, QPageSize::Millimeter), orientation, QMarginsF(), QPageLayout::Millimeter);
        layout.setOrientation(orientation);
        targetSupportSizePixels = layout.fullRectPixels(int(screenDPI)).size();

        if(fabs(layout.fullRect().width() / layout.fullRect().height() - sheetSizeMm.width() / sheetSizeMm.height()) > 0.01)
            targetSupportSizePixels.transpose();
    }
    else
    {
        targetSupportSizePixels = sheetSizePx;
    }
}

QSize MainView::getTargetSupportSizePixels()
{
    return targetSupportSizePixels;
}

double MainView::getMinFigureRelativeSize()
{
    return minRelSize;
}

void MainView::exportPDF(QString fileName, SheetSizeType sizeType)
{
    QPdfWriter *pdfWriter = new QPdfWriter(fileName);

    pdfWriter->setCreator(QString("ZeGrapher ") + SOFTWARE_VERSION_STR);
    pdfWriter->setTitle(tr("Exported graph"));

    int targetResolution = int(screenDPI / userScalingFactor);

    pdfWriter->setResolution(targetResolution);   

    QPageLayout layout;
    layout.setUnits(QPageLayout::Millimeter);

    QSizeF sheetSizeMm(sheetSizeCm.width() * 10, sheetSizeCm.height() * 10);
    layout.setPageSize(QPageSize(sheetSizeMm, QPageSize::Millimeter, "", QPageSize::FuzzyOrientationMatch));

    if(sizeType == SheetSizeType::NORMALISED)
        layout.setOrientation(orientation);
    else layout.setOrientation(QPageLayout::Portrait);

    pdfWriter->setPageLayout(layout);

    painter.begin(pdfWriter);

    if(information->getGraphSettings().backgroundColor != QColor(Qt::white))
    {
        painter.setBrush(QBrush(information->getGraphSettings().backgroundColor));
        painter.drawRect(painter.viewport());
    }

    figureRectScaled = getFigureRect(painter.viewport());

    painter.translate(figureRectScaled.topLeft());

    paint();

    painter.end();

    delete pdfWriter;
}

void MainView::exportSVG(QString fileName)
{
    QSvgGenerator svgGenerator;
    svgGenerator.setFileName(fileName);

    svgGenerator.setTitle(tr("Exported graph"));
    svgGenerator.setDescription(tr("Created with ZeGrapher ") + SOFTWARE_VERSION_STR);

    double targetResolution = screenDPI / userScalingFactor;

    svgGenerator.setResolution(int(targetResolution));

    QSize sizePx(int(sheetSizeCm.width() * 0.393701 * targetResolution), int(sheetSizeCm.height() * 0.393701 * targetResolution));

    svgGenerator.setSize(sizePx);
    svgGenerator.setViewBox(QRect(QPoint(0, 0), sizePx));

    painter.begin(&svgGenerator);

    if(information->getGraphSettings().backgroundColor != QColor(Qt::white))
    {
        painter.setBrush(QBrush(information->getGraphSettings().backgroundColor));
        painter.drawRect(painter.viewport());
    }

    figureRectScaled = getFigureRect(painter.viewport());

    painter.translate(figureRectScaled.topLeft());

    paint();

    painter.end();
}


void MainView::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    painter.begin(this);    

    drawSupport();
    drawFigureRect();

    assignMouseRects();

    drawGraph();

    painter.end();
}

void MainView::assignMouseRects()
{
    QPoint topLeftTranslation;
    topLeftTranslation.setX(-8);
    topLeftTranslation.setY(-8);

    QPoint bottomRightTranslation;
    bottomRightTranslation.setX(8);
    bottomRightTranslation.setY(8);


    topLeft.setTopLeft(figureRect.topLeft() + topLeftTranslation);
    topLeft.setBottomRight(figureRect.topLeft() + bottomRightTranslation);

    topRight.setTopLeft(figureRect.topRight() + topLeftTranslation);
    topRight.setBottomRight(figureRect.topRight() + bottomRightTranslation);

    bottomLeft.setTopLeft(figureRect.bottomLeft() + topLeftTranslation);
    bottomLeft.setBottomRight(figureRect.bottomLeft() + bottomRightTranslation);

    bottomRight.setTopLeft(figureRect.bottomRight() + topLeftTranslation);
    bottomRight.setBottomRight(figureRect.bottomRight() + bottomRightTranslation);

    top.setTopLeft(topLeft.topRight());
    top.setBottomRight(topRight.bottomLeft());

    bottom.setTopLeft(bottomLeft.topRight());
    bottom.setBottomRight(bottomRight.bottomLeft());

    left.setTopLeft(topLeft.bottomLeft());
    left.setBottomRight(bottomLeft.topRight());

    right.setTopLeft(topRight.bottomLeft());
    right.setBottomRight(bottomRight.topRight());
}

QRect MainView::getDrawableRect(const QRect &refSupportRect)
{   // gives the drawable rect in the given support, in the support's coordinates
    // the drawable rect is the support's rect minus the margins

    QSize supportSize(refSupportRect.size());
    QPoint marginTopLeft;

    if(information->getGraphSizeSettings().sizeUnit == ZeSizeSettings::CENTIMETER)
    {
        marginTopLeft.setX(int(sheetMarginCm / sheetSizeCm.width() * double(supportSize.width())));
        marginTopLeft.setY(int(sheetMarginCm / sheetSizeCm.height() * double(supportSize.height())));
    }
    else
    {
        marginTopLeft.setX(int(double(imageMarginPx) / double(sheetSizePx.width()) * double(supportSize.width())));
        marginTopLeft.setY(int(double(imageMarginPx) / double(sheetSizePx.height()) * double(supportSize.height())));
    }

    QRect drawableRect;
    drawableRect.setWidth(supportSize.width() - 2 * marginTopLeft.x());
    drawableRect.setHeight(supportSize.height() - 2 * marginTopLeft.y());
    drawableRect.moveTopLeft(marginTopLeft + refSupportRect.topLeft());

    return drawableRect;
}

QRect MainView::getFigureRect(const QRect &refSupportRect)
{
    QRect rect;
    QRectF relRect;
    QPoint graphRectTopLeft;

    if(information->getGraphSizeSettings().sizeUnit == ZeSizeSettings::CENTIMETER)
        relRect = sheetFigureRectRelative;
    else relRect = imageFigureRectRelative;

    QRect drawableRect = getDrawableRect(refSupportRect);

    graphRectTopLeft.setX(int(relRect.topLeft().x() * double(drawableRect.width())));
    graphRectTopLeft.setY(int(relRect.topLeft().y() * double(drawableRect.height())));

    rect.setWidth(int(relRect.width() * drawableRect.width()));
    rect.setHeight(int(relRect.height() * drawableRect.height()));
    rect.moveTopLeft(drawableRect.topLeft() + graphRectTopLeft);

    return rect;
}

QRect MainView::supportRectFromViewRect(QRect viewRect)
{
    double ratio, targetRatio;

    ratio = double(viewRect.height()) / double(viewRect.width());

    if(information->getGraphSizeSettings().sizeUnit == ZeSizeSettings::CENTIMETER)
        targetRatio = sheetSizeCm.height() / sheetSizeCm.width();
    else targetRatio = double(sheetSizePx.height()) / double(sheetSizePx.width());

    QRect rect;

    if(ratio <= targetRatio)
    {
        rect.setHeight(viewRect.height());
        rect.setWidth(int(double(viewRect.height()) / targetRatio));

        rect.moveTopLeft(QPoint((viewRect.width() - rect.width())/2, 0));
    }
    else
    {
        rect.setHeight(int(double(viewRect.width()) * targetRatio));
        rect.setWidth(viewRect.width());

        rect.moveTopLeft(QPoint(0, (viewRect.height() - rect.height())/2));
    }

    return rect;
}

void MainView::drawSupport()
{ // draws the sheet on an untransformed view
    painter.setBrush(QBrush(information->getGraphSettings().backgroundColor));;

    supportRect = supportRectFromViewRect(painter.viewport());

    painter.drawRect(supportRect);
}

void MainView::drawFigureRect()
{
    figureRect = getFigureRect(supportRect);

    painter.setBrush(Qt::NoBrush);
    pen.setStyle(Qt::DashLine);
    pen.setWidth(1);
    pen.setColor(information->getAxesSettings().x.color);
    painter.setPen(pen);
    painter.drawRect(figureRect);

    pen.setStyle(Qt::SolidLine);
    painter.setPen(pen);
}

void MainView::setGraphRange(GraphRange range)
{

}

void MainView::scaleView(const QRect &refSheetRect)
{
    double newZoom;

    if(information->getGraphSizeSettings().sizeUnit == ZeSizeSettings::CENTIMETER)
    {
        newZoom = double(refSheetRect.width()) / double(targetSupportSizePixels.width());
    }
    else
    {       
        newZoom = double(refSheetRect.width()) / double(sheetSizePx.width());
    }

    if(fabs(newZoom - currentZoom) > 0.001)
    {
        ZeZoomSettings zoomSettings = information->getGraphZoomSettings();

        zoomSettings.zoomingType = ZeZoomSettings::CUSTOM;
        zoomSettings.zoom = newZoom;

        information->setGraphZoomSettings(zoomSettings);
    }

    currentZoom = newZoom;

    double totalScaleFactor = currentZoom * userScalingFactor;


    painter.scale(totalScaleFactor, totalScaleFactor);
}

void MainView::drawGraph()
{
    scaleView(supportRect);

    sheetRectScaled = painter.worldTransform().inverted().mapRect(supportRect);

    figureRectScaled = getFigureRect(sheetRectScaled);

    painter.translate(figureRectScaled.topLeft());

    paint();
}

void MainView::onSizeSettingsChange()
{
    const auto &sizeSettings = information->getGraphSizeSettings();

    this->sheetMarginCm = sizeSettings.cmMargins;
    this->imageMarginPx = sizeSettings.pxMargins;

    userScalingFactor = sizeSettings.scalingFactor;

    figureSizeCm = sizeSettings.cmFigureSize;
    sheetFigureRectRelative.setWidth(figureSizeCm.width() / (sheetSizeCm.width() - 2*sheetMarginCm));
    sheetFigureRectRelative.setHeight(figureSizeCm.height() / (sheetSizeCm.height() - 2*sheetMarginCm));
    constrainFigureRectRel();

    figureSizePx = sizeSettings.pxFigureSize;
    imageFigureRectRelative.setWidth(double(figureSizePx.width()) / double(sheetSizePx.width() - 2*imageMarginPx));
    imageFigureRectRelative.setHeight(double(figureSizePx.height()) / double(sheetSizePx.height() - 2*imageMarginPx));
    constrainFigureRectRel();

    sheetSizeCm = sizeSettings.cmSheetSize;
    sheetSizePx = sizeSettings.pxSheetSize;

    updateFigureSize();
    updateTargetSupportSizePx();

    update();
}

void MainView::mousePressEvent(QMouseEvent *event)
{
    if(topLeft.contains(event->pos()))
        moveType = TOPLEFT_CORNER;
    else if(topRight.contains(event->pos()))
        moveType = TOPRIGHT_CORNER;
    else if(top.contains(event->pos()))
        moveType = TOP_SIDE;
    else if(bottomLeft.contains(event->pos()))
        moveType = BOTTOMLEFT_CORNER;
    else if(bottomRight.contains(event->pos()))
        moveType = BOTTOMRIGHT_CORNER;
    else if(bottom.contains(event->pos()))
        moveType = BOTTOM_SIDE;
    else if(left.contains(event->pos()))
        moveType = LEFT_SIDE;
    else if(right.contains(event->pos()))
        moveType = RIGHT_SIDE;
    else if(figureRect.contains(event->pos()))
        moveType = ALL;
    else moveType = NOTHING;

    if(moveType != NOTHING)
        lastMousePos = event->pos();
}

void MainView::mouseMoveEvent(QMouseEvent *event)
{
    if(moveType == NOTHING)
    {
        if(topLeft.contains(event->pos()) || bottomRight.contains(event->pos()))
            setCursor(Qt::SizeFDiagCursor);
        else if(topRight.contains(event->pos()) || bottomLeft.contains(event->pos()))
            setCursor(Qt::SizeBDiagCursor);
        else if(top.contains(event->pos()) || bottom.contains(event->pos()))
            setCursor(Qt::SizeVerCursor);
        else if(left.contains(event->pos()) || right.contains(event->pos()))
            setCursor(Qt::SizeHorCursor);
        else if(figureRect.contains(event->pos()))
            setCursor(Qt::SizeAllCursor);
        else setCursor(Qt::ArrowCursor);

        lastMousePos = event->pos();
    }
    else
    {
        QPoint dr = event->pos() - lastMousePos;
        lastMousePos = event->pos();

        switch(moveType)
        {
        case TOPLEFT_CORNER:
            figureRect.setTopLeft(figureRect.topLeft() + dr);
            break;
        case TOPRIGHT_CORNER:
            figureRect.setTopRight(figureRect.topRight() + dr);
            break;
        case BOTTOMLEFT_CORNER:
            figureRect.setBottomLeft(figureRect.bottomLeft() + dr);
            break;
        case BOTTOMRIGHT_CORNER:
            figureRect.setBottomRight(figureRect.bottomRight() + dr);
            break;
        case LEFT_SIDE:
            figureRect.setLeft(figureRect.left() + dr.x());
            break;
        case TOP_SIDE:
            figureRect.setTop(figureRect.top() + dr.y());
            break;
        case RIGHT_SIDE:
            figureRect.setRight(figureRect.right() + dr.x());
            break;
        case BOTTOM_SIDE:
            figureRect.setBottom(figureRect.bottom() + dr.y());
            break;
        case ALL:
            figureRect.translate(dr);
            break;
        case NOTHING:
            break;
        }

        QRect drawableRect = getDrawableRect(supportRect);

        QPointF pt = figureRect.topLeft() - drawableRect.topLeft();
        pt.setX(pt.x() / double(drawableRect.width()));
        pt.setY(pt.y() / double(drawableRect.height()));

        if(information->getGraphSizeSettings().sizeUnit == ZeSizeSettings::CENTIMETER)
        {
            sheetFigureRectRelative.setWidth(double(figureRect.width()) / double(drawableRect.width()));
            sheetFigureRectRelative.setHeight(double(figureRect.height()) / double(drawableRect.height()));
            sheetFigureRectRelative.moveTopLeft(pt);
        }
        else
        {
            imageFigureRectRelative.setWidth(double(figureRect.width()) / double(drawableRect.width()));
            imageFigureRectRelative.setHeight(double(figureRect.height()) / double(drawableRect.height()));
            imageFigureRectRelative.moveTopLeft(pt);           
        }

        constrainFigureRectRel();
        updateFigureSize();
        update();
    }

}

void MainView::updateFigureSize()
{
    if(information->getGraphSizeSettings().sizeUnit == ZeSizeSettings::CENTIMETER)
    {
        QSizeF oldFigureSizeCm = figureSizeCm;

        figureSizeCm.setWidth(sheetFigureRectRelative.width() * (sheetSizeCm.width() - 2*sheetMarginCm));
        figureSizeCm.setHeight(sheetFigureRectRelative.height() * (sheetSizeCm.height() - 2*sheetMarginCm));

        if(oldFigureSizeCm != figureSizeCm)
        {
            ZeSizeSettings sizeSettings = information->getGraphSizeSettings();

            sizeSettings.cmFigureSize = figureSizeCm;

            information->setGraphSizeSettings(sizeSettings);
        }
    }
    else
    {
        QSizeF oldFigureSizePx = figureSizePx;

        figureSizePx.setWidth(int(imageFigureRectRelative.width() * double(sheetSizePx.width() - 2*imageMarginPx)));
        figureSizePx.setHeight(int(imageFigureRectRelative.height() * double(sheetSizePx.height() - 2*imageMarginPx)));

        if(oldFigureSizePx != figureSizePx)
        {
            ZeSizeSettings sizeSettings = information->getGraphSizeSettings();

            sizeSettings.pxFigureSize = figureSizePx;

            information->setGraphSizeSettings(sizeSettings);
        }
    }
}

void MainView::constrainFigureRectRel()
{
    if(information->getGraphSizeSettings().sizeUnit == ZeSizeSettings::CENTIMETER)
    {
        if(sheetFigureRectRelative.width() > 1)
            sheetFigureRectRelative.setWidth(1);
        if(sheetFigureRectRelative.height() > 1)
            sheetFigureRectRelative.setHeight(1);

        if(sheetFigureRectRelative.width() < minRelSize)
            sheetFigureRectRelative.setWidth(minRelSize);
        if(sheetFigureRectRelative.height() < minRelSize)
            sheetFigureRectRelative.setHeight(minRelSize);

        if(sheetFigureRectRelative.left() < 0)
            sheetFigureRectRelative.moveLeft(0);
        if(sheetFigureRectRelative.right() > 1)
            sheetFigureRectRelative.moveRight(1);

        if(sheetFigureRectRelative.bottom() > 1)
            sheetFigureRectRelative.moveBottom(1);
        if(sheetFigureRectRelative.top() < 0)
            sheetFigureRectRelative.moveTop(0);      
    }
    else
    {
        if(imageFigureRectRelative.width() > 1)
            imageFigureRectRelative.setWidth(1);
        if(imageFigureRectRelative.height() > 1)
            imageFigureRectRelative.setHeight(1);

        if(imageFigureRectRelative.width() < minRelSize)
            imageFigureRectRelative.setWidth(minRelSize);
        if(imageFigureRectRelative.height() < minRelSize)
            imageFigureRectRelative.setHeight(minRelSize);

        if(imageFigureRectRelative.left() < 0)
            imageFigureRectRelative.moveLeft(0);
        if(imageFigureRectRelative.right() > 1)
            imageFigureRectRelative.moveRight(1);

        if(imageFigureRectRelative.bottom() > 1)
            imageFigureRectRelative.moveBottom(1);
        if(imageFigureRectRelative.top() < 0)
            imageFigureRectRelative.moveTop(0);
    }

}

void MainView::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    setCursor(Qt::ArrowCursor);
    moveType = NOTHING;
}

void MainView::wheelEvent(QWheelEvent *event)
{
    Q_UNUSED(event);
}