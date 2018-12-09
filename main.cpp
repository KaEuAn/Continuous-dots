
#include "geometry.h"
#include "table.h"
#include "connection.h"
#include <thread>
#include <mutex>
#include <functional>
#include <atomic>

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

int main(int argc, char *argv[]) {
  u32 n = 1000, m = 1000, player_number = 2, area_number = 4, count;
  std::condition_variable cond_var;
  
  char* input[20];
  clear(input, 20);
  bool isStarted = false;
  pthread_t player_comp;
  
  Table table(n, m, area_number);
  std::vector<Bot> bots(player_number, 1);
  std::vector<std::mutex> mutexes(area_number + 1);
  std::vector<u32> bots_iterations(player_number, 0);
  std::vector<u32> bots_made(player_number, 0);
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
      std::thread(table.connect).detach();

      for(u32 i = 0; i < bots.size(); ++i) {
        bots[i].team_number = i + 1;
        std::thread(bots[i].connect, std::ref(bots_iterations), std::ref(bots_made), i, std::ref(cond_var)).detach();
      }

    } else if (count = isRun(input)) {
      for(auto it: bots_iterations) {
        (*it) += count;
      }
      table.cond_var.notify_all();
    } else if (isStop(input)) {
      
    } else if (isQuit(input)) {

    } else { //isStatus

    }
    clear(input, 20);
  }
  return 0;
}