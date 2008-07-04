
#include "GraphicsImageItem.h"

#include <iostream>

GraphicsImageItem::GraphicsImageItem(QGraphicsItem* parent, QGraphicsScene* scene):
  QGraphicsPixmapItem(parent, scene)
{
  setShapeMode(QGraphicsPixmapItem::BoundingRectShape);
  setTransformationMode(Qt::SmoothTransformation);
}
