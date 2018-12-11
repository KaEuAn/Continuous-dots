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


bool isStart(char* input) {
  return input[0] == 's' && input[1] == 't' && input[2] == 'a' && input[3] == 'r' && input[4] == 't';
}
bool isStop(char* input) {
  return input[0] == 's' && input[1] == 't' && input[2] == 'o' && input[3] == 'p';
}
bool isQuit(char* input) {
  return input[0] == 'q' && input[1] == 'u' && input[2] == 'i' && input[3] == 't';
}
u32 isRun(char* input) {
  bool ans = input[0] == 'r' && input[1] == 'u' && input[2] == 'n' && input[3] == 'p';
  if (! ans) return 0;
  u32 i = 4, answer = 0;
  while( i < 20 && (input[i] > '9' || input[i] < '0' )) {
    ++i;
  }
  for(;i < 20 && input[i] != 0; ++i) {
    answer *= 10;
    answer += static_cast<int>(input[i]);
  }
  return answer;
}

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