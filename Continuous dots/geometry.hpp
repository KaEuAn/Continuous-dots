//
// Created by Eugene on 14.11.2018.
//
#pragma once

#include <cstdio>
#include <vector>
#include <string>
#include <cmath>
#include <typeinfo>
#include <set>
#include <list>
#include <utility>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <iostream>
using ld = long double;
using ll = long long;
using u32 = uint32_t;




bool isEqual (const ld a, const ld b);
bool isLessThan(ld x, ld y);
bool isGreaterThan(ld x, ld y);

bool isLessOrEqual(ld x, ld y);

struct Point {
  ld x, y;

  Point(ld a, ld b);

  Point();
  Point(const Point& a);

  bool operator ==(const Point& a) const;
  bool operator != (const Point& a);

  Point operator - (const Point& a) const;
  Point& operator += (const Point& a);
  Point operator + (const Point& a) const;
  Point operator / (ld a) const;
  Point operator * (ld a) const;

  ld angle() const;
  ld length() const;

  ld lengthToPoint(const Point& b) const;

  ld scalarProduct(const Point& a) const;
  ld pseudoScalarProduct(const Point& a) const;
  ld operator%(const Point& second) const;
  ld operator*(const Point& second) const;

  bool isBetweenY(const Point& a, const Point& b) const;

  void print() const;
};

u32 max(u32 a, u32 b);
u32 min(u32 a, u32 b);


bool isEqual (const Point& a, const Point& b);

struct boolAndIt{
  bool answer;
  std::list<Point>::const_iterator first_it;
  std::list<Point>::const_iterator second_it;
};

//isLess
bool smartCompare(Point cmpPoint, Point b, Point c);

class DotArea{
public:
  bool isActive;
  u32 team_number;
  u32 len;
  u32 max_area;
  std::list<Point> points;
public:
  DotArea(const std::vector<Point>& p_inp, u32 t);
  DotArea(const Point& p_inp, u32 t);
  DotArea(Point&& p_inp, u32 t);

  bool isActivef() const;
  u32 getMaxArea() const;

  bool print() const;

  bool hasIn(const Point& point) const;

  bool hasIn(const DotArea& hull) const;

  boolAndIt isCombinable(const DotArea& other) const;

  void combine(DotArea& other, boolAndIt answer);

  void ezOptimize();

  void Djarvis();
};
