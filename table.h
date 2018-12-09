//
// Created by Eugene on 14.11.2018.
//

#ifndef CONTINUOUS_DOTS_TABLE_H
#define CONTINUOUS_DOTS_TABLE_H

#include "geometry.h"
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
#include <mutex>
#include <utility>
#include <condition_variable>


class Player{
public:
  u32 number;
  int socket;
};



class Area {
public:
  u32 len;
  std::list<DotArea> dot_areas;
  Area() : len(0) {}

  void addPoint(Point&& point, u32 t) {
    dot_areas.emplace_back({point, t});
    ++len;
  }

};

class Table {

private:
  ld x_min, x_max, y_min, y_max;
  u32 max_players_number;
  u32 areas_number;
  std::vector<Area> areas;
  std::list<Player> players;
  u32 players_count, make_turn;
  std::vector<std::mutex> mutexes;

public:
  Table(u32 a) : x_min(0), x_max(1e6), y_min(0), y_max(1e6), max_players_number(0), areas_number(a), areas(areas_number)
          , players_count(0), make_turn(0), mutexes(areas_number) {}
  Table(u32 n, u32 m, u32 a) : x_min(0), x_max(n), y_min(0), y_max(m), max_players_number(0), areas_number(a)
          , areas(areas_number), players_count(0), make_turn(0), mutexes(areas_number) {}

  void reset(u32 n, u32 m, u32 a) {
    x_max = n; y_max = m; areas_number = a;
  }

  void addPoint(Point&& point) {
    u32 i = static_cast<u32> ((point.x - x_min) / (x_max - x_min) * areas_number + 0.0000001);
    areas[i].addPoint(std::forward<Point>(point));
  }

  void optimize(u32 i) {
    //first part
    mutexes[i].lock();
    Area& my_area = areas[i];
    u32 max_area = i;
    for(auto& it: my_area.dot_areas) {
      if (! it->isActive)
        continue;
      max_area = max(max_area, it->max_area);
      auto& new_it = it;
      while(new_it != my_area.dot_areas.end()) {
        if (it == new_it || ! new_it.isActive) {
          continue; 
          ++new_it
        }
        boolAndIt answer;
        if ((answer = it->isCombinable(*new_it)).first == true) {
          it->combine(*new_it, answer);
          my_area.dot_areas.erase(new_it++);
          --my_area.len;
          it->ezOptimize();
          it->Djarvis();
        }else if(it->hasIn(*new_it)) {
          new_it->isActive = false;
        } else if(new_it->hasIn(*it)) {
          it->isActive = false;
        } else {
          ++new_it;
        }
      }
    }
    //we have mutex order so first will combine areas with big number
    for(u32 j = i + 1; j < min(max_area + 1, areas_number) + 1; ++j) {
      mutexes[j].lock();
      //can work with j now
      Area& new_area = areas[j];
      for(auto& it: my_area.dot_areas) {
        if (!it->isActive)
          continue;
        max_area = max(max_area, it->max_area);
        auto& new_it = new_area.dot_areas.begin();
        while(new_it != my_area.dot_areas.end()) {
          if (!new_it.isActive) {
            continue; 
            ++new_it
          }
          boolAndIt answer;
          if ((answer = it->isCombinable(*new_it)).first == true) {
            it->combine(*new_it, answer);
            new_area.dot_areas.erase(new_it++);
            --new_area.len;
            it->ezOptimize();
            it->Djarvis();
          }else if(it->hasIn(*new_it)) {
            new_it->isActive = false;
          } else if(new_it->hasIn(*it)) {
            it->isActive = false;
          } else {
            ++new_it;
          }
        }
      }
    }

    for(u32 j = min(max_area + 1, areas_number); j >= i; --j) {
      mutexes[j].unlock();
    }
  }

  void connect() {

    int serverSocket;
    if ((serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_IP)) == 0) {
      perror("server socket failure");
      exit(1);
    }

    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(8080);
    int addrlen = sizeof(servaddr);
    if(bind(serverSocket, (struct sockaddr *)&servaddr, sizeof(servaddr))<0){
      perror("bind failure");
      exit(1);
    }
    if(listen(serverSocket, 5)<0){
      perror("listen failure");
      exit(1);
    }

    fd_set readset;//=writeset
    int max_sd; //for select
    size_t round_number = 1;
    clock_t prev_time = clock();

    while(1) {
      size_t my_clock_ps = 260;
      FD_ZERO(&readset);
      FD_SET(serverSocket, &readset);
      max_sd = serverSocket;
      size_t request_size = ADD_SIZE_STRUCTURE;

      for(auto& player: players) {
        int sd = player->socket;
        if (sd > 0) {
          FD_SET(sd, &readset);
        }
        if (sd > max_sd) {
          max_sd = sd;
        }
        FD_SET(sd, &readset);
      }
      //select
      struct timeval tim;
      tim.tv_sec = 1;
      int select_answer = select(max_sd + 1, &readset, NULL, NULL, &tim);
      printf("get select response\n");
      if ((select_answer < 0) && (errno != EINTR)) { //EINTR = cought a signal
        perror("select error");
        exit(1);
      }
      
      //connection of new player
      if (FD_ISSET(serverSocket, &readset)) {
        int new_socket;
        if ((new_socket = accept(serverSocket, (struct sockaddr *) &servaddr, &addrlen)) < 0) {
          perror("accept failure");
        }
        ++max_players_number;
        players.emplace_back({max_players_number, new_socket});
        ++players_count;
        printf("accept succesful %d\n", new_socket);

        std::string s = static_cast<std::string>(x_min) + " " + static_cast<std::string>(x_max) + " " +
                static_cast<std::string>(y_min) + " " + static_cast<std::string>(y_max) + "\n";
        u32 ssize = s.size();

        if (send(new_socket, "new round is started\n", ssize, 0) != ssize) {
          perror("send failure");
          close(new_socket);
          players.pop_back();
          --players_count;
        }
      }

      //processing players input
      for (auto& player: players) {
        char buffer[1023];
        clear(&buffer, 1023);
        int sd = player->socket;
        if (FD_ISSET(sd, &readset)) {
          int count;
          if ((count = read(sd, buffer, 1023)) <= 0  || buffer[0] > '9' || buffer[0] < '0') {
            close(sd);
            players.erase(player);
            --players_count;
          }
          ld x = 0, y = 0;
          u32 i = 0;
          while (buffer[i] <= '9' || buffer[i] >= '0') {
            x *= 10; x += static_cast<int>(buffer[i]); ++i;
          }
          ++i;
          ans = 0.1;
          while (buffer[i] <= '9' || buffer[i] >= '0') {
            x += ans * buffer[i]; ans /= 10; ++i;
          }
          while (!(buffer[i] <= '9' || buffer[i] >= '0')) {
            ++i
          }
          while (buffer[i] <= '9' || buffer[i] >= '0') {
            y *= 10; y += static_cast<int>(buffer[i]); ++i;
          }
          ++i;
          ans = 0.1;
          while (buffer[i] <= '9' || buffer[i] >= '0') {
            y += ans * buffer[i]; ans /= 10; ++i;
          }
          table.addPoint(std::move(Point), player->number);
          ++make_turn;
        }
      } // end for
      if (make_turn == players_count && player_count != 0) {
        for(u32 i = 0; i < areas_number; ++i) {

        }
      }
    }
  }
};


#endif //CONTINUOUS_DOTS_TABLE_H