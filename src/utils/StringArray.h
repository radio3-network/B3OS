#ifndef STRINGARRAY_H
#define STRINGARRAY_H

#define MAX_STR_LEN 100

/* 



    StringArray *myStrings = StringArray_create(5);

    StringArray_add(myStrings, "Hello");
    StringArray_add(myStrings, "World");
    StringArray_add(myStrings, "How");
    StringArray_add(myStrings, "Are");
    StringArray_add(myStrings, "You?");

    printf("Size of array: %d\n", StringArray_size(myStrings));

    StringArray_delete(myStrings, 2);

    printf("Size of array after deleting element at index 2: %d\n", StringArray_size(myStrings));

    StringArray_destroy(myStrings);

 */

typedef struct {
    const char **strings;
    int size;
    int capacity;
} StringArray;

/*
 * Function: StringArray_create
 * ----------------------------
 * Creates a new StringArray with the specified capacity.
 *
 * capacity: The initial capacity of the array.
 *
 * returns: A pointer to the newly created StringArray.
 */
static StringArray* StringArray_create(int capacity);

/*
 * Function: StringArray_destroy
 * -----------------------------
 * Frees the memory allocated to the specified StringArray.
 *
 * stringArray: A pointer to the StringArray to be destroyed.
 */
static void StringArray_destroy(StringArray *stringArray);

/*
 * Function: StringArray_add
 * -------------------------
 * Adds a new string to the end of the specified StringArray.
 *
 * stringArray: A pointer to the StringArray to which the string will be added.
 * str: The string to be added to the array.
 *
 * returns: The index of the newly added string in the array.
 */
static int StringArray_add(StringArray *stringArray, const char *str);

/*
 * Returns the string at the given index in the StringArray
 */
static const char* StringArray_get(StringArray *stringArray, int index);

/*
 * Function: StringArray_delete
 * ----------------------------
 * Deletes the string at the specified index from the specified StringArray.
 *
 * stringArray: A pointer to the StringArray from which the string will be deleted.
 * index: The index of the string to be deleted.
 *
 * returns: 1 if the deletion was successful, 0 otherwise.
 */
static int StringArray_delete(StringArray *stringArray, int index);

/*
 * Function: StringArray_size
 * --------------------------
 * Gets the number of strings in the specified StringArray.
 *
 * stringArray: A pointer to the StringArray whose size will be returned.
 *
 * returns: The number of strings in the array.
 */
static int StringArray_size(StringArray *stringArray);

#endif /* STRINGARRAY_H */