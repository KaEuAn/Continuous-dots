#pragma once

#include "game.hpp"
#include "table.hpp"

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
  bool ans = input[0] == 'r' && input[1] == 'u' && input[2] == 'n';
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

Game::Game(): n(1000), m(1000), player_number(2), area_number(4), count(0), isStarted(false), isStopped(false), isQuited(false), table(n, m, area_number)
,bots(player_number, 1), bot_mutexes(area_number + 1), bots_iterations(player_number, 0), bots_made(player_number, 0){
  input = new char[20];
  clear(input, 20);
}

Game::~Game() {
  delete[] input;
}

void Game::stop() {
  isStopped = true;
  cond_var.notify_all();
}

void Game::print() {
  table.print();
}

void Game::process(int argc, char *argv[]) {
  while(true) {
    std::cout << "enter your command\n";
    fgets(input, 20, stdin);
    if (isStart(input)) {
      std::cout << "get start signal\n";
      if (isStarted) {
        std::cout << "we are already started\n";
        continue;
      }
      isStarted = true;
      /*if (argc == 1) {
        n = static_cast<int>(argv[1]);
        m = n;
      }
      if (argc == 2) {
        n = static_cast<int>(argv[1]);
        m = static_cast<int>(argv[2]);
      }
      table.reset(n, m, area_number);*/
      server_thread = std::thread(&Table::connect, table, this);
      std::cout << "make server_thread\n";
      for(u32 i = 0; i < bots.size(); ++i) {
        bots[i].team_number = i + 1;
        bot_threads.push_back(std::move(std::thread(&Bot::connect, bots[i], this, i)));
        std::cout << "make bot thread " << i << '\n';
      }
      std::cout << "end of start\n";

    } else if ((count = isRun(input))) {
      std::cout << "get run signal\n";
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
      std::cout << "get stop signal\n";
      stop();
    } else if (isQuit(input)) {
      std::cout << "get quit signal\n";
      isQuited = true;
      cond_var.notify_all();
      server_thread.join();
      for(u32 i = 0; i < player_number; ++i) {
          bot_threads[i].join();
      }
      return;
    } else { //isStatus
      stop();
      print();
    }
    clear(input, 20);
  }
}