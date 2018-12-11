//
// Created by Eugene on 27.11.2018.
//
#pragma once

#include <cstdlib>
#include "geometry.hpp"
#include <string>
#include <condition_variable>
#include <arpa/inet.h>
#include "table.hpp"

class Game;

class Bot{
public:
  u32 team_number;
  Bot(u32 n);

  Point makeTurn(u32 n, u32 m);
  
  void connect(Game* game, u32 bot_thread_number);
};

