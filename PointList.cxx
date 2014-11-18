#include "PointList.h"

#include <QFile>
#include <QTextStream>
#include <cmath>

PointList* PointList::self = 0;

PointList::PointList() :
  axis_point0_(0,0),
  axis_point1_(0,0),
  axis_point0_set_(false),
  axis_point1_set_(false),
  axis_point0_color_("blue"),
  axis_point1_color_("green"),
  point_color_("red"),
  state_(Uninitialized),
  next_type_(AxisPoint0),
  mode_(None)
{}


void PointList::point_clicked(const QPointF &pointf)
{
  if( !axis_point0_set_ ) {
    axis_point0_ = pointf;
    axis_point0_set_ = true;
    next_type_ = AxisPoint1;
    emit axis_point_set(0, pointf);
    
  } else if( !axis_point1_set_ ) {
    axis_point1_ = pointf;
    axis_point1_set_ = true;
    next_type_ = Point;
    emit axis_point_set(1, pointf);

  } else if(state_==Uninitialized) {
    point_list_.append( PointObject(pointf) );
    state_ = PointStored;
    emit data_point_added(pointf);

  } else {
    if( mode_ == Symmetric ){
      if( state_==PointStored ){
        point_list_[size()-1].low_error  = pointf.y();
        point_list_[size()-1].high_error = pointf.y();
        state_ = HighErrorStored;
        QPointF out = point_list_[size()-1].point - QPointF(0., point_list_[size()-1].high_error);
        emit high_error_added(out);

        out = point_list_[size()-1].point + QPointF(0., point_list_[size()-1].low_error);
        emit low_error_added(out);

      } else {
        point_list_.append( PointObject(pointf) );
        state_ = PointStored;
        emit data_point_added(pointf);
      }

    } else if( mode_ == Asymmetric ){
      if( state_==PointStored ){
        point_list_[size()-1].low_error = fabs(pointf.y() - point_list_[size()-1].point.y());
        state_ = LowErrorStored;
        QPointF out = point_list_[size()-1].point + QPointF(0., point_list_[size()-1].low_error);
        emit low_error_added(out);
      } else if( state_==LowErrorStored ){
        point_list_[size()-1].high_error = fabs(pointf.y() - point_list_[size()-1].point.y());
        state_ = HighErrorStored;
        QPointF out = point_list_[size()-1].point - QPointF(0., point_list_[size()-1].high_error);
        emit high_error_added(out);
      } else {
        point_list_.append( PointObject(pointf) );
        state_ = PointStored;
        emit data_point_added(pointf);
      }

    } else {  // mode == no error bars
      point_list_.append( PointObject(pointf) );
      state_ = PointStored;
      emit data_point_added(pointf);
    }
  }
  return;
}


void PointList::save_points(QPointF graph_point1, QPointF graph_point2, bool logx, bool logy)
{
  PointConverter pc(graph_point1, graph_point2, 
                    axis_point0_, axis_point1_, 
                    logx, logy);

#ifdef DEBUG
  QString filename("points.out");
#else
  QString filename = QFileDialog::getOpenFileName(this,
                                                  tr("Open File"), 
                                                  QDir::currentPath());
#endif

  QFile outfile(filename);
  if( !outfile.open(QIODevice::WriteOnly | QIODevice::Text) )
    return;
  QTextStream out(&outfile);
    
  for(int i=0; i<size(); ++i) {
    double x = pc.convert_x(point_list_[i].point.x());
    double y = pc.convert_y(point_list_[i].point.y());
    out << x << " " << y;
    
    if( mode_ == Symmetric ){
      double ey = pc.convert_y( point_list_[i].high_error );
      out << " 0.0 " << (ey-y);

    } else if( mode_ == Asymmetric ) {
      double ey = pc.convert_y( point_list_[i].low_error );
      out << " 0.0 0.0 " << ey;

      ey = pc.convert_y( point_list_[i].high_error );
      out << " " << ey;
    }

    out << endl;
  }

  outfile.close();
  return;
}


//==========================================================================
//
PointConverter::PointConverter(const QPointF &graph_p0,
                               const QPointF &graph_p1,
                               const QPointF &scene_p0,
                               const QPointF &scene_p1,
                               const bool &logx,
                               const bool &logy)
{
  if(logx) delta_x_graph_ = log10(graph_p1.x()) - log10(graph_p0.x());
  else     delta_x_graph_ = graph_p1.x() - graph_p0.x();
  
  if(logy) delta_y_graph_ = log10(graph_p1.y()) - log10(graph_p0.y());
  else     delta_y_graph_ = graph_p1.y() - graph_p0.y();

  delta_x_scene_ = scene_p1.x() - scene_p0.x();
  delta_y_scene_ = scene_p1.y() - scene_p0.y();

  graph_x0_ = graph_p0.x();
  graph_y0_ = graph_p0.y();
  scene_x0_ = scene_p0.x();
  scene_y0_ = scene_p0.y();

  logx_ = logx;
  logy_ = logy;
}


double PointConverter::convert_x(double x_scene)
{
  double x = 0;
  if(logx_)
    x = pow(10, (delta_x_graph_/delta_x_scene_) * (x_scene - scene_x0_) + log10(graph_x0_));

  else
    x = (delta_x_graph_/delta_x_scene_) * (x_scene - scene_x0_) + graph_x0_;

  return x;
}


double PointConverter::convert_y(double y_scene)
{
  double y = 0;
  if(logy_)
    y = pow(10, (delta_y_graph_/delta_y_scene_) * (y_scene - scene_y0_) + log10(graph_y0_));

  else
    y = (delta_y_graph_/delta_y_scene_) * (y_scene - scene_y0_) + graph_y0_;

  return y;
}
