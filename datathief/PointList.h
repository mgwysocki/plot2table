#ifndef __POINTLIST_H
#define __POINTLIST_H

#include <QObject>
#include <QPointF>
#include <QList>

#include "ErrorModes.h"

//-----------------------------------------------------------------------------
//
class PointObject
{
 public:
  PointObject(QPointF p, double el=0., double eh=0.)
    {point = p; high_error = eh; low_error = el;}

  QPointF point;
  float high_error;
  float low_error;
};


//-----------------------------------------------------------------------------
//
class PointList : public QObject
{
  Q_OBJECT

 public:
  
  enum State {
    Uninitialized,
    PointStored,
    LowErrorStored,
    HighErrorStored
  };

  enum NextType {
    AxisPoint0,
    AxisPoint1,
    Point,
    LowError,
    HighError
  };

  static PointList* instance()
  { if(!self) self = new PointList();
    return self; }

  int size() const {return point_list_.size();}
  
  PointObject & operator[](int i) {return point_list_[i];}
  const PointObject & operator[](int i) const {return point_list_[i];}

  void append(const QPointF &p, const double &el, const double &eh)
    {point_list_.append( PointObject(p,el,eh) );}

  void remove(int i)  {point_list_.removeAt(i); state_=Uninitialized;}
  void remove_last()  {point_list_.removeLast(); state_=Uninitialized;}
  void clear()  {point_list_.clear(); state_=Uninitialized;}

  void set_error_mode(ErrorMode m) {mode_ = m;}
  ErrorMode get_error_mode() const {return mode_;}

  void point_clicked(const QPointF &pointf);
  NextType get_next_type() const {return next_type_;}

 public slots:
  void save_points(QPointF graph_point1, QPointF graph_point2, 
		   bool logx, bool logy);

 signals:
  void axis_point_set(int, QPointF);
  void data_point_added(QPointF);
  void low_error_added(QPointF);
  void high_error_added(QPointF);
  void sym_error_added(QPointF);
  void modified();

 private:
  PointList();
  static PointList* self;
  QList<PointObject> point_list_;
  QPointF axis_point0_;
  QPointF axis_point1_;
  bool axis_point0_set_;
  bool axis_point1_set_;

  State state_;
  NextType next_type_;
  ErrorMode mode_;
};


//-----------------------------------------------------------------------------
//
class PointConverter
{
 public:
  PointConverter(const QPointF &graph_p0,
		 const QPointF &graph_p1,
		 const QPointF &scene_p0,
		 const QPointF &scene_p1,
		 const bool &logx,
		 const bool &logy);

  double convert_x(double x_scene);
  double convert_y(double y_scene);

 private:
  double delta_x_graph_;
  double delta_y_graph_;
  double delta_x_scene_;
  double delta_y_scene_;

  double graph_x0_;
  double graph_y0_;
  double scene_x0_;
  double scene_y0_;

  bool logx_;
  bool logy_;

};
#endif // __POINTLIST_H
