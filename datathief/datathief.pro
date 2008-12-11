######################################################################
# qmake configuration file for datathief project


TEMPLATE = app
TARGET = datathief
# DEPENDPATH += .
# INCLUDEPATH += .

win32 {
  OBJECTS_DIR = build_windows
}
macx {
  OBJECTS_DIR = build_mac
}
#unix {
#  OBJECTS_DIR = build_linux
#}

MOC_DIR = $$OBJECTS_DIR

# Input
HEADERS += ButtonBar.h \
           OpenDialog.h \
           Label.h \ 
           MainWindow.h \
           GraphicsView.h \
           GraphicsImageItem.h \
           GraphicsPointItem.h \
           GraphicsScene.h \
           LabelButton.h \
           ZoomedView.h \
           PointList.h

SOURCES += ButtonBar.cxx \
           OpenDialog.cxx \
           Label.cxx \
           MainWindow.cxx \
           GraphicsView.cxx \
           GraphicsImageItem.cxx \
           GraphicsPointItem.cxx \
           GraphicsScene.cxx \
           LabelButton.cxx \
           ZoomedView.cxx \
           PointList.cxx \
           main.cxx

RESOURCES += icons.qrc
DEFINES += DEBUG
