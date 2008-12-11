#ifndef __LABELBUTTON_H
#define __LABELBUTTON_H

#include <QLabel>

class LabelButton : public QLabel
{
Q_OBJECT
  
 public:
  LabelButton( QWidget* parent = 0, Qt::WindowFlags f = 0);
  LabelButton( const QString &text, QWidget* parent = 0, Qt::WindowFlags f = 0 );

 signals:
  void clicked();
  
 protected:
  virtual void mousePressEvent( QMouseEvent* event );
  virtual void mouseReleaseEvent( QMouseEvent* event );

  bool clicked_;
};

#endif // __LABELBUTTON_H
