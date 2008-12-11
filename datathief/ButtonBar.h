#ifndef __MBUTTONBAR_H
#define __MBUTTONBAR_H

#include "LabelButton.h"
#include <QWidget>
class QPushButton;
class QLineEdit;
class QLabel;
class QCheckBox;
class QWidget;
class QToolButton;

class ButtonBar : public QWidget
{
  Q_OBJECT

 public:
  ButtonBar(QWidget* parent=0);

  const QSize sizeHint();

  QPushButton* set_axes_button;
  QPushButton* remove_button;
  QPushButton* remove_all_button;
  QPushButton* save_button;

  QLineEdit* x0_line_edit;
  QLineEdit* y0_line_edit;
  QLineEdit* x1_line_edit;
  QLineEdit* y1_line_edit;

  QCheckBox* log_x_checkbox;
  QCheckBox* log_y_checkbox;

 public slots:
  void axis_point_set(int, QPointF);
  void point_added();
  void point_removed();
  void all_points_removed();
  void enable_buttons();
  void enable() {setEnabled(true);}

 signals:
  void time_to_save(QPointF, QPointF, bool, bool);
  void color_changed();

 protected:
  void setup_buttons_();
  int npoints_;

  LabelButton* label0_;
  LabelButton* label1_;
  QLabel* instruction0_;
  QLabel* instruction1_;
  LabelButton* instruction2_;

 protected slots:
  void save_button_clicked_();
  void select_axis_point0_color_();
  void select_axis_point1_color_();
  void select_point_color_();
};

#endif
