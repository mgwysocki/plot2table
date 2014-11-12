// This is the main window of the application; it talks to all the sub-widgets,
// and connects the buttons of the program to the actual guts.

#ifndef __MAIN_WINDOW_H
#define __MAIN_WINDOW_H

#include <QMainWindow>

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

 signals:
  void file_opened();

 private slots:
  void about();
  void resize_to_image(QSize);

 private:
  void createActions();
  void createMenus();

  QPixmap image_pixmap_;
  GraphicsView* gview_;
  ZoomedView* zoomview_;
  GraphicsScene* scene_;
  ButtonBar* button_bar_;
  QDockWidget* dock_widget_;

  QAction* open_act_;
  QAction* exit_act_;
  QAction* zoom_in_act_;
  QAction* zoom_out_act_;
  QAction* normal_size_act_;
  QAction* fit_to_window_act_;
  QAction* about_act_;
  QAction* about_qt_act_;

  QMenu* file_menu_;
  QMenu* view_menu_;
  QMenu* help_menu_;
};

#endif
