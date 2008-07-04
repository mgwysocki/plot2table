#ifndef __MZOOMEDVIEW_H
#define __MZOOMEDVIEW_H

#include <QGraphicsView>
#include <QLabel>

//-------------------------------------------------------------------
// The QLabel is our overlay that paints on the zoomed QGraphicsScene.
//
class MZoomedOverlay : public QLabel
{
 Q_OBJECT

 public:
  MZoomedOverlay(QWidget* parent=0);

 protected:
  void paintEvent(QPaintEvent*);
};


//---------------------------------------------------------------------
// The GraphicsView has to resize the overlay whenever it's resized.
//
class ZoomedView : public QGraphicsView
{
 Q_OBJECT

 public:
  ZoomedView(QWidget* parent=0);

 protected:
  MZoomedOverlay* _overlay;
  void resizeEvent(QResizeEvent*);
};

#endif
