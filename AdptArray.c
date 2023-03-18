#include <stdio.h>
#include <stdlib.h>
#include "AdptArray.h"

// definition of the struct defined in book.h
typedef struct AdptArray_
{
    PElement* data; // the memory of the adpt
    COPY_FUNC copy;
    DEL_FUNC delete;
    PRINT_FUNC print;
    int size; // the number of elements we added
    int maxSize; // the size of the all array including empty places 
} AdptArray;


PAdptArray CreateAdptArray(COPY_FUNC copy, DEL_FUNC delete, PRINT_FUNC print)
{
    // creating a new adpt and initializing all the memory to be 0 
    PAdptArray adpt = (PAdptArray) calloc(1,sizeof(AdptArray));
    if (adpt == NULL) {return NULL;}// condition to check if an error occur during the creation 
    // initializing all the value 
    adpt->maxSize = 0;
    adpt->size = 0;
    adpt->data = NULL;

    // init the function of the object used in the array 
    adpt->copy = copy;
    adpt->delete = delete;
    adpt->print = print;
    return adpt;
}


void DeleteAdptArray(PAdptArray adpt)
{

    if (adpt == NULL ) { printf("FAIL");}
    else {
        //run over all our struct and free all the data in
        for (int i = 0  ; i < adpt->maxSize; i++)
        {
            //deleting only for the NON-NULL values 
            if (adpt->data[i] != NULL) 
            {
                adpt->delete(adpt->data[i]);
            }else {
                free(adpt->data[i]);
            }

        }

    }
    // we free our memory that we allocated in the create function
    free(adpt->data);
    free(adpt);
}

Result SetAdptArrayAt(PAdptArray adpt, int index, PElement newObj)
{
    if (adpt == NULL || index < 0) { return FAIL; } // the adpt is empty or the index is negative
    if (index >= adpt->maxSize) //  the index is bigger than the max-size of the adpt
    {
        int newMaxSize = ( index + 1 ) * 2;
        // copy the old data to another 
        PElement* newData = (PElement*) realloc(adpt->data, newMaxSize * sizeof(PElement));
        // we set to NULL all the new Data cells we create
        for (int i = adpt->maxSize; i< newMaxSize ; i++) {
            newData[i]=NULL;
        }

        if (newData == NULL) { return FAIL; }
        adpt->maxSize = newMaxSize;
        adpt->data = newData;
    }

    if (index > adpt->size) { adpt->size = index+1; } // the size is now the bigegr index plus one to match the start with 0
    if (adpt->data[index] != NULL) {
        adpt->delete(adpt->data[index]); 
        adpt->data[index] = NULL;
    } // delete the old object that was there by the new one 
    adpt->data[index] = adpt->copy(newObj);

    return SUCCESS;
}

PElement GetAdptArrayAt(PAdptArray adpt, int index)
{
    // if the adpt is empty or the index is bigger or the index is under 0 or if the value in is null 
    if (adpt == NULL || adpt->size <= index || index < 0 || (adpt->data[index] == NULL) ) { return NULL; }

    return  adpt->copy(adpt->data[index]);
}

int GetAdptArraySize(PAdptArray adpt )
{
    if (adpt == NULL) { return -1; }
    return  adpt->size;
}

void PrintDB(PAdptArray adpt)
{
    if (adpt == NULL) { printf("FAIL"); }
    for (int i = 0; i < adpt->size; i++) 
    {
        if (adpt->data[i] != NULL) { adpt->print(adpt->data[i]); }
    }
}



