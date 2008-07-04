#ifndef __OPENDIALOG_H
#define __OPENDIALOG_H

#include <QDialog>

#include "ErrorModes.h"

class QLineEdit;
class QPushButton;
class QCheckBox;
class QRadioButton;

class OpenDialog : public QDialog
{
  Q_OBJECT

 public:
  OpenDialog(QString default_filename="", QWidget* parent=0);
  
  QString get_filename() {return _filename;}
  ErrorMode get_error_bar_type() {return _error_mode;}
  bool is_x_log_scale() {return _x_log_scale;}
  bool is_y_log_scale() {return _y_log_scale;}
  ErrorMode get_error_mode();

 public slots:
  void accept();

 protected slots:
  void _browse_for_file();

 protected:
  QLineEdit* _filename_edit;
  QPushButton* _browse_button;
  QCheckBox* _x_log_checkbox;
  QCheckBox* _y_log_checkbox;
  QRadioButton* _no_errors_radiobutton;
  QRadioButton* _symmetric_errors_radiobutton;
  QRadioButton* _asymmetric_errors_radiobutton;

  QString _filename;
  ErrorMode _error_mode;
  bool _x_log_scale;
  bool _y_log_scale;
  
};

#endif
