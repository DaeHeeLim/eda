#include "arcitem.h"
#include "arcitemcontrolpoint.h"

//#include "shapeitemaddcommand.h"
//#include "shapeitemremovecommand.h"
//#include "shapeitemmovecommand.h"
//#include "shapeitemchanggeometryecommand.h"


#include "pcbscene.h"

ArcItem::ArcItem(PCBScene *scene, QGraphicsItem *parent)
    :QGraphicsPathItem(parent),
      AbstractShapeItem(scene,this)
{
    initial();
    if(parent && (parent->type() > Item::PackageItemType)){
        isMemberOfPackage = true;
    }else{
        setFlags(ItemIsMovable | ItemIsSelectable | ItemIsFocusable);
    }

    metadata["type"] = Type;
    metadata["lineStyle"] = LineStyle::SolidLine;
    metadata["lineWidth"] = LineWidth::Normal;
    metadata["color"] = schColor.display;

}

ArcItem::ArcItem(PCBScene *scene, const json &j, QGraphicsItem *parent)
    :QGraphicsPathItem(parent),
      AbstractShapeItem(scene,this)
{

}

ArcItem::~ArcItem()
{
}

void ArcItem::selected()
{
    setPen(selectedPen);
    drawControlPoints();
}

void ArcItem::unselected()
{
    setPen(normalPen);
    foreach (QGraphicsItem *item, childItems()) {
        pcbScene->removeItem(item);
        delete item;
    }
}

int ArcItem::type() const
{
    return Type;
}

QRectF ArcItem::getRect() const
{
    return metaRect;
}

void ArcItem::setRect(const QRectF &rect)
{
    metaRect = rect;
}

qreal ArcItem::getStartAngle() const
{
    return metaStartAngle;
}

void ArcItem::setStartAngle(qreal angle)
{
    metaStartAngle = angle;
}

qreal ArcItem::getSweepLength() const
{
    return metaSweepLength;
}

void ArcItem::setSweepLength(qreal length)
{
    metaSweepLength = length;
}

//void ArcItem::createCommandAdd()
//{
//    pcbScene->pushCommand(new ShapeItemAddCommand(this),true);
//    metadata["geometry"]["rect"] = metaRect;
//    metadata["geometry"]["startAngle"] = metaStartAngle;
//    metadata["geometry"]["sweepLength"] = metaSweepLength;
//}

//void ArcItem::createCommandRemove()
//{
//    pcbScene->pushCommand(new ShapeItemRemoveCommand(this),true);

//}

//void ArcItem::createCommandMove()
//{
//    pcbScene->pushCommand(new ShapeItemMoveCommand(this));
//}

//void ArcItem::createCommandChangeGeometry()
//{
//    pcbScene->pushCommand(new ShapeItemChangeGeometryCommand(this));
//}

void ArcItem::setGeometry(const json &geometry)
{
    QPainterPath tmpPath;
    metaRect = geometry["rect"];
    metaStartAngle = geometry["startAngle"];
    metaSweepLength = geometry["sweepLength"];

    tmpPath.arcMoveTo(metaRect,metaStartAngle);
    tmpPath.arcTo(metaRect, metaStartAngle, metaSweepLength);
    setPath(tmpPath);
}

QVariant ArcItem::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    switch (change)
    {
    case ItemSelectedHasChanged:
        value.toBool() ? selected() : unselected();
        break;
    default:
        break;
    }
    return QGraphicsItem::itemChange(change, value);
}

void ArcItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if(isMoved()){
//        createCommandMove();
    }
    QGraphicsPathItem::mouseReleaseEvent(mouseEvent);
}

void ArcItem::drawControlPoints()
{
    const QPainterPath &p = path();

    ArcItemControlPoint *controlPointStart = new ArcItemControlPoint(this);
    controlPointStart->setPos(p.elementAt(0));
    controlPointStart->flag = 0;

    ArcItemControlPoint *controlPointEnd = new ArcItemControlPoint(this);
    controlPointEnd->setPos( p.elementAt(p.elementCount() -1));
    controlPointEnd->flag = p.elementCount() -1;
}

void ArcItem::initial()
{
    normalPen   = QPen(QColor(schColor.display), 0, Qt::SolidLine, Qt::SquareCap, Qt::MiterJoin);
    selectedPen = QPen(QColor(schColor.selection), 0, Qt::SolidLine, Qt::SquareCap, Qt::MiterJoin);
    drawingPen  = QPen(QColor(schColor.drawing), 0, Qt::SolidLine, Qt::SquareCap, Qt::MiterJoin);

//    setAcceptHoverEvents(true);

    if (pcbScene->sceneState == FSM::SelectState /*|| pcbScene->sceneState == FSM::PartItemState*/)
    {
        //TODO 评估是否有必要每次setpen时重绘
        setPen(normalPen);
    }
    else
    {
        setPen(drawingPen);
    }

    setZValue(ZValue::BaseItemZValue);
}
