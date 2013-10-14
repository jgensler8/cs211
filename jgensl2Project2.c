#include <stdio.h>

int max( int, int);
void swap( int* , int*);

int main( int argc, char* argv[] ){
    int num1, num2, temp;

    printf("My name is Jeff Genlser. My UIN is 679010474\n");
    printf("please enter two integers: ");
    scanf(" %d %d", &num1, &num2);
    
    //find and print the larger of two numbers
    temp = max( num1, num2);
    printf("The value returned is: %d\n", temp);


    //swap two numbers and show they have swapped
    printf("The numbers before the swap are %d, %d\n", num1, num2);
    swap( &num1, &num2);
    printf("The numbers after the swap are %d, %d\n", num1, num2);

    return 0;
}

int max(int one, int other){
    if( one > other)
        return one;
    else if( other > one)
        return other;
    else{
        printf("The numbers are equal!\n");
        return 0;
    }
}

void swap( int *one, int *other){
    int temp;
    
    temp = *one;
    *one = *other;
    *other = temp;

    return;
}   

