#include "../includes/dynamic_array.h"

array_t create_array(size_t size){
    array_t array;
    array.items = malloc(sizeof(Track)* size); 
    array.count = 0; 
    array.size = size;
    return array;
}

void delete_array(array_t *array){
    free(array -> items);
    array -> items = NULL;
    array -> count = 0;
    array -> size = 0;
}

static void push_item(array_t *array, Track item){
    array -> items[array -> count] = item;
    array -> count++;
}

void append(array_t *array, Track item){
    if ((array -> count + 1) < array -> size ){
	push_item(array, item);
    } else {
	Track *tmp = realloc(array -> items, sizeof(Track) * array -> size * 2);
	if (tmp != NULL){
	    array -> items = tmp;
	    array -> size *= 2;
	    push_item(array, item);
	}
    }
}



