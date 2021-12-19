#include <oto.h>

SliceI64 *new_slice_i64(VectorI64 *vec, int64_t start, int64_t end) {
    if (IS_NULL(vec)) {
        return NULL;
    } else if (start > end) {
        return NULL;
    }
    
    SliceI64 *slice = MYMALLOC1(SliceI64);
    if (IS_NULL(slice)) {
        return NULL;
    }
    slice->data = &(vec->data[start]);
    slice->abs_idx = start;

    if (end > vec->length) {
        slice->length = vec->length;
    } else {
        slice->length = end - start;
    }

    return slice;
}

SliceI64 *new_slice_i64_from_slice(SliceI64 *org_slice, int64_t start, int64_t end) {
    if (IS_NULL(org_slice)) {
        return NULL;
    }

    SliceI64 *slice = MYMALLOC1(SliceI64);
    if (IS_NULL(slice)) {
        return NULL;
    }
    slice->data = &(org_slice->data[start]);
    slice->abs_idx = org_slice->abs_idx + start;

    if (end > org_slice->length) {
        slice->length = org_slice->length;
    } else {
        slice->length = end - start;
    }

    return slice;
}

void free_slice_i64(SliceI64 *slice) {
    free(slice);
}

int64_t slice_i64_get(SliceI64 *slice, int64_t idx) {
    if (idx >= slice->length || idx < 0) {
        return 0;
    }
    return slice->data[idx];
}
