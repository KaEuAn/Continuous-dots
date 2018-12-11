//
// Created by Eugene on 14.11.2018.
//
#pragma once

#include "geometry.hpp"
#include "table.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <stdlib.h>
#include <string>
#include <pthread.h>
#include <thread>
#include <mutex>
#include <utility>
#include <condition_variable>
#include <deque>
#include <memory>

void clear(char* input, u32 size);


class Player{
public:
  u32 number;
  int socket;
  Player(u32 n, int s);
};



class Area {
public:
  u32 len;
  std::list<DotArea> dot_areas;
  Area();

  void addPoint(Point&& point, u32 t);
  
  void print() const;

};

class Game;

class Table {

private:
  ld x_min, x_max, y_min, y_max;
  u32 max_players_number;
  u32 areas_number;
  std::vector<Area> areas;
  std::list<Player> players;
  u32 players_count, make_turn;
  std::mutex* mutexes;
  std::thread* threads;
  
public:
  Table(u32 a);
  Table(u32 n, u32 m, u32 a);
  ~Table();

  void reset(u32 n, u32 m, u32 a);

  void addPoint(Point&& point, u32 t);

  void connect(Game *game);

  void optimize(u32 i);

  void print(); 
};
