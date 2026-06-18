#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "structures.h"

// Function used to shift the right part content of a pointer array to the left because a pointer was removed from the array (very specific)
void	shift_pointer_array(void** array, int start_index, int current_count)
{
    int i = start_index;
    while (i < current_count - 1)
    {
        array[i] = array[i + 1];
        i++;
    }
}
