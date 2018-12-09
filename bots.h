//
// Created by Eugene on 27.11.2018.
//

#ifndef CONTINUOUS_DOTS_BOTS_H
#define CONTINUOUS_DOTS_BOTS_H


#include <cstdlib>
#include "geometry.h"
#include <string>
#include <condition_variable>

template<Typename T>
std::string toString(T val, T val2)
{
    std::stringstream ss("");
    ss << val << ' ' << val2;
    return ss.str();
}

Class Bot{
public:
  u32 team_number;
  Bot(u32 n) : team_number(n) {}

  Point makeTurn(u32 n, u32 m){
    return {n * std::rand(), m * std::rand()};
  }

  void connect(Table* table, bot_thread_number) {
    struct sockaddr_in address;
    struct sockaddr_in serv_addr; 
    int Socket;
    if ((Socket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
      perror("Class Bot, server socket failure");
      exit(1);
    }
    memset(&serv_addr, '0', sizeof(serv_addr)); 
    serv_addr.sin_family = AF_INET; 
    serv_addr.sin_port = htons(8080);

    // Convert IPv4 and IPv6 addresses from text to binary form and save in sin_addr
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)  
    { 
        printf("Bots, Invalid address/Address not supported \n"); 
        return -1; 
    } 
    if (connect(Socket, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
    { 
        printf("Bot, Connection Failed \n"); 
        return -1; 
    }

    while(true) {
      char buffer[1024] = {0};
      if((count = read(Socket, buffer, 1024)) <= 0) {
        break;
      }
      std::string answer = toString(makeTurn());
      char resp[1000];
      for(u32 i = 0; i < answer.size(); ++i) {
        resp[i] = answer[i];
      }
      std::unique_lock<std::mutex> lock(table->bots_mutexes[bot_thread_number]);
      while(table->bots_iterations[bot_thread_number] == table->bots_made[bot_thread_number] || table->isStopped) {
        // т.к. либо они изначально равны, либо мы остановились и нужно приравнять, т.к. мы под мьютексом - всё ок
        table->bots_iterations[bot_thread_number] = table->bots_made[bot_thread_number];
        if (table->isStopped) {
          std::cout << "Bot " << bot_thread_number << " is stopped";
        }
        cond_var.wait(lock);
      }
      lock.unlock();
      send(Socket, resp, answer.size());
    }

  }
};


#endif //CONTINUOUS_DOTS_BOTS_H