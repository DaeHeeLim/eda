#ifndef TEXTITEM_H
#define TEXTITEM_H

#include <QGraphicsSimpleTextItem>
#include "global.h"

class PCBScene;

class TextItem : public QGraphicsSimpleTextItem
{

public:
    TextItem(PCBScene *scene,const QString &text, QGraphicsItem *parent = nullptr);
    ~TextItem();

    PCBScene *pcbScene;

    void selected();
    void unselected();

    enum {
        Type = Item::TextItemType
    };

    int type() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
//    void setText(const QString &text);
    void followRotation();

//    void createCommandAdd();
//    void createCommandRemove();
//    void createCommandMove();

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);

    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void keyPressEvent(QKeyEvent *event);

};

#endif // TEXTITEM_H
