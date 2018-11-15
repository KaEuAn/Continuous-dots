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
  Table(u32 p, u32 a) : x_min(0), x_max(0), y_min(1e6), y_max(1e6), players_number(n), areas_number(a), areas(areas_number) {}


};