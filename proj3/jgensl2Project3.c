#include <stdio.h>
#include <stdlib.h>

void arrayCopy(const int* fromArray, int* toArray, int size);
void isort(int* arr, int size);
void lsearch(const int* arr, int size, int target, int* numComparisons);
int bsearch2( const int* array, int size, int target, int* numComparisons);

int main(){
    int val, numcomps, size = 0;
    int* firstarr, *secondarr;
    firstarr = (int*)(malloc( sizeof(int)*1));    
    
    //gather the input
    printf("Enter numbers; use value -999 to finalize array:\n");
    do{
	scanf("%d", &val);
	if( val == -999)
	    break;
	size++;
	firstarr = (int*)realloc( firstarr, sizeof(int)*size);
	firstarr[size-1] = val;
    }while(1);
    
    //allocate the 2nd array
    secondarr = (int*)(malloc(sizeof(int)*size));
    //copy the first array into the second
    arrayCopy(firstarr, secondarr, size);
    //sort the first array
    isort( firstarr, size);

    //perform searches on the input
    
    printf("Enter values to search for; use -999 to end:\n");
    do{
	scanf("%d", &val);
	if( val == -999)
	    break;

   	//bin search the sort array
   	bsearch2( firstarr, size, val, &numcomps);
   	printf("\tbsearch:comparrisons to find %d:%d\n", val, numcomps);
    	//liner search on unsorted array
	lsearch( secondarr, size, val, &numcomps);
    	printf("\tlsearch:comparrisons to find %d:%d\n\n", val, numcomps);

    }while(1);

    free(firstarr);
    free(secondarr);
    return 0;
}

void arrayCopy(const int* fromArray, int* toArray, int size){
    int counter;
    for( counter = 0; counter < size; counter++){
	toArray[counter] = fromArray[counter];
    }
}

void isort(int* arr, int size){
    int i, j, temp;
    for( i = 1; i < size; i++){
	j = i;

	while( j > 0 && arr[j] < arr[j-1]){
	    temp = arr[j];
	    arr[j] = arr[j-1];
	    arr[j-1] = temp;
	    j--;
	}
    }

}

void lsearch(const int* arr, int size, int target, int* numComparisons){
    *numComparisons = 0;
    int temp = size;
    for(temp = 0; temp < size; temp++){
	*numComparisons += 1;
   	if( arr[temp] == target){
	    printf("The value %d was found!\n", target);
	    return;
	}
    }

    printf("!!! The value %d WAS NOT FOUND!\n", target);
}


int bsearch2( const int* array, int size, int target, int* numComparisons){
    int hi = size-1, lo = 0, mid;
    *numComparisons = 0;    

    while( hi >= lo){
	mid = (lo+hi)/2;
        if( target < array[mid] ){
	    *numComparisons += 2;
	    hi = mid - 1;
	}
	else if( target > array[mid]){
	    *numComparisons += 3;
	    lo = mid + 1;
	}
	else{
	    *numComparisons += 4;
	    printf("The value %d was found!\n", target);
	    return mid;
	}
    }

    printf("!!! The value %d WAS NOT FOUND\n", target);
    return -1;
}
