#include "roundedrectitem.h"
#include "roundedrectitemcontrolpoint.h"

//#include "shapeitemaddcommand.h"
//#include "shapeitemremovecommand.h"
//#include "shapeitemmovecommand.h"
//#include "shapeitemchanggeometryecommand.h"

#include "pcbscene.h"

RoundedRectItem::RoundedRectItem(PCBScene *scene, QGraphicsItem *parent)
    :QGraphicsRoundedRectItem(parent),
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
    metadata["fillStyle"] = FillStyle::NoBrush;
    metadata["color"] = schColor.display;

}

RoundedRectItem::RoundedRectItem(PCBScene *scene, const json &j, QGraphicsItem *parent)
    :QGraphicsRoundedRectItem(parent),
      AbstractShapeItem(scene,this)
{

}

RoundedRectItem::~RoundedRectItem()
{

}

void RoundedRectItem::selected()
{
    setPen(selectedPen);
//    if(is2DLine || pcbScene->isPartEditorMode){
        drawControlPoints();
//    }
}

void RoundedRectItem::unselected()
{
    setPen(normalPen);
//    if(is2DLine || pcbScene->isPartEditorMode){
        for(QGraphicsItem *item:childItems()) {
            pcbScene->removeItem(item);
            delete item;
        }
//    }
}

int RoundedRectItem::type() const
{
    return Type;
}

//void RoundedRectItem::createCommandAdd()
//{
//    pcbScene->pushCommand(new ShapeItemAddCommand(this),true);
//    metadata["geometry"]["rect"] = rect();
//    metadata["geometry"]["xRadius"] = xRadius();
//    metadata["geometry"]["yRadius"] = yRadius();
//    metadata["geometry"]["mode"] = mode();
//}

//void RoundedRectItem::createCommandRemove()
//{
//    pcbScene->pushCommand(new ShapeItemRemoveCommand(this),true);

//}

//void RoundedRectItem::createCommandMove()
//{
//    pcbScene->pushCommand(new ShapeItemMoveCommand(this));
//}

//void RoundedRectItem::createCommandChangeGeometry()
//{
//    pcbScene->pushCommand(new ShapeItemChangeGeometryCommand(this));
//}

void RoundedRectItem::setGeometry(const json &geometry)
{
    setRect(geometry["rect"]);
}

QVariant RoundedRectItem::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
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


void RoundedRectItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if(isMoved()){
//        createCommandMove();
    }
    QGraphicsRoundedRectItem::mouseReleaseEvent(mouseEvent);
}

void RoundedRectItem::keyPressEvent(QKeyEvent *keyEvent)
{

}

void RoundedRectItem::drawControlPoints()
{
    const QRectF &metaRect = rect();
    for(int i=0;i< 4;i++){
        RoundedRectItemControlPoint *controlPoint = new RoundedRectItemControlPoint(this);
        controlPoint->setPos(flagToPoint(i,metaRect));
        controlPoint->flag = i;
    }
}

void RoundedRectItem::initial()
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
