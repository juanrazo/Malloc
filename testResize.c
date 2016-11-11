#include "stdio.h"
#include "stdlib.h"
#include "myAllocator.h"
#include "sys/time.h"
#include <sys/resource.h>
#include <unistd.h>

double diffTimeval(struct timeval *t1, struct timeval *t2) {
  double d = (t1->tv_sec - t2->tv_sec) + (1.0e-6 * (t1->tv_usec - t2->tv_usec));
  return d;
}

void getutime(struct timeval *t)
{
  struct rusage usage;
  getrusage(RUSAGE_SELF, &usage);
  *t = usage.ru_utime;
}

int main() 
{
  void *p1, *p2, *p3, *p4;
  arenaCheck();
  p1 = firstFitAllocRegion(254);
  arenaCheck();
  p2 = firstFitAllocRegion(100);
  arenaCheck();
  p3 = firstFitAllocRegion(254);
  printf("\n\n---------------------------Addresses----------------------------\n");
  printf("p1:%8zx \tp2:%8zx \tp3:%8zx\n", p1, p2, p3);
  printf("---------------------------Addresses----------------------------\n\n");
  arenaCheck();
  freeRegion(p2);
  arenaCheck();
  p1 = resizeRegion(p1, 300);
  arenaCheck();
  printf("\n\n---------------------------Addresses----------------------------\n\n");
  printf("p1:%8zx \tp3:%8zx\n", p1, p3);
  printf("---------------------------Addresses----------------------------\n\n");
  p1 = resizeRegion(p1, 600);
  arenaCheck();
  printf("\n\n---------------------------Addresses----------------------------\n");
  printf("p1:%8zx \tp3:%8zx\n", p1, p3);
  printf("---------------------------Addresses----------------------------\n\n");
  arenaCheck();
  printf("\n\n---------------------------Addresses----------------------------\n");
  printf("p1:%8zx \tp3:%8zx\n", p1, p3);
  printf("---------------------------Addresses----------------------------\n\n");
  printf("\n---------------------------BEGIN TO FREE----------------------------\n\n");
  freeRegion(p3);
  arenaCheck();
  freeRegion(p1);
  arenaCheck();
  {				/* measure time for 10000 mallocs */
    struct timeval t1, t2;
    int i;
    getutime(&t1);
    for(i = 0; i < 10000; i++)
      if (firstFitAllocRegion(4) == 0) 
	break;
    getutime(&t2);
    printf("%d firstFitAllocRegion(4) required %f seconds\n", i, diffTimeval(&t2, &t1));
  }
  return 0;
}



