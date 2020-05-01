#include <stdio.h>

void swap(int x, int y) {
   int tmp;

   tmp = x;
   x = y;
   y = tmp;

   printf("x = %d\ny = %d", x, y);
}

int main(void) {
    int a = 3;
    int b = 5;

    swap(a, b);
}
