#include "growable_array.h"
#include <stdlib.h>
#include <string.h>

static void grow_array(struct GrowableArray* array) {
    array->capacity *= 2; // double array capacity
    array->data = realloc(array->data, array->capacity * sizeof *array->data);
}

struct GrowableArray* create_array(
    int (*cmp)(const void*, const void*),
    void (*free_element)(void*),
    void* (*duplicate_element)(const void*)) {
        struct GrowableArray* output_array = malloc(sizeof *output_array);
        output_array->size = 0; // set initial real occupancy to 0
        output_array->capacity = 1; // set current max occupancy to 2
        output_array->data = NULL;
        grow_array(output_array);
        output_array->cmp = cmp;
        output_array->free_element = free_element;
        output_array->duplicate_element = duplicate_element;

        return output_array;
    }

void free_array(struct GrowableArray* array) {
    for (size_t i = 0; i < array->size; i++) {
        array->free_element(array->data[i]);

    }
    free(array->data);
    free(array);
}

void for_each(struct GrowableArray* array, void (*func)(void*)) {
    for (size_t i = 0; i < array->size; i++) {
        func(array->data[i]);
    }
}

struct GrowableArray* map(struct GrowableArray* array, void* (*func)(void*)) {
    struct GrowableArray* mapped_array = create_array(array->cmp, array->free_element, array->duplicate_element);
    for (size_t i = 0; i < array->size; i++) {
        append(mapped_array, func(array->data[i]));
    }
    
    return mapped_array;
}

struct GrowableArray* filter(struct GrowableArray* array, int (*predicate)(void*)) {
    struct GrowableArray* filtered_array = create_array(array->cmp, array->free_element, array->duplicate_element);
    for (size_t i = 0; i < array->size; i++) {
        if (predicate(array->data[i])) {
            append(filtered_array, array->data[i]);
        }
    }
    
    return filtered_array;
}

bool contains(const struct GrowableArray* array, const void* element) {
    for (size_t i = 0; i < array->size; i++) {
        if (array->cmp(array->data[i], element) == 0) {
            return true; // element located
        }
    }
    return false; // check did not pass
}

void* max_element(const struct GrowableArray* array) {
    void* max = array->data[0];
    for (size_t i = 0; i < array->size; i++) {
        if (array->cmp(array->data[i], max) > 0) {
            max = array->data[i];
        }
    }
    return max;
}

void* min_element(const struct GrowableArray* array) {
    void* min = array->data[0];
    for (size_t i = 0; i < array->size; i++) {
        if (array->cmp(array->data[i], min) < 0) {
            min = array->data[i];
        }
    }
    return min;
}

void append(struct GrowableArray* array, void* element) {
    if (array->size == array->capacity) { // if current occupancy < capacity
        grow_array(array);
    }
    array->data[array->size++] = element;
}

void insert(struct GrowableArray* array, void* element, size_t index) {
    if (array->size == array->capacity) {
        grow_array(array);
    }
    memmove(&array->data[index + 1], &array->data[index], sizeof *array->data * (array->size - index));
    array->size++;

    // alternative to memmove
    /*
    for (size_t i = array->size - 1; i > index; i--) {
        array->data[i] = array->data[i-1];
    }
    */
    array->data[index] = element;
}

void* pop(struct GrowableArray* array) {
    --array->size;
    return array->data[array->size];
}

struct GrowableArray* duplicate(const struct GrowableArray* array) {
    struct GrowableArray* output_array = create_array(array->cmp, array->free_element, 
        array->duplicate_element);
    
    for (size_t i = 0; i < array->size; i++) {
        append(output_array, array->duplicate_element(array->data[i]));
    }
    
    return output_array;
}
