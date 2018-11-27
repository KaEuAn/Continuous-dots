#include <iostream>
#include "geometry.h"
#include "table.h"

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


int main(int argc, char *argv[]) {
  u32 n = 1000, m = 1000, player_number = 5, area_number = 4;
  
  Table table(n, m, player_number, area_number);

  char* input[20];
  clear(input, 20);
  while(gets(input)) {
    if (isStart(input)) {
        if (argc == 1) {
          n = static_cast<int>(argv[1]);
          m = n;
        }
        if (argc == 2) {
          n = static_cast<int>(argv[1]);
          m = static_cast<int>(argv[2]);
        }

    } else if (isRun(input)) {

    } else if (isStop(input)) {

    } else if (isQuit(input)) {

    }
    clear(input, 20);
  }
  return 0;
}