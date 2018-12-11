#pragma once


#include "geometry.hpp"
#include "table.hpp"
#include "bots.hpp"
#include <thread>
#include <mutex>
#include <functional>
#include <atomic>
#include <stdio.h>
#include <stdlib.h>
#include <deque>


bool isStart(char* input);
bool isStop(char* input);
bool isQuit(char* input);
u32 isRun(char* input);

class Game {
public:
  u32 n, m, player_number, area_number, count;
  bool isStarted, isStopped, isQuited;
  pthread_t player_comp;
  
  Table table;
  std::vector<Bot> bots;
  std::vector<std::mutex> bot_mutexes;
  std::vector<u32> bots_iterations;
  std::vector<u32> bots_made;
  std::vector<std::thread> bot_threads;
  std::thread server_thread;
  
  std::condition_variable cond_var;
  
  char* input;
  Game();
  
  ~Game();

  void stop();
  
  void print();

  void process(int argc, char *argv[]);
  
};