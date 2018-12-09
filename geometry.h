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
#include <list>
#include <utility>
#define PI 3.1415926535897932384626433832795
using ld = long double;
using ll = long long;
using u32 = uint32_t;

ld eps = 2e-10;

bool isEqual (const ld a, const ld b) {
  ld difference = a - b;
  if (difference < 0)
    difference *= -1;
  return difference < eps;
}
bool isEqual (const Point& a, const Point& b) {
  return aisEqual(a.x, b.x) && isEqual(a.y, b.y);
}

bool isLessThan(ld x, ld y) {
  return x < y - eps;
}
bool isGreaterThan(ld x, ld y) {
  return x >  y + eps;
}

bool isLessOrEqual(ld x, ld y) {
  return isLessThan(x, y) || isEqual(x, y)
}

struct Point {
  ld x, y;

  Point(ld a, ld b) : x(a), y(b) {};

  Point() : x(0), y(0) {};
  Point(const Point& a) : x(a.x), y(a.y) {}

  bool operator ==(const Point& a) const {
    return (isEqual(x, a.x) && isEqual(y, a.y));
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

  bool isBetweenY(const Point& a, const Point& b) const {
    return (isLessThan(y, b,y) && isLessThan(a.y, y)) || (! isLGreaterThan(y. b.y) && ! isGraterThan(a.y, y));
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

u32 max(u32 a, u32 b) {
  if (a > b)
    return a;
  return b;
}

u32 min(u32 a, u32 b) {
  if (a < b)
    return a;
  return b;
}

ld Radius = 10;

struct boolAndIt{
  bool answer;
  std::list<Point>::const_iterator first_it;
  std::list<Point>::const_iterator second_it;
};

//isLess
bool smartCompare(Point cmpPoint, Point b, Point c) {
  ld ans = (b - cmpPoint) % (c - cmpPoint);
  return (isGreaterThan(ans, 0));
}

class DotArea{
private:
  bool isActive;
  u32 team_number;
  u32 len;
  u32 max_area;
  std::list<Point> points;
public:
  DotArea(const vector<Point>& p_inp, u32 t) : isActive(true), team_number(t), len(p_int.size()), max_area(0) {
    team_number = t;
    points(p_inp.begin(), p_inp.end());
    len = p_inp.size();
  }
  DotArea(const Point& p_inp, u32 t) : isActive(true), team_number(t), len(1), max_area(0) {
    points.push_back(p_inp);
  }
  DotArea(Point&& p_inp, u32 t) : isActive(true), team_number(t), len(1), max_area(0)  {
    points.emplace_back(std::forward<Point>(p_inp));
  }

  bool hasIn(const Point& point) const {
    std::set<Point>::const_iterator it_prev = nullptr, it = points.begin();
    int per = 0;
    while (it != points.end()) {
      if (! isEqual(it->y, it_prev->y) && point.isBetweenY(*it, *it_prev)) {
        if (isGreaterThan((point - *it_prev) % (*it - *it_prev), 0))
          ++per;
      }
      it_prev = it;
      ++it;
    }
    return per % 2;
  }

  bool hasIn(const DotArea& hull) const {
    for (const auto& point: hull.points) {
      if (! hasIn(point))
        return false;
    }
    return true;
  }

  boolAndIt isCombinable(const DotArea& other) const {
    for(const auto& it: points) {
      for(const auto& other_it: other.points) {
        if (isLessThan(it.lengthToPoint(other_it), Radius))
          return {true, it, other_it};
      }
    }
    return {false, nullptr, nullptr};
  }

  void combine(DotArea& other, boolAndIt answer) {
    max_area = max(max_area, other.max_area);
    Point p_new = Point(*(answer.first_it));
    const auto& pos = std::next(answer.first_it);
    points.splice(pos, other.points, answer.second_it, other.points.end());
    points.emplace(pos, std::move(p_new));
  }

  void ezOptimize() {
    if (len < 3)
      return;
    const auto& it = points.begin();
    const auto& it_prr = it++;
    const auto& it_pr = it++;

    while (len >= 2 && it != points.end()) {
      while(it != points.end() && !smartCompare(*it_prr, *it_pr, *it) && isLessThan(it->lengthToPoint(*it_prr), Radius)) {
        points.erase(it);
        it = std::next(it_pr);
      }
      ++it, ++it_pr, ++it_prr;
    }

  }

  void Djarvis() {
    if (len < 3)
      return;
    
    std::list<Point> new_list;
    ld minx = 99999999;
    std::list<Point>::iterator cur;
    for(auto& it: points){
      if(it->x < minx) {
        cur = it;
        minx = cur->x;
      }
    }
    new_list.push_back(*cur);
    points.erase(cur);
    std::list<Point>::iterator start = cur;
    std::list<Point>::iterator itnext(nullptr;
    for(auto& it: points){
      if (itnext == nullptr) {
        if (isLessOrEqual(it->lengthToPoint(*cur), Radius))
          itnext = it;
      }
      else {
        if (smartCompare(*cur, *itnext, *it) && isLessOrEqual(it->lengthToPoint(*cur), Radius)) {
          itnext = it;
        }
      }
    }
    new_list.push_back(*cur);
    points.erase(cur);
    ld new_len = 2;
    //so now we have to start points
    while(*start != *itnext) {
      std::list<Point>::iterator new_point = nullptr;
      bool isConvexSituation = true;
      for(auto& it: points){
        if (smartCompare(*cur, *itnext, *it)) {
           //convex point (more left than or point)
          if ((! isConvexSituation) || isGreaterThan(it->lengthToPoint(*itnext), Radius) )
            continue;
          if (new_point == nullptr || smartCompare(*itnext, *it, *new_point)) {
            new_point = it;
          }
        } else {
          if (isGreaterThan(it->lengthToPoint(*itnext), Radius)) {
            continue;
          }
          //we found inconvex place and shoud go here to maximize square
          if (isConvexSituation)
            new_point = it;
          if (smartCompare(*itnext, *it, *new_point))
            new_point = it;
          isConvexSituation = false;
          
        }
      }
    }

    len = new_len;
    points = new_list; 
  }
};