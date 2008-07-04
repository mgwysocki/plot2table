#ifndef __MGRAPHICSVIEW_H
#define __MGRAPHICSVIEW_H

#include <QGraphicsView>
#include <QLabel>

//-------------------------------------------------------------------
// The QLabel is our overlay that paints on the QGraphicsView.
//
class MCursorOverlay : public QLabel
{
 Q_OBJECT

 public:
  MCursorOverlay(QWidget* parent=0);

  bool cursor_in;
  QPoint cursor_position;

 signals:
  void movescene(QPoint);

 protected:
  void mouseMoveEvent(QMouseEvent*);
  void enterEvent(QEvent*);
  void leaveEvent(QEvent*);
  void paintEvent(QPaintEvent*);
};


//---------------------------------------------------------------------
// The GraphicsView has to resize the overlay whenever it's resized.
//
class GraphicsView : public QGraphicsView
{
 Q_OBJECT

 public:
  GraphicsView(QWidget* parent=0);
  MCursorOverlay _overlay;

 protected:
  void resizeEvent(QResizeEvent*);
};

#endif
