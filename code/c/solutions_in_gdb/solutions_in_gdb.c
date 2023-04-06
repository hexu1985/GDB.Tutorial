#include <stdlib.h>

int *x;

int main()
{
    x = (int *) malloc( 25*sizeof(int) );
    x[3] = 12;

    return 0;
}
