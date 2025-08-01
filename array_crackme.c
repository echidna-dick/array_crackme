#include <stdio.h>

int main(){
    int num[5];
    int i;
    int sum = 0;

    for (i = 0; i < 5; i++){
        num[i] = i * 10;
    }

    for(i = 0; i < 5; i++){
        sum += num[i];
    }

    printf("the sum of array elements id: %d\n", sum);
    return 0;
}
