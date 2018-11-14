//
// Created by Eugene on 14.11.2018.
//

#ifndef CONTINUOUS_DOTS_GEOMETRY_H
#define CONTINUOUS_DOTS_GEOMETRY_H

#endif //CONTINUOUS_DOTS_GEOMETRY_H

#include <cstdio>
#include <vector>
#include <string>
#include <cmath>
#include <typeinfo>
#include <set>
#define PI 3.1415926535897932384626433832795
using ld = long double;
using ll = long long;
using u32 = uint32_t;

bool approximatelyEqual (const ld a, const ld b) {
  ld eps = 2e-10;
  ld difference = a - b;
  if (difference < 0)
    difference *= -1;
  return difference < eps;
}
bool approximatelyEqual (const Point& a, const Point& b) {
  return approximatelyEqual(a.x, b.x) && approximatelyEqual(a.y, b.y);
}

bool isLessThan(ld x, ld y) {
  return x < y - EPS;
}
bool isGreaterThan(ld x, ld y) {
  return x >  y + EPS;
}

struct Point {
  ld x, y;

  Point(ld a, ld b) : x(a), y(b) {};

  Point() : x(0), y(0) {};
  Point(const Point& a) : x(a.x), y(a.y) {}

  bool operator ==(const Point& a) const {
    return (approximatelyEqual(x, a.x) && approximatelyEqual(y, a.y));
  }

  bool operator != (const Point& a) {
    return ! (*this == a);
  }

  Point operator - (const Point& a) const {
    Point c(*this);
    c.x -= a.x;
    c.y -= a.y;
  }
  Point& operator += (const Point& a) {
    x += a.x;
    y += a.y;
    return *this;
  }
  Point operator + (const Point& a) const {
    Point c(*this);
    return c += a;
  }
  Point operator / (ld a) const {
    Point c(*this);
    c.x /= a;
    c.y /= a;
    return c;
  }
  Point operator * (ld a) const {
    Point c(*this);
    c.x *= a;
    c.y *= a;
    return c;
  }

  ld angle() const {
    return y / sqrt(x * x + y * y);
  }
  ld length() const {
    return lengthToPoint({0, 0});
  }

  ld lengthToPoint(const Point& b) const {
    return sqrt((x - b.x) * (x - b.x) + (y - b.y) * (y - b.y));
  }

  void scale(Point center, ld coefficient) {
    x += (x - center.x) * (coefficient - 1);
    y += (y - center.y) * (coefficient - 1);
  }
  void rotate(Point center, ld angle) {
    Point differ = *this - center;
    ld old_angle = std::asin(angle());
    angle *= PI/180;
    old_angle += angle;
    x = std::sin(old_angle) * differ.length();
    y = std::cos(old_angle) * differ.length();
  }
  void reflex(Line axis) {
    Line perAxis = axis.makePerpendicularLine(*this);
    Point a = perAxis.intersection(axis);
    (*this) += (a - (*this)) * 2;
  }
  void reflex(Point center) {
    scale(center, -1);
  }

  ld scalarProduct(const Point& a) const {
    return a.x * x + a.y * y;
  }
  ld pseudoScalarProduct(const Point& a) const {
    return a.x * y - x * a.y;
  }
  ld operator%(const Point& second) const {
    return pseudoScalarProduct(second);
  }
  ld operator*(const Point& second) const {
    return scalarProduct(second);
  }
};

class Polygon {

protected:
  std::vector<Point> vertices;

  void pushEl(Point a) {
    vertices.push_back(a);
  }

  template<class... Args>
  void pushEl(Point a, Args... args) {
    vertices.push_back(a);
    pushEl(args...);
  }
public:
  //constructors
  template<class... Args>
  Polygon(Args... args) {
    pushEl(args...);
  }
  Polygon (std::vector<Point>& vec): vertices(vec){}
  //

  virtual ~Polygon() {};

  const std::vector<Point> getVertices () const {
    return vertices;
  }
  uint32_t verticesCount() const {
    return vertices.size();
  }

};


Point cmpPoint; //by x
//isLess
//if cmpPoint
bool smartCompare(Point b, Point c) {
  ld vect = (b - cmpPoint) % (c - cmpPoint);
  if ( (isLessThan(b.x, cmpPoint.x) && isLessThan(c.x, cmpPoint.x)) || (isGreaterThan(b.x, cmpPoint.x) && isGreaterThan(c.x, cmpPoint.x)) )
    return (isGreaterThan(vect, 0));
  if (isLessThan(b.x, cmpPoint.x))
    return false;
  return true;
}
bool xCompare(Point a, Point b) {
  if (isZero(a.x - b.x))
    return a.y < b.y;
  return a.x < b.x;
}

vector<Point> makeConvexHull_first(vector<Point>& xy, uint32_t n) {
  vector<Point> convexHull;
  convexHull.reserve(n);
  std::sort(xy.begin(), xy.end(), xCompare);
  cmpPoint = xy[0];
  std::sort(++xy.begin(), xy.end(), smartCompare);
  for (int i = 1; i < n; ++i) {
    while (convexHull.size() >= 2 && !smartCompare1 (convexHull[convexHull.size() - 2],
                                                     convexHull[convexHull.size() - 1], xy[i]))
      convexHull.pop_back();
    convexHull.push_back(convexHull[i]);
  }
  return convexHull;
}

class ConvexHull{
private:
  std::set<Point, smartCompare> points;
  int team_number;
public:
  ConvexHull(const vector<Point>& p_inp, int t) {
    team_number = t;
    vector<Point> convh = makeConvexHull_first(p_inp, p_inp.size());
    points(convh.begin(), convh.end());
  }
  ConvexHull(const Point& p_inp, int t) {
    team_number = t;
    vector<Point> convh = vector<Point>();
    convh.push_back(p_inp);
    cmpPoint = p_inp;
    points(convh.begin(), convh.end());
  }

  bool isIn(const Point& point) const {
    std::set<Point, smartCompare>::iterator it_prev = nullptr, it = points.begin();
    while (it != points.end()) {
      ld vect = (point - *it) % (*it_prev - *it);
      if (isLessThan(vect, 0))
        return false;
      it_prev = it;
      ++it;
    }
    return true;
  }

  bool isIn(const ConvexHull& hull) const {
    for (const auto& point: hull.points) {
      if (! isIn(point))
        return false;
    }
    return true;
  }
};