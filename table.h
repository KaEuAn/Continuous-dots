//
// Created by Eugene on 14.11.2018.
//

#ifndef CONTINUOUS_DOTS_TABLE_H
#define CONTINUOUS_DOTS_TABLE_H

#endif //CONTINUOUS_DOTS_TABLE_H

#include "geometry.h"


class Area: public ConvexHull {
private:

public:

};

class Table {

private:
  double x_min, x_max, y_min, y_max;
  u32 players_number;
  u32 areas_number;
  std::vector<Area> areas;

public:
  Table(u32 pla_n, u32 a) : x_min(0), x_max(1e6), y_min(0), y_max(1e6), players_number(pla_n), areas_number(a), areas(areas_number) {}
  Table(u32 n, u32 m, u32 pla_n, u32 a) : x_min(0), x_max(n), y_min(1e6), y_max(m), players_number(pla_n), areas_number(a), areas(areas_number) {}

  
};