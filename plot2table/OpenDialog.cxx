
#include <QLineEdit>
#include <QPushButton>
#include <QCheckBox>
#include <QRadioButton>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QFileDialog>

#include "OpenDialog.h"

OpenDialog::OpenDialog(QString default_filename, QWidget* parent) :
  QDialog(parent),
  _error_mode(None),
  _x_log_scale(0),
  _y_log_scale(0)
{
  QGridLayout* layout = new QGridLayout;
  _filename_edit = new QLineEdit("Filename goes here", this);
  if(!default_filename.isEmpty())
    _filename_edit->setText(default_filename);
  _browse_button = new QPushButton("Browse", this);
  connect(_browse_button, SIGNAL(clicked()),
	  this, SLOT(_browse_for_file()));
  layout->addWidget(_filename_edit, 0, 0, 1, 8);
  layout->addWidget(_browse_button, 0, 8, 1, 2);

  _x_log_checkbox = new QCheckBox("Log scale for x-axis", this);
  _y_log_checkbox = new QCheckBox("Log scale for y-axis", this);
  layout->addWidget(_x_log_checkbox, 2, 5, 1, 5);
  layout->addWidget(_y_log_checkbox, 3, 5, 1, 5);

  QGroupBox* groupbox = new QGroupBox("Error Bars?", this);
  QVBoxLayout* gblayout = new QVBoxLayout;
  _no_errors_radiobutton = new QRadioButton("No Error Bars");
  _no_errors_radiobutton->setChecked(true);
 gblayout->addWidget(_no_errors_radiobutton);
  _symmetric_errors_radiobutton = new QRadioButton("Symmetric Error Bars");
  gblayout->addWidget(_symmetric_errors_radiobutton);
  _asymmetric_errors_radiobutton = new QRadioButton("Asymmetric Error Bars");
  gblayout->addWidget(_asymmetric_errors_radiobutton);
  groupbox->setLayout(gblayout);
  layout->addWidget(groupbox, 1, 0, 3, 5);
  
  QPushButton* ok_button = new QPushButton("OK");
  layout->addWidget(ok_button, 4, 0, 1, 5);
  connect(ok_button, SIGNAL(clicked()),
	  this, SLOT(accept()));
  QPushButton* cancel_button = new QPushButton("Cancel");
  layout->addWidget(cancel_button, 4, 5, 1, 5);
  connect(cancel_button, SIGNAL(clicked()),
	  this, SLOT(reject()));

  setLayout(layout);
}

void OpenDialog::_browse_for_file()
{
  QString filename = QFileDialog::getOpenFileName(this,
						  tr("Open File"), 
						  QDir::currentPath());
  _filename_edit->setText(filename);
  return;
}

void OpenDialog::accept()
{
  _filename = _filename_edit->text();
  _x_log_scale = _x_log_checkbox->isChecked();
  _y_log_scale = _y_log_checkbox->isChecked();
  _error_mode = None;
  if( _symmetric_errors_radiobutton->isChecked() )
    _error_mode = Symmetric;
  else if( _asymmetric_errors_radiobutton->isChecked() )
    _error_mode = Asymmetric;

  QDialog::accept();
  return;
}

ErrorMode OpenDialog::get_error_mode()
{ return _error_mode;}
