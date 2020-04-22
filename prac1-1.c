#include <stdio.h>

swap(int x, int y) {
   int tmp;

   tmp = x;
   x = y;
   y = tmp;
}

int main(void) {
    int a = 3;
    int b = 5;

    swap(a, b);
}