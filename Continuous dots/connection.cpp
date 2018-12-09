//
// Created by Eugene on 30.11.2018.
//
#pragma once

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

void clear(char* input, u32 size) {
  for(u32 i = 0; i < size; ++i) size[i] = 0;
}

bool isStart(char* input) {
  return input[0] == 's' && input[1] == 't' && input[2] == 'a' && input[3] == 'r' && input[4] == 't'
}
bool isStop(char* input) {
  return input[0] == 's' && input[1] == 't' && input[2] == 'o' && input[3] == 'p'
}
bool isQuit(char* input) {
  return input[0] == 'q' && input[1] == 'u' && input[2] == 'i' && input[3] == 't'
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

    for(size_t i = 0; i < request_size; ++i) {
      int sd = heroesStructs.p[i].socket;
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

    if (FD_ISSET(serverSocket, &readset)) {
      int new_socket;
      if ((new_socket = accept(serverSocket, (struct sockaddr *) &servaddr, &addrlen)) < 0) {
        perror("accept failure");
      }
      printf("accept succesful %d\n", new_socket);
      getLogin(new_socket);
      //add new_socket

      std::string s = static_cast<std::string>()

      if (send(heroesStructs.p[i].socket, "new round is started\n", 21, 0) != 21) {
        perror("send failure");
        close(heroesStructs.p[i].socket);
        heroesStructs.p[i].status = -1;
      }
    }

    for (size_t i = 0; i < request_size; ++i) {
      char buffer[1023];
      clear(&buffer, 1023);
      int sd = heroesStructs.p[i].socket;
      if (FD_ISSET(sd, &readset)) {
        printf("start processing %d", heroesStructs.p[i].socket);
        int count;
        if ((count = read(sd, buffer, 1023)) <= 0  || buffer[0] > '9' || buffer[0] < '0') {
          close(sd);
          //delete from
        }
        //add point
      }
    }
  }
}