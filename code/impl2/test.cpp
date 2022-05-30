#include <stdio.h>


struct t {
    int n; // 4 octets
    char c; // 1 octet + 3 padding
    int arr[];
};


int main()
{
    printf("%i", sizeof(t)); // 8
}