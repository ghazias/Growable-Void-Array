#ifndef RESIZEABLEARRAY_GROWABLEARRAY_H_
#define RESIZEABLEARRAY_GROWABLEARRAY_H_

#include <stdbool.h>
#include <stddef.h>


struct GrowableArray {
    void** data; // an array of void*
    size_t size; // total actual elements
    size_t capacity; // total possible elements
    int (*cmp)(const void*, const void*); //comparison function
    void (*free_element)(void*); // function to delete an element (used in free_array)
    void* (*duplicate_element)(const void*); // function to duplicate an element
};

// Creates an array of elements with a comparison function, and a destruction function
struct GrowableArray* create_array(
    int (*cmp)(const void*, const void*),
    void (*free_element)(void*),
    void* (*duplicate_element)(const void*));
    
void for_each(struct GrowableArray* array, void (*func)(void*));

struct GrowableArray* map(struct GrowableArray* array, void* (*func)(void*));

struct GrowableArray* filter(struct GrowableArray* array, int (*func)(void*));

// free all associated data. You must free both the GrowableArray struct, and any
// data you allocated within it. Every malloc() needs a free()
// Additionally, you have to free_element() each pointer in the array
void free_array(struct GrowableArray* array);

// return true if the array contains a given Element
bool contains(const struct GrowableArray* array, const void* element);

// return largest element in the array
void* max_element(const struct GrowableArray* array);

// return smallest element in the array
void* min_element(const struct GrowableArray* array);

// return true if the two arrays have the same contents
bool equals(const struct GrowableArray* lhs, const struct GrowableArray* rhs);

// appends element to end of array if size < capacity, doubles space if not.
// takes ownership of element
void append(struct GrowableArray* array, void* element);

void insert(struct GrowableArray* array, void* element, size_t index);

// caller must free the returned pointer
void* pop(struct GrowableArray* array);

struct GrowableArray* duplicate(const struct GrowableArray* array);


#endif
