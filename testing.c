#include <stdio.h>
#include <stdlib.h>
#define MAX 10
void print ( int a[][4] ) {
    int r,c;
    for ( r = 0 ; r < MAX ; r++ ) {
        for ( c = 0 ; c < 4 ; c++ ) {
            printf( "%d ", a[r][c] );
        }
        printf( "\n" );
    }
}
 
int cmp ( const void *pa, const void *pb ) {
    const int (*a)[4] = pa;
    const int (*b)[4] = pb;
    if ( (*a)[3] < (*b)[3] ) return -1;
    if ( (*a)[3] > (*b)[3] ) return +1;
    return 0;
}
 
int main ( ) {
    int a[MAX][4];
    int r,c;
    for ( r = 0 ; r < MAX ; r++ ) {
        for ( c = 0 ; c < 4 ; c++ ) {
            a[r][c] = rand() % 100;
        }
    }
    print( a );
    printf("\n");
    qsort( a, MAX, sizeof a[0], cmp );
    print( a );
    return 0;
}