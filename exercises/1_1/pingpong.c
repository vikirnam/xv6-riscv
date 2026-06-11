#include <stdio.h>
#include <time.h>
#include <unistd.h>

#define N 100000

int main() {
  int ping[2];
  int pong[2];

  pipe(ping);
  pipe(pong);

  char buff[5];

  int pid = fork();

  if (pid == 0) {
    clock_t t;
    t = clock();
    for (int i = 0; i < N; i++) {
      close(pong[0]);
      close(ping[1]);
      read(ping[0], buff, 5);
      write(pong[1], "PONG", 5);
    }
    t = clock() - t;
    double time_taken = ((double)t) / CLOCKS_PER_SEC;
    printf("No of exchanges per seconds: %.3lf\n", N / time_taken);
  } else if (pid > 0) {
    for (int i = 0; i < N; i++) {
      close(pong[1]);
      close(ping[0]);
      write(ping[1], "PING", 5);
      read(pong[0], buff, 5);
    }
  } else {
    fprintf(stderr, "fork error\n");
    return 1;
  }
  return 0;
}
