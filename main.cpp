
#include "geometry.h"
#include "table.h"
#include "connection.h"
#include <thread>


int main(int argc, char *argv[]) {
  u32 n = 1000, m = 1000, player_number = 2, area_number = 4;
  

  char* input[20];
  clear(input, 20);
  bool isStarted = false;
  pthread_t player_comp;
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
      Table table(n, m, player_number, area_number);
      std::thread(table.connect).detach();

      std::vector<Bot> bots(player_number, 1);
      for(u32 i = 0; i < bots.size(); ++i) {
        bots[i].team_number = i + 1;
        std::thread(bots[i].connect).detach();
      }

    } else if (isRun(input)) {

    } else if (isStop(input)) {

    } else if (isQuit(input)) {

    }
    clear(input, 20);
  }
  return 0;
}