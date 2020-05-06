#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/ioctl.h>
#include <math.h>

struct _beweglich
{
  int bewegung_x;
  int bewegung_y;
  int vorzeichen_x;
  int vorzeichen_y;
  int r[100];

};

typedef struct _beweglich beweglich;

void rando(int min, int max, beweglich* objekte){
  int range, result, cutoff;

 	for (int i = 0; i < 50; i++){
    	range = max - min + 1;
    	cutoff = (RAND_MAX / range) * range;
    	 do {
        	result = rand();
    	} while (result >= cutoff);
 		   objekte->r[i] = result % range + min;
 	}
}


int main(void) {
  beweglich* objekte = (beweglich*) malloc(sizeof(beweglich));
  for (int j = 0; j < 10; j++){
    rando(-4, 4, objekte);
    int z = objekte->r[j];
    printf("%d\n", z);
  }
  for (int j = 0; j < 10; j++){
    rando(1, 4, objekte);
    int z = objekte->r[j];
    printf("%d\n", z);
  }
 	free(objekte);

}
