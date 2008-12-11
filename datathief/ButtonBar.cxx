
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
#include <QToolButton>

#include "ButtonBar.h"
#include "PointList.h"

#include <iostream>
using namespace std;

ButtonBar::ButtonBar(QWidget* parent) :
  QWidget::QWidget(parent),
  npoints_(0)
{
  setup_buttons_();

  QSizePolicy size_policy(QSizePolicy::Preferred, 
			  QSizePolicy::Preferred);
  setSizePolicy( size_policy );

  connect( remove_button, SIGNAL(clicked()),
	   this, SLOT(point_removed()) );

  connect( remove_all_button, SIGNAL(clicked()),
	   this, SLOT(all_points_removed()) );

  connect( save_button, SIGNAL(clicked()),
	   this, SLOT(save_button_clicked_()) );
}

const QSize ButtonBar::sizeHint()
{
  return QSize(200, 800);
}

void ButtonBar::axis_point_set(int i, QPointF)
{
  if(i==0) {
    instruction0_->hide();
    label0_->show();
    x0_line_edit->show();
    y0_line_edit->show();
    instruction1_->show();
  } else {
    instruction1_->hide();
    label1_->show();
    x1_line_edit->show();
    y1_line_edit->show();
    instruction2_->setText("Click to add data points...");
    instruction2_->show();
  }
  return;
}

void ButtonBar::point_added()
{
  if( !remove_button->isEnabled() ) {
    remove_button->setDisabled(false);
    remove_all_button->setDisabled(false);
  }

  npoints_++;
  QString text = QString("Data points: %1").arg(npoints_);
  instruction2_->setText(text);
  return;
}

void ButtonBar::point_removed()
{
  npoints_--;
  if(npoints_>0) {
    QString text = QString("Data points: %1").arg(npoints_);
    instruction2_->setText(text);

  } else {
    instruction2_->setText("Click to add data points...");
    if( remove_button->isEnabled() )
      remove_button->setDisabled(true);
  }
  return;
}

void ButtonBar::all_points_removed()
{
  npoints_=0;
  instruction2_->setText("Click to add data points...");
  if( remove_button->isEnabled() )
    remove_button->setDisabled(true);
  return;
}

void ButtonBar::enable_buttons()
{
//   zoom_in_button->setDisabled(false);
//   zoom_out_button->setDisabled(false);
//   normal_size_button->setDisabled(false);
//   fit_to_screen_button->setDisabled(false);
  save_button->setDisabled(false);
  return;
}

void ButtonBar::setup_buttons_()
{
  QVBoxLayout* vlayout = new QVBoxLayout;

  // Define the axes...
  //
  QGroupBox* groupbox = new QGroupBox("Axis Mapping", this);
  vlayout->addWidget(groupbox,0);

  QGridLayout* layout = new QGridLayout;

  instruction0_ = new QLabel("<font color=blue>Click to set first axis point...</font>");
  layout->addWidget(instruction0_, 0, 0, 1, 3);

  //label0_ = new LabelButton("<font color=blue><big>(x<sub>0</sub>, y<sub>0</sub>) =<big></font>");
  label0_ = new LabelButton("<big>(x<sub>0</sub>, y<sub>0</sub>) =<big>");
  connect( label0_, SIGNAL(clicked()),
	   this, SLOT(select_axis_point0_color_()));
  QPalette pal = label0_->palette();
  pal.setColor(QPalette::WindowText, QColor("blue"));
  label0_->setPalette(pal);
  layout->addWidget(label0_, 1, 0);
  label0_->hide();

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

  instruction1_ = new QLabel("<font color=#00bf00>Click to set second axis point...</font>");
  layout->addWidget(instruction1_, 2, 0, 1, 3);
  instruction1_->hide();

  label1_ = new LabelButton("<big>(x<sub>1</sub>, y<sub>1</sub>) =<big>");
  connect( label1_, SIGNAL(clicked()),
	   this, SLOT(select_axis_point1_color_()));
  pal = label1_->palette();
  pal.setColor(QPalette::WindowText, QColor("#00bf00"));
  label1_->setPalette(pal);
  layout->addWidget(label1_, 3, 0);
  label1_->hide();

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
  
  instruction2_ = new LabelButton(" ");
  instruction2_->setAlignment(Qt::AlignHCenter);
  connect( instruction2_, SIGNAL(clicked()),
	   this, SLOT(select_point_color_()));
  pal = instruction2_->palette();
  pal.setColor(QPalette::WindowText, QColor("red"));
  instruction2_->setPalette(pal);
  vlayout->addWidget(instruction2_);

  remove_button = new QPushButton("Remove Last Point", this);
  remove_button->setDisabled(true);
  vlayout->addWidget(remove_button,0);

  remove_all_button = new QPushButton("Remove All Points", this);
  remove_all_button->setDisabled(true);
  vlayout->addWidget(remove_all_button,0);

  vlayout->addSpacing(30);
  save_button = new QPushButton("Save Points", this);
  save_button->setDisabled(true);
  vlayout->addWidget(save_button,0);

  vlayout->addStretch(1);
  setLayout(vlayout);
  return;
}

void ButtonBar::save_button_clicked_()
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

void ButtonBar::select_axis_point0_color_()
{
  PointList* pl = PointList::instance();
  QColor c = QColorDialog::getColor( pl->get_axis_point0_color() );
  if( c != pl->get_axis_point0_color() ) {
    pl->set_axis_point0_color(c);
    QPalette pal = label0_->palette();
    pal.setColor(QPalette::WindowText, c);
    label0_->setPalette(pal);
    emit color_changed();
  }
  return;
}

void ButtonBar::select_axis_point1_color_()
{
  PointList* pl = PointList::instance();
  QColor c = QColorDialog::getColor( pl->get_axis_point1_color() );
  if( c != pl->get_axis_point1_color() ) {
    pl->set_axis_point1_color(c);
    QPalette pal = label1_->palette();
    pal.setColor(QPalette::WindowText, c);
    label1_->setPalette(pal);
    emit color_changed();
  }
  return;
}


void ButtonBar::select_point_color_()
{
  PointList* pl = PointList::instance();
  QColor c = QColorDialog::getColor( pl->get_point_color() );
  if( c != pl->get_point_color() ) {
    pl->set_point_color(c);
    QPalette pal = instruction2_->palette();
    pal.setColor(QPalette::WindowText, c);
    instruction2_->setPalette(pal);
    emit color_changed();
  }
  return;
}
