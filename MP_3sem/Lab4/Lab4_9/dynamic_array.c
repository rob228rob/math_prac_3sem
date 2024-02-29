#include <dynamic_array.h>
#include <stdlib.h>

Vector* create_vector() {
    Vector* vector = (Vector*)malloc(sizeof(Vector)*1);
    if (vector == NULL) {
        return NULL;
    }
    vector->capacity = 10;
    vector->array = (tdata*)malloc(sizeof(tdata)*vector->capacity);
    if (vector->array == NULL) return NULL;
    vector->size = 0;

    return vector;
}

int is_empty_vec(Vector* vec) {
    if (vec == NULL) return 2;

    if (vec->size == 0) return 1;
    return 0;
}

int b_search_by_ind(Vector* vec, int index, tdata* res) {
    if (vec == NULL || index <= 0 || res == NULL) return 2;

    int left = 0;
    int right = vec->size;
    while (left <= right)
    {
        int mid = left + (right - left) / 2;
        // if (array[mid].is_delete == 1)
        //     continue;

        if (mid == index)
        {
            *res = vec->array[mid];
            return 0;
        }
        else if (mid <= index)
        {
            left = mid + 1;
        }
        else
        {
            right = mid - 1;
        }
    }
    // if element doesn't exist
    return -1;
}

int push_back(Vector* vec, tdata data) {
    if (vec == NULL) return 2;
    size_t size = vec->size;
    if (vec->size + 1 >= vec->capacity) {
        //REALLOC SIZE
        vec->capacity *= 2;
        tdata* temp = (tdata*)realloc(vec->array, vec->capacity*sizeof(tdata));
        if (temp == NULL) return 2;
        vec->array = temp;
    } else {
        vec->array[vec->size] = data;
        vec->size++;
    }

    return 0;
}


int pop_back(Vector* vec) {
    if (vec == NULL || vec->size == 0) return 2;

    vec->size--;
    vec->array[vec->size] = 0;

    return 0;
}


int pop_by_ind(Vector* vec, int index);
int pop_by_ind(Vector* vec, int index);

void delete_vector(Vector* vec) {
    free(vec->array);
    free(vec);

    return;
}