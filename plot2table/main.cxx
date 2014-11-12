
#include <QApplication>
#include <QPlastiqueStyle>

#include "MainWindow.h"

int main(int argc, char *argv[])
{
  QApplication app(argc, argv);

#ifdef Q_WS_X11
  // Why the hell is Motif my default style on Linux?
  // Might be fixed in Qt 4.2
  //QApplication::setStyle(new QPlastiqueStyle);
#endif

  MainWindow main_window;
  main_window.show();
  if(argc>1)
    main_window.open( argv[argc-1] );
  return app.exec();
}
