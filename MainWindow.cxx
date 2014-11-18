#include <QtWidgets>

#include "ZoomedView.h"
#include "GraphicsScene.h"
#include "OpenDialog.h"
#include "ButtonBar.h"
#include "MainWindow.h"
#include "GraphicsView.h"
#include "PointList.h"

#include <iostream>

MainWindow::MainWindow() :
  QMainWindow::QMainWindow()
{
  button_bar_ = new ButtonBar;
  button_bar_->setEnabled(false);
  scene_ = new GraphicsScene;
  gview_ = new GraphicsView;
  //gview_->setDragMode(QGraphicsView::ScrollHandDrag);
  gview_->setScene(scene_);
  gview_->setBackgroundRole(QPalette::Dark);

  zoomview_ = new ZoomedView;
  zoomview_->setScene(scene_);
  zoomview_->setInteractive(false);
  zoomview_->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  zoomview_->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  zoomview_->scale(10., 10.);  // 10x zoomed-in view

  // Ugly, but we need this for the scene to send the cursor position
  //  updates to the zoomed view
  scene_->set_zoomed_view(zoomview_);

  QWidget* w = new QWidget;
  QVBoxLayout* vlayout = new QVBoxLayout;
  vlayout->addWidget(button_bar_, 0);
  vlayout->addWidget(zoomview_, 1);
  vlayout->setContentsMargins(0,0,0,0);
  w->setLayout(vlayout);

  QSplitter* splitter = new QSplitter;
  splitter->addWidget(w);
  splitter->addWidget(gview_);

  QList<int> sizes;
  sizes.append(200);
  sizes.append(800);
  splitter->setSizes(sizes);
  splitter->setChildrenCollapsible(false);
  
  splitter->setStretchFactor(0, 0);
  splitter->setStretchFactor(1, 1);
  this->setCentralWidget(splitter);


  connect(PointList::instance(), SIGNAL(axis_point_set(int, QPointF)),
          button_bar_,             SLOT(axis_point_set(int, QPointF)));
  connect(PointList::instance(), SIGNAL(axis_point_set(int, QPointF)),
          scene_,                  SLOT(axis_point_set(int, QPointF)));
  connect(PointList::instance(), SIGNAL(data_point_added(QPointF)),
          button_bar_,             SLOT(point_added()));
  connect(PointList::instance(), SIGNAL(data_point_added(QPointF)),
          scene_,                  SLOT(add_point(QPointF)));
  connect(PointList::instance(), SIGNAL(low_error_added(QPointF)),
          scene_,                  SLOT(add_low_error(QPointF)));
  connect(PointList::instance(), SIGNAL(high_error_added(QPointF)),
          scene_,                  SLOT(add_high_error(QPointF)));
  connect(button_bar_->remove_button, SIGNAL(clicked()), 
          scene_, SLOT(remove_last_point()));
  connect(button_bar_->remove_all_button, SIGNAL(clicked()), 
          scene_, SLOT(remove_all_points()));

  connect(button_bar_, SIGNAL(color_changed()), 
          scene_, SLOT(point_color_changed()));

  connect(button_bar_, SIGNAL(time_to_save(QPointF, QPointF, bool, bool)), 
	  PointList::instance(), SLOT(save_points(QPointF, QPointF, bool, bool)));

  connect(this, SIGNAL(file_opened()),
          button_bar_, SLOT(enable()));
  connect(scene_, SIGNAL(image_doesnt_fit(QSize)),
          this, SLOT(resize_to_image(QSize)));

  createActions();
  createMenus();
  setWindowTitle(tr("Plot2Table"));

  QToolBar* toolbar = new QToolBar();
  toolbar->addAction(open_act_);
  toolbar->addSeparator();
  toolbar->addAction(zoom_in_act_);
  toolbar->addAction(zoom_out_act_);
  toolbar->addAction(normal_size_act_);
  toolbar->addAction(fit_to_window_act_);
  toolbar->setAllowedAreas(Qt::RightToolBarArea);
  toolbar->setFloatable(false);
  //toolbar->setToolButtonStyle( Qt::ToolButtonTextUnderIcon );
  toolbar->setIconSize( QSize(24,24) );
  //toolbar->setMovable(false);
  addToolBar( toolbar );

  // Set the maximum window size to the desktop size minus a 50 pixel border
  QSize maxsize(QApplication::desktop()->size());
  maxsize += QSize(-50, -50);
  setMaximumSize(maxsize);

  // Set it to some default size
  resize(800, 600);

  // Testing...
  std::cout << "Current directory:\n" 
	    << qPrintable(QDir::current().path()) << std::endl;
}


void MainWindow::scale_image(double factor)
{
  std::cout << "MainWindow::scale_image(" << factor << ")" << std::endl;
  gview_->scale(factor, factor);
  scene_->set_scale_factor( gview_->matrix().m11() );

  return;
}


void MainWindow::zoom_in()
{
  scale_image(1.25);
}

void MainWindow::zoom_out()
{
  scale_image(0.8);
}

void MainWindow::normal_size()
{
  gview_->resetMatrix();
  scene_->set_scale_factor( gview_->matrix().m11() );
}

void MainWindow::fit_to_window()
{
  float width_scale_factor = width() / (float) image_pixmap_.width();
  float height_scale_factor = height() / (float) image_pixmap_.height();
  if(width_scale_factor < height_scale_factor)  scale_image(width_scale_factor);
  else                                          scale_image(height_scale_factor);
  return;
}

void MainWindow::resize_to_image(QSize image_size)
{
  std::cout << "MainWindow::resize_to_image()" << std::endl;

  QSize other = size() - gview_->size();
  QSize newsize = other + image_size + QSize(5,5);

  std::cout << "window size: " << width() << "x" << height() << std::endl;
  std::cout << "zoomview_ size: " << zoomview_->size().width() << "x" << zoomview_->size().height() << std::endl;
  QScrollArea* sa = (QScrollArea*) zoomview_->viewport();
  std::cout << "scroll area size: " << sa->width() << "x" << sa->height() << std::endl;
  std::cout << "buttonbar size: " << button_bar_->width() << "x" << button_bar_->height() << std::endl;
  std::cout << "image size: " << image_size.width() << "x" << image_size.height() << std::endl;

  resize(newsize);
  return;
}

void MainWindow::open(QString default_filename)
{
  OpenDialog od(default_filename, this);
  int value = od.exec();
  if(value == QDialog::Rejected)
    return;

  std::cout << "Filename: " << qPrintable(od.get_filename()) << std::endl;
  std::cout << "Log x: " << od.is_x_log_scale() << std::endl;
  std::cout << "Log y: " << od.is_y_log_scale() << std::endl;

  QImage image( od.get_filename() );
  image_pixmap_ = QPixmap::fromImage(image);
  scene_->set_image_pixmap( image_pixmap_ );
  gview_->setSceneRect( scene_->get_image_rect() );

  button_bar_->enable_buttons();

  button_bar_->log_x_checkbox->setChecked( od.is_x_log_scale() );
  button_bar_->log_y_checkbox->setChecked( od.is_y_log_scale() );
  PointList::instance()->set_error_mode( od.get_error_mode() );
  emit file_opened();
  return;
}

void MainWindow::about()
{
  QMessageBox::about(this, tr("About Image Viewer"),
		     tr("<p>The <b>Image Viewer</b> example shows how to combine QLabel "
			"and QScrollArea to display an image. QLabel is typically used "
			"for displaying a text, but it can also display an image. "
			"QScrollArea provides a scrolling view around another widget. "
			"If the child widget exceeds the size of the frame, QScrollArea "
			"automatically provides scroll bars. </p><p>The example "
			"demonstrates how QLabel's ability to scale its contents "
			"(QLabel::scaledContents), and QScrollArea's ability to "
			"automatically resize its contents "
			"(QScrollArea::widgetResizable), can be used to implement "
			"zooming and scaling features. </p><p>In addition the example "
			"shows how to use QPainter to print an image.</p>"));
}

void MainWindow::createActions()
{
  open_act_ = new QAction(tr("&Open Image..."), this);
  open_act_->setShortcut(tr("Ctrl+O"));
  connect(open_act_, SIGNAL(triggered()), this, SLOT(open()));

  exit_act_ = new QAction(tr("E&xit"), this);
  exit_act_->setShortcut(tr("Ctrl+Q"));
  connect(exit_act_, SIGNAL(triggered()), this, SLOT(close()));

  QIcon zoominicon(":/icons/zoomin.png");
  zoom_in_act_ = new QAction(zoominicon, tr("Zoom &In (25%)"), this);
  zoom_in_act_->setShortcut(tr("Ctrl++"));
  connect(zoom_in_act_, SIGNAL(triggered()), this, SLOT(zoom_in()));

  QIcon zoomouticon(":/icons/zoomout.png");
  zoom_out_act_ = new QAction(zoomouticon, tr("Zoom &Out (25%)"), this);
  zoom_out_act_->setShortcut(tr("Ctrl+-"));
  connect(zoom_out_act_, SIGNAL(triggered()), this, SLOT(zoom_out()));

  normal_size_act_ = new QAction(tr("&Normal Size"), this);
  normal_size_act_->setShortcut(tr("Ctrl+S"));
  connect(normal_size_act_, SIGNAL(triggered()), this, SLOT(normal_size()));

  QIcon fiticon(":/icons/fittowindow.png");
  fit_to_window_act_ = new QAction(fiticon, tr("&Fit to Window"), this);
  fit_to_window_act_->setShortcut(tr("Ctrl+F"));
  connect(fit_to_window_act_, SIGNAL(triggered()), this, SLOT(fit_to_window()));

  about_act_ = new QAction(tr("&About"), this);
  connect(about_act_, SIGNAL(triggered()), this, SLOT(about()));

  about_qt_act_ = new QAction(tr("About &Qt"), this);
  connect(about_qt_act_, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
}

void MainWindow::createMenus()
{
  file_menu_ = new QMenu(tr("&File"), this);
  file_menu_->addAction(open_act_);
  file_menu_->addSeparator();
  file_menu_->addAction(exit_act_);

  view_menu_ = new QMenu(tr("&View"), this);
  view_menu_->addAction(zoom_in_act_);
  view_menu_->addAction(zoom_out_act_);
  view_menu_->addAction(normal_size_act_);
  view_menu_->addAction(fit_to_window_act_);

  help_menu_ = new QMenu(tr("&Help"), this);
  help_menu_->addAction(about_act_);
  help_menu_->addAction(about_qt_act_);

  menuBar()->addMenu(file_menu_);
  menuBar()->addMenu(view_menu_);
  menuBar()->addMenu(help_menu_);
}
