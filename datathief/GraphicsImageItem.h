#ifndef __MGRAPHICSIMAGEITEM_H
#define __MGRAPHICSIMAGEITEM_H

#include <QGraphicsPixmapItem>

class GraphicsImageItem : public QGraphicsPixmapItem
{
 public:
  GraphicsImageItem(QGraphicsItem* parent = 0,
		    QGraphicsScene* scene = 0);
};

#endif
