#ifndef __MLABEL_H
#define __MLABEL_H

#include <QLabel>

class QPaintEvent;

class Label : public QLabel
{
 Q_OBJECT

 public:
  Label(QWidget* parent=0);

 protected slots:
  void paintEvent(QPaintEvent* event);
};

#endif
