#pragma once

#include "geometry.cpp"
#include "table.cpp"
#include "bots.cpp"
#include <thread>
#include <mutex>
#include <functional>
#include <atomic>


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

  Game(): n(1000), m(1000), player_number(2), area_number(4), count(0), isStarted(false), isStopped(false), isQuited(false), table(n, m, area_number)
  ,bots(player_number, 1), bot_mutexes(area_number + 1), bots_iterations(player_number, 0), bots_made(player_number, 0){
    input = new char[20];
    clear(input, 20);
  }
  ~Game() {
    delete[] input;
  }

  void stop() {
    // и я кричу остановите плёнку
    // это кино я уже смотрел
    isStopped = true;
  }
  
  void print() {
    table.print();
  }

  void process(int argc, char *argv[]) {
    while(gets(input)) {
      if (isStart(input)) {
        if (isStarted) {
          std::cout << "we are started\n";
          continue;
        }
        isStarted = true;
        if (argc == 1) {
          n = static_cast<int>(argv[1]);
          m = n;
        }
        if (argc == 2) {
          n = static_cast<int>(argv[1]);
          m = static_cast<int>(argv[2]);
        }
        table.reset(n, m, area_number);
        server_thread = std::thread(&Table::connect, table, this).detach();

        for(u32 i = 0; i < bots.size(); ++i) {
          bots[i].team_number = i + 1;
          bot_threads.push_back(std::move(std::thread(&Bot::connect, bots[i], this, i).detach()));
        }

      } else if (count = isRun(input)) {
        for(u32 num = 0; num < player_number; ++num) {
          bot_mutexes[num].lock();
          bots_iterations[num] += count;
        }
        for(u32 num = 0; num < player_number; ++num) {
          bot_mutexes[num].unlock();
        }
        isStopped = false;
        cond_var.notify_all();
      } else if (isStop(input)) {
        stop();
      } else if (isQuit(input)) {
        isQuited = true;
      } else { //isStatus
        stop();
        print();
      }
      clear(input, 20);
    }
  }
}