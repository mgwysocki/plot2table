#ifndef __MBUTTONBAR_H
#define __MBUTTONBAR_H

#include <QWidget>
class QPushButton;
class QLineEdit;
class QLabel;
class QCheckBox;
class QWidget;

class ButtonBar : public QWidget
{
  Q_OBJECT

 public:
  ButtonBar(QWidget* parent=0);

  const QSize sizeHint();

  QPushButton* open_button;
  QPushButton* set_axes_button;
  QPushButton* zoom_in_button;
  QPushButton* zoom_out_button;
  QPushButton* normal_size_button;
  QPushButton* fit_to_screen_button;
  QPushButton* remove_button;
  QPushButton* remove_all_button;
  QPushButton* save_button;

  QLineEdit* x0_line_edit;
  QLineEdit* y0_line_edit;
  QLineEdit* x1_line_edit;
  QLineEdit* y1_line_edit;

  QCheckBox* log_x_checkbox;
  QCheckBox* log_y_checkbox;

  QPushButton* point_color_button;

 public slots:
  void point0_set();
  void point1_set();
  void point_added();
  void point_removed();
  void all_points_removed();
  void enable_buttons();

 signals:
  void time_to_save(double, double, double, double, bool, bool);
  void color_changed(QColor);

 protected:
  void _setup_buttons();
  int _npoints;

  QLabel* _label0;
  QLabel* _label1;
  QLabel* _instruction0;
  QLabel* _instruction1;
  QLabel* _instruction2;

  QColor _point_color;

 protected slots:
  void _save_button_clicked();
  void _select_color();
};

#endif
