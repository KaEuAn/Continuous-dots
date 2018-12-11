#include "geometry.hpp"
#include <cmath>
#define PI 3.1415926535897932384626433832795
ld eps = 2e-10;
ld Radius = 10;


bool isEqual (const ld a, const ld b) {
  ld difference = a - b;
  if (difference < 0)
    difference *= -1;
  return difference < eps;
}

bool isLessThan(ld x, ld y) {
  return x < y - eps;
}

bool isGreaterThan(ld x, ld y) {
  return x >  y + eps;
}

bool isLessOrEqual(ld x, ld y) {
  return isLessThan(x, y) || isEqual(x, y);
}

Point::Point(ld a, ld b) : x(a), y(b) {}
Point::Point(const Point& a) : x(a.x), y(a.y) {}
bool Point::operator ==(const Point& a) const {
  return (isEqual(x, a.x) && isEqual(y, a.y));
}

bool Point::operator != (const Point& a) {
  return ! (*this == a);
}

Point Point::operator - (const Point& a) const {
  Point c(*this);
  c.x -= a.x;
  c.y -= a.y;
  return c;
}
Point& Point::operator += (const Point& a) {
  x += a.x;
  y += a.y;
  return *this;
}

Point Point::operator + (const Point& a) const {
  Point c(*this);
  return c += a;
}

Point Point::operator / (ld a) const {
  Point c(*this);
  c.x /= a;
  c.y /= a;
  return c;
}

Point Point::operator * (ld a) const {
  Point c(*this);
  c.x *= a;
  c.y *= a;
  return c;
}

ld Point::angle() const {
  return y / sqrt(x * x + y * y);
}

ld Point::length() const {
  return lengthToPoint({0, 0});
}

ld Point::lengthToPoint(const Point& b) const {
  return sqrt((x - b.x) * (x - b.x) + (y - b.y) * (y - b.y));
}

ld Point::scalarProduct(const Point& a) const {
  return a.x * x + a.y * y;
}

ld Point::pseudoScalarProduct(const Point& a) const {
  return a.x * y - x * a.y;
}

ld Point::operator%(const Point& second) const {
  return pseudoScalarProduct(second);
}

ld Point::operator*(const Point& second) const {
  return scalarProduct(second);
}

bool Point::isBetweenY(const Point& a, const Point& b) const {
  return (isLessThan(y, b.y) && isLessThan(a.y, y)) || (! isGreaterThan(y, b.y) && ! isGreaterThan(a.y, y));
}

void Point::print() const {
  std::cout << std::setprecision(3) << x << ' ' << y;
}  

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

bool isEqual (const Point& a, const Point& b) {
  return isEqual(a.x, b.x) && isEqual(a.y, b.y);
}

//isLess
bool smartCompare(Point cmpPoint, Point b, Point c) {
  ld ans = (b - cmpPoint) % (c - cmpPoint);
  return (isGreaterThan(ans, 0));
}

DotArea::DotArea(const std::vector<Point>& p_inp, u32 t) : isActive(true), team_number(t), len(p_inp.size()), max_area(0)
, points(p_inp.begin(), p_inp.end()) {
  len = p_inp.size();
}

DotArea::DotArea(const Point& p_inp, u32 t) : isActive(true), team_number(t), len(1), max_area(0) {
  points.push_back(p_inp);
}

DotArea::DotArea(Point&& p_inp, u32 t) : isActive(true), team_number(t), len(1), max_area(0)  {
  points.emplace_back(std::forward<Point>(p_inp));
}

bool DotArea::isActivef() const {
  return isActive;
}
u32 DotArea::getMaxArea() const {
  return max_area;
}

bool DotArea::print() const {
  if (!isActive)
    return false;
  std::cout << "team number " << team_number << ", points:";
  for(auto& it: points) {
    it.print();
    std::cout << "; ";
  }
  return true;
}

bool DotArea::hasIn(const Point& point) const {
  std::list<Point>::const_iterator it_prev = static_cast<std::list<Point>::const_iterator>(nullptr), it = points.begin();
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

bool DotArea::hasIn(const DotArea& hull) const {
  for (const auto& point: hull.points) {
    if (! hasIn(point))
      return false;
  }
  return true;
}


boolAndIt DotArea::isCombinable(const DotArea& other) const {
  for(auto it = points.begin(); it != points.end(); ++it) {
    for(auto other_it = points.begin(); other_it != other.points.end(); ++other_it) {
      if (isLessThan(it->lengthToPoint(*other_it), Radius))
        return {true, it, other_it};
    }
  }
  return {false, static_cast<std::list<Point>::const_iterator>(nullptr), static_cast<std::list<Point>::const_iterator>(nullptr)};
}

void DotArea::combine(DotArea& other, boolAndIt answer) {
  max_area = max(max_area, other.max_area);
  Point p_new = Point(*(answer.first_it));
  pos = std::next(answer.first_it);
  points.splice(pos, other.points, answer.second_it, other.points.end());
  points.splice(pos, other.points, other.points.begin(), other.points.end());
  points.emplace(pos, std::move(p_new));
  other.isActive = false;
}

void DotArea::ezOptimize() {
  if (len < 3)
    return;
  auto it = points.begin();
  auto it_prr = it++;
  auto it_pr = it++;

  while (len >= 2 && it != points.end()) {
    while(it != points.end() && !smartCompare(*it_prr, *it_pr, *it) && isLessThan(it->lengthToPoint(*it_prr), Radius)) {
      points.erase(it);
      it = std::next(it_pr);
    }
    ++it, ++it_pr, ++it_prr;
  }

}

void DotArea::Djarvis() {
  if (len < 3)
    return;
  
  std::list<Point> new_list;
  ld minx = 99999999;
  std::list<Point>::iterator cur;
  for(auto it = points.begin(); it != points.end(); ++it){
    if(it->x < minx) {
      cur = it;
      minx = cur->x;
    }
  }
  new_list.push_back(*cur);
  points.erase(cur);
  std::list<Point>::iterator start = cur;
  std::list<Point>::iterator itnext(nullptr);
  for(auto it = points.begin(); it != points.end(); ++it){
    if (itnext == static_cast<std::list<Point>::iterator>(nullptr)) {
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
    std::list<Point>::iterator new_point = static_cast<std::list<Point>::iterator>(nullptr);
    bool isConvexSituation = true;
    for(auto it = points.begin(); it != points.end(); ++it){
      if (smartCompare(*cur, *itnext, *it)) {
          //convex point (more left than or point)
        if ((! isConvexSituation) || isGreaterThan(it->lengthToPoint(*itnext), Radius) )
          continue;
        if (new_point == static_cast<std::list<Point>::iterator>(nullptr) || smartCompare(*itnext, *it, *new_point)) {
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