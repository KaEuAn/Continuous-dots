#pragma once

#include "game.hpp"
#include "table.hpp"



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
}

void Game::print() {
  table.print();
}

void Game::process(int argc, char *argv[]) {
  while(true) {
    fgets(input, 20, stdin);
    if (isStart(input)) {
      if (isStarted) {
        std::cout << "we are started\n";
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

      for(u32 i = 0; i < bots.size(); ++i) {
        bots[i].team_number = i + 1;
        bot_threads.push_back(std::move(std::thread(&Bot::connect, bots[i], this, i)));
      }

    } else if ((count = isRun(input))) {
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