// This is the main window of the application; it talks to all the sub-widgets,
// and connects the buttons of the program to the actual guts.

#ifndef __MAIN_WINDOW_H
#define __MAIN_WINDOW_H

#include <QMainWindow>

class QAction;
class QLabel;
class QMenu;
class QScrollArea;
class QScrollBar;
class QDockWidget;
class QGraphicsView;
class QStackedWidget;

class Label;
class ButtonBar;
class GraphicsScene;
class GraphicsView;
class ZoomedView;

class MainWindow : public QMainWindow
{
 Q_OBJECT

 public:
  MainWindow();

 public slots:
  void open(QString default_filename="");
  void zoom_in();
  void zoom_out();
  void normal_size();
  void fit_to_window();
  void scale_image(double);
  void move_zoom_view(QPoint);

 private slots:
  void about();
  void resize_to_image(QSize);

 private:
  void createActions();
  void createMenus();

  QPixmap _image_pixmap;
  GraphicsView* _gview;
  ZoomedView* _zoomview;
  GraphicsScene* _scene;
  ButtonBar* _button_bar;
  QDockWidget* _dock_widget;
  //QStackedWidget* _stack;
  //Label* _overlay;

  QAction* _open_act;
  QAction* _exit_act;
  QAction* _zoom_in_act;
  QAction* _zoom_out_act;
  QAction* _normal_size_act;
  QAction* _fit_to_window_act;
  QAction* _about_act;
  QAction* _about_qt_act;

  QMenu* _file_menu;
  QMenu* _view_menu;
  QMenu* _help_menu;
};

#endif
