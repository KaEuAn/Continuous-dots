//
// Created by Eugene on 14.11.2018.
//

#ifndef CONTINUOUS_DOTS_TABLE_H
#define CONTINUOUS_DOTS_TABLE_H

#endif //CONTINUOUS_DOTS_TABLE_H

#include "geometry.h"//
// Created by Eugene on 30.11.2018.
//

#ifndef INC_4_SEMESTER_CONNECTION_H
#define INC_4_SEMESTER_CONNECTION_H

#endif //INC_4_SEMESTER_CONNECTION_H

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

void clear(char* input, u32 size) {
  for(u32 i = 0; i < size; ++i) size[i] = 0;
}

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

class Player{
public:
  u32 number;
  int socket;
};



class Area {
public:
  std::vector<DotArea>;

};

class Table {

private:
  double x_min, x_max, y_min, y_max;
  u32 max_players_number;
  u32 areas_number;
  std::vector<Area> areas;
  std::list<Player> players;
  players_count;

public:
  Table(u32 a) : x_min(0), x_max(1e6), y_min(0), y_max(1e6), max_players_number(0), areas_number(a), areas(areas_number)
          , players_count(0) {}
  Table(u32 n, u32 m, u32 pla_n, u32 a) : x_min(0), x_max(n), y_min(1e6), y_max(m), max_players_number(0), areas_number(a)
          , areas(areas_number), players_count(0) {}

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
          while (true) {
            if (buffer[i] <= '9' || buffer[i] >= '0') {
              x *= 10; x += static_cast<int>(buffer[i]);
            }
          }
        }
      }
    }
  }
};