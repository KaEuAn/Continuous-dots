#pragma once

#include "bots.hpp"
#include "game.cpp"
#include <arpa/inet.h>

std::string toString(const Point& x)
{
    std::stringstream ss("");
    ss << x.x << ' ' << x.y;
    return ss.str();
}

Point Bot::makeTurn(u32 n, u32 m){
    return Point(n * std::rand(), m * std::rand());
  }

void Bot::connect(Game* game, u32 bot_thread_number) {
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
        return; 
    } 
    if (::connect(Socket, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
    { 
        printf("Bot, Connection Failed \n"); 
        return; 
    }

    while(true) {
      char buffer[1024] = {0};
      int count;
      if((count = read(Socket, buffer, 1024)) <= 0) {
        break;
      }
      std::string answer = toString(makeTurn(game->n, game->m));
      char resp[1000];
      for(u32 i = 0; i < answer.size(); ++i) {
        resp[i] = answer[i];
      }
      std::unique_lock<std::mutex> lock(game->bot_mutexes[bot_thread_number]);
      while((game->bots_iterations[bot_thread_number] == game->bots_made[bot_thread_number] || game->isStopped) && !game->isQuited) {
        // т.к. либо они изначально равны, либо мы остановились и нужно приравнять, т.к. мы под мьютексом - всё ок
        game->bots_iterations[bot_thread_number] = game->bots_made[bot_thread_number];
        if (game->isStopped) {
          std::cout << "Bot " << bot_thread_number << " is stopped";
        }
        game->cond_var.wait(lock);
      }
      lock.unlock();
      if (game->isQuited) {
        return;
      }
      send(Socket, resp, answer.size(), 0);
    }

  }