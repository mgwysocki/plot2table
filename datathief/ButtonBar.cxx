
#include <QGroupBox>
#include <QPushButton>
#include <QLineEdit>
#include <QSizePolicy>
#include <QDoubleValidator>
#include <QLabel>
#include <QGridLayout>
#include <QIcon>
#include <QMessageBox>
#include <QCheckBox>
#include <QColorDialog>

#include "ButtonBar.h"

#include <iostream>
using namespace std;

ButtonBar::ButtonBar(QWidget* parent) :
  QWidget::QWidget(parent),
  _npoints(0),
  _point_color(Qt::red)
{
  _setup_buttons();

  QSizePolicy size_policy(QSizePolicy::Preferred, 
			  QSizePolicy::Preferred);
  setSizePolicy( size_policy );

  connect( remove_button, SIGNAL(clicked()),
	   this, SLOT(point_removed()) );

  connect( remove_all_button, SIGNAL(clicked()),
	   this, SLOT(all_points_removed()) );

  connect( save_button, SIGNAL(clicked()),
	   this, SLOT(_save_button_clicked()) );

  connect( point_color_button, SIGNAL(clicked()),
	   this, SLOT(_select_color()));
}

const QSize ButtonBar::sizeHint()
{
  return QSize(200, 800);
}

void ButtonBar::axis_point_set(int i, QPointF)
{
  if(i==0) {
    _instruction0->hide();
    _label0->show();
    x0_line_edit->show();
    y0_line_edit->show();
    _instruction1->show();
  } else {
    _instruction1->hide();
    _label1->show();
    x1_line_edit->show();
    y1_line_edit->show();
    _instruction2->show();
  }
  return;
}

void ButtonBar::point_added()
{
  if( !remove_button->isEnabled() ) {
    remove_button->setDisabled(false);
    remove_all_button->setDisabled(false);
  }

  _npoints++;
  QString text = QString("<font color=red>Data points: %1</font>").arg(_npoints);
  _instruction2->setText(text);
  return;
}

void ButtonBar::point_removed()
{
  _npoints--;
  if(_npoints>0) {
    QString text = QString("<font color=red>Data points: %1</font>").arg(_npoints);
    _instruction2->setText(text);

  } else {
    _instruction2->setText("<font color=red>Click to add data points...</font>");
    if( remove_button->isEnabled() )
      remove_button->setDisabled(true);
  }
  return;
}

void ButtonBar::all_points_removed()
{
  _npoints=0;
  _instruction2->setText("<font color=red>Click to add data points...</font>");
  if( remove_button->isEnabled() )
    remove_button->setDisabled(true);
  return;
}

void ButtonBar::enable_buttons()
{
  zoom_in_button->setDisabled(false);
  zoom_out_button->setDisabled(false);
  normal_size_button->setDisabled(false);
  fit_to_screen_button->setDisabled(false);
  save_button->setDisabled(false);
  return;
}

void ButtonBar::_setup_buttons()
{
  QVBoxLayout* vlayout = new QVBoxLayout;

  open_button = new QPushButton("Open Image", this);
  vlayout->addWidget(open_button);

  // IMAGE CONTROL
  //
  QGroupBox* groupbox = new QGroupBox("Image Controls", this);
  vlayout->addWidget(groupbox);

  QGridLayout *layout = new QGridLayout;
  QIcon zoominicon;
  zoominicon.addFile(":/icons/zoomin.png");
  zoom_in_button = new QPushButton(zoominicon, "");
  zoom_in_button->setIconSize(QSize(32,32));
  zoom_in_button->setMinimumSize(QSize(64,64));
  zoom_in_button->setMaximumSize(QSize(64,64));
  zoom_in_button->setDisabled(true);
  layout->addWidget(zoom_in_button, 0, 0);

  QIcon zoomouticon;
  zoomouticon.addFile(":/icons/zoomout.png");
  zoom_out_button = new QPushButton(zoomouticon, "");
  zoom_out_button->setIconSize(QSize(32,32));
  zoom_out_button->setMinimumSize(QSize(64,64));
  zoom_out_button->setMaximumSize(QSize(64,64));
  zoom_out_button->setDisabled(true);
  layout->addWidget(zoom_out_button, 0, 1);

  normal_size_button = new QPushButton("Actual\nSize");
  //normal_size_button->setIconSize(QSize(32,32));
  normal_size_button->setMinimumSize(QSize(64,64));
  normal_size_button->setMaximumSize(QSize(64,64));
  normal_size_button->setDisabled(true);
  layout->addWidget(normal_size_button, 1, 0);

  QIcon fiticon;
  fiticon.addFile(":/icons/fittowindow.png");
  fit_to_screen_button = new QPushButton(fiticon, "");
  fit_to_screen_button->setIconSize(QSize(32,32));
  fit_to_screen_button->setMinimumSize(QSize(64,64));
  fit_to_screen_button->setMaximumSize(QSize(64,64));
  fit_to_screen_button->setDisabled(true);
  layout->addWidget(fit_to_screen_button, 1, 1);
  groupbox->setLayout(layout);


  // Define the axes...
  //
  groupbox = new QGroupBox("Axis Mapping", this);
  vlayout->addWidget(groupbox,0);

  layout = new QGridLayout;

  _instruction0 = new QLabel("<font color=blue>Click to set first axis point...</font>");
  layout->addWidget(_instruction0, 0, 0, 1, 3);

  _label0 = new QLabel("<font color=blue><big>(x<sub>0</sub>, y<sub>0</sub>) =<big></font>");
  layout->addWidget(_label0, 1, 0);
  _label0->hide();

  x0_line_edit = new QLineEdit;
  x0_line_edit->setValidator( new QDoubleValidator(x0_line_edit) );
  x0_line_edit->setMaximumWidth(50);
  layout->addWidget(x0_line_edit, 1, 1);
  x0_line_edit->hide();

  y0_line_edit = new QLineEdit("0.0");
  y0_line_edit->setValidator( new QDoubleValidator(y0_line_edit) );
  y0_line_edit->setMaximumWidth(50);
  layout->addWidget(y0_line_edit, 1, 2);
  y0_line_edit->hide();

  _instruction1 = new QLabel("<font color=#00bf00>Click to set second axis point...</font>");
  layout->addWidget(_instruction1, 2, 0, 1, 3);
  _instruction1->hide();

  _label1 = new QLabel("<font color=#00bf00><big>(x<sub>1</sub>, y<sub>1</sub>) =<big></font>");
  layout->addWidget(_label1, 3, 0);
  _label1->hide();

  x1_line_edit = new QLineEdit("0.0");
  x1_line_edit->setValidator( new QDoubleValidator(x1_line_edit) );
  x1_line_edit->setMaximumWidth(50);
  layout->addWidget(x1_line_edit, 3, 1);
  x1_line_edit->hide();

  y1_line_edit = new QLineEdit;
  y1_line_edit->setValidator( new QDoubleValidator(y1_line_edit) );
  y1_line_edit->setMaximumWidth(50);
  layout->addWidget(y1_line_edit, 3, 2);
  y1_line_edit->hide();

  log_x_checkbox = new QCheckBox(" x-axis = log scale");
  layout->addWidget(log_x_checkbox, 4, 0, 1, 3);

  log_y_checkbox = new QCheckBox(" y-axis = log scale");
  layout->addWidget(log_y_checkbox, 5, 0, 1, 3);

  layout->setColumnStretch(0,0);
  groupbox->setLayout(layout);
  
  //vlayout->addSpacing(50);
  _instruction2 = new QLabel("<font color=red>Click to add data points...</font>");
  vlayout->addWidget(_instruction2);
  _instruction2->hide();
  //vlayout->addSpacing(50);

  remove_button = new QPushButton("Remove Last Point", this);
  remove_button->setDisabled(true);
  vlayout->addWidget(remove_button,0);

  remove_all_button = new QPushButton("Remove All Points", this);
  remove_all_button->setDisabled(true);
  vlayout->addWidget(remove_all_button,0);

  save_button = new QPushButton("Save Points", this);
  save_button->setDisabled(true);
  vlayout->addWidget(save_button,0);

  point_color_button = new QPushButton("Point Color", this);
  vlayout->addWidget(point_color_button);
  vlayout->addStretch(1);

  setLayout(vlayout);
  return;
}

void ButtonBar::_save_button_clicked()
{
  if( x0_line_edit->hasAcceptableInput() &&
      y0_line_edit->hasAcceptableInput() &&
      x1_line_edit->hasAcceptableInput() &&
      y1_line_edit->hasAcceptableInput() ){

    double x0 = x0_line_edit->text().toDouble();
    double y0 = y0_line_edit->text().toDouble();
    double x1 = x1_line_edit->text().toDouble();
    double y1 = y1_line_edit->text().toDouble();
    bool logx = log_x_checkbox->isChecked();
    bool logy = log_y_checkbox->isChecked();

    emit time_to_save(QPointF(x0,y0), QPointF(x1,y1), logx, logy);

  } else if( x1_line_edit->isVisible() ) {
    QMessageBox::warning(this, 
			 tr("Could Not Save"),
			 tr("You must enter the coordinates of the setup points you chose!"),
			 tr("&OK"));
  } else {
    QMessageBox::warning(this, 
			 tr("Could Not Save"),
			 tr("You must use two points to define the coordinate system!"),
			 tr("&OK"));
  }
  return;
}

void ButtonBar::_select_color()
{
  QColor c = QColorDialog::getColor ( _point_color );
  if( c != _point_color ) {
    emit color_changed(c);
    _point_color = c;
  }
  return;
}
