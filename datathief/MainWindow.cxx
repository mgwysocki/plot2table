#include <QtGui>

#include "ZoomedView.h"
#include "GraphicsScene.h"
#include "OpenDialog.h"
#include "ButtonBar.h"
#include "MainWindow.h"
#include "GraphicsView.h"

#include <iostream>

MainWindow::MainWindow()
{
  QSplitter* splitter = new QSplitter;
  QSplitter* splitter2 = new QSplitter;
  _button_bar = new ButtonBar;
  splitter2->addWidget(_button_bar);
  splitter2->setOrientation(Qt::Vertical);
  splitter->addWidget(splitter2);

  _scene = new GraphicsScene;
  _gview = new GraphicsView;
  _gview->setDragMode(QGraphicsView::ScrollHandDrag);
  _gview->setScene(_scene);
  _gview->setCursor(Qt::CrossCursor);
  _gview->setBackgroundRole(QPalette::Dark);
  splitter->addWidget(_gview);

  _zoomview = new ZoomedView;
  _zoomview->setScene(_scene);
  splitter2->addWidget(_zoomview);

  QList<int> sizes;
  sizes.append(200);
  sizes.append(800);
  splitter->setSizes(sizes);
  splitter->setChildrenCollapsible(false);
  
  splitter->setStretchFactor(0, 0);
  splitter->setStretchFactor(1, 1);
  this->setCentralWidget(splitter);

  _zoomview->setInteractive(false);
  _zoomview->scale(10., 10.);
  splitter2->setStretchFactor(0, 0);
  splitter2->setStretchFactor(1, 1);

  _zoomview->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  _zoomview->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  

  connect(&_gview->_overlay, SIGNAL(movescene(QPoint)), this, SLOT(move_zoom_view(QPoint)));

  connect(_button_bar->open_button, SIGNAL(clicked()), 
	  this, SLOT(open()));
  connect(_button_bar->zoom_in_button, SIGNAL(clicked()), 
	  this, SLOT(zoom_in()));
  connect(_button_bar->zoom_out_button, SIGNAL(clicked()), 
	  this, SLOT(zoom_out()));
  connect(_button_bar->normal_size_button, SIGNAL(clicked()), 
	  this, SLOT(normal_size()));
  connect(_button_bar->fit_to_screen_button, SIGNAL(clicked()), 
	  this, SLOT(fit_to_window()));
  connect(_button_bar->remove_button, SIGNAL(clicked()), 
	  _scene, SLOT(remove_last_point()));
  connect(_button_bar->remove_all_button, SIGNAL(clicked()), 
	  _scene, SLOT(remove_all_points()));
  connect(_button_bar, SIGNAL(color_changed(QColor)), 
	  _scene, SLOT(change_point_color(QColor)));
  connect(_button_bar, SIGNAL(time_to_save(double, double, double, double, bool, bool)), 
	  _scene, SLOT(save_points(double, double, double, double, bool, bool)));

//   connect(this, SIGNAL(file_opened(QString)),
// 	  _button_bar, SLOT(enable_buttons()));
  connect(_scene, SIGNAL(point0_set()),
	  _button_bar, SLOT(point0_set()));
  connect(_scene, SIGNAL(point1_set()),
	  _button_bar, SLOT(point1_set()));
  connect(_scene, SIGNAL(data_point_added()),
	  _button_bar, SLOT(point_added()));

  connect(_scene, SIGNAL(image_doesnt_fit(QSize)),
	  this, SLOT(resize_to_image(QSize)));

  createActions();
  createMenus();

  setWindowTitle(tr("Image Viewer"));
  QSize maxsize(QApplication::desktop()->size());
  maxsize += QSize(-50, -50);
  setMaximumSize(maxsize);
  resize(800, 600);

  // Testing...
  std::cout << "Current directory:\n" 
	    << qPrintable(QDir::current().path()) << std::endl;
}


void MainWindow::scale_image(double factor)
{
  std::cout << "MainWindow::scale_image(" << factor << ")" << std::endl;
  _gview->scale(factor, factor);
  _scene->set_scale_factor( _gview->matrix().m11() );

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
  _gview->resetMatrix();
  _scene->set_scale_factor( _gview->matrix().m11() );
}

void MainWindow::fit_to_window()
{
  float width_scale_factor = width() / (float) _image_pixmap.width();
  float height_scale_factor = height() / (float) _image_pixmap.height();
  if(width_scale_factor < height_scale_factor)  scale_image(width_scale_factor);
  else                                          scale_image(height_scale_factor);
  return;
}

void MainWindow::resize_to_image(QSize image_size)
{
  std::cout << "MainWindow::resize_to_image()" << std::endl;

  QSize other = size() - _gview->size();
  QSize newsize = other + image_size + QSize(5,5);

  std::cout << "window size: " << width() << "x" << height() << std::endl;
  std::cout << "_zoomview size: " << _zoomview->size().width() << "x" << _zoomview->size().height() << std::endl;
  QScrollArea* sa = (QScrollArea*) _zoomview->viewport();
  std::cout << "scroll area size: " << sa->width() << "x" << sa->height() << std::endl;
  std::cout << "buttonbar size: " << _button_bar->width() << "x" << _button_bar->height() << std::endl;
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
  _image_pixmap = QPixmap::fromImage(image);
  _scene->set_image_pixmap( _image_pixmap );

  _button_bar->enable_buttons();

  _button_bar->log_x_checkbox->setChecked( od.is_x_log_scale() );
  _button_bar->log_y_checkbox->setChecked( od.is_y_log_scale() );
  _scene->error_mode = od.get_error_mode();
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
  _open_act = new QAction(tr("&Open..."), this);
  _open_act->setShortcut(tr("Ctrl+O"));
  connect(_open_act, SIGNAL(triggered()), this, SLOT(open()));

  _exit_act = new QAction(tr("E&xit"), this);
  _exit_act->setShortcut(tr("Ctrl+Q"));
  connect(_exit_act, SIGNAL(triggered()), this, SLOT(close()));

  _zoom_in_act = new QAction(tr("Zoom &In (25%)"), this);
  _zoom_in_act->setShortcut(tr("Ctrl++"));
  connect(_zoom_in_act, SIGNAL(triggered()), this, SLOT(zoom_in()));

  _zoom_out_act = new QAction(tr("Zoom &Out (25%)"), this);
  _zoom_out_act->setShortcut(tr("Ctrl+-"));
  connect(_zoom_out_act, SIGNAL(triggered()), this, SLOT(zoom_out()));

  _normal_size_act = new QAction(tr("&Normal Size"), this);
  _normal_size_act->setShortcut(tr("Ctrl+S"));
  connect(_normal_size_act, SIGNAL(triggered()), this, SLOT(normal_size()));

  _fit_to_window_act = new QAction(tr("&Fit to Window"), this);
  _fit_to_window_act->setShortcut(tr("Ctrl+F"));
  connect(_fit_to_window_act, SIGNAL(triggered()), this, SLOT(fit_to_window()));

  _about_act = new QAction(tr("&About"), this);
  connect(_about_act, SIGNAL(triggered()), this, SLOT(about()));

  _about_qt_act = new QAction(tr("About &Qt"), this);
  connect(_about_qt_act, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
}

void MainWindow::createMenus()
{
  _file_menu = new QMenu(tr("&File"), this);
  _file_menu->addAction(_open_act);
  _file_menu->addSeparator();
  _file_menu->addAction(_exit_act);

  _view_menu = new QMenu(tr("&View"), this);
  _view_menu->addAction(_zoom_in_act);
  _view_menu->addAction(_zoom_out_act);
  _view_menu->addAction(_normal_size_act);
  _view_menu->addSeparator();
  _view_menu->addAction(_fit_to_window_act);

  _help_menu = new QMenu(tr("&Help"), this);
  _help_menu->addAction(_about_act);
  _help_menu->addAction(_about_qt_act);

  menuBar()->addMenu(_file_menu);
  menuBar()->addMenu(_view_menu);
  menuBar()->addMenu(_help_menu);
}

void MainWindow::move_zoom_view(QPoint p)
{
  QPointF pf( _gview->mapToScene(p) );
  QWidget* v = _zoomview->viewport();
  _zoomview->setSceneRect(pf.x()-v->width()/2., pf.y()-v->height()/2.,
			  v->width(), v->height());
  return;
}
