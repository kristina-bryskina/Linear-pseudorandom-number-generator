#include "random_source.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

RandomSource *
destroy_linear(RandomSource *src)
{
    if (src != NULL) {
        free(src->ops);
        free(src);
        src = NULL;
    }
    return NULL;
}

double
next_linear(RandomSource *src)
{
    unsigned long long cur = src->current;
    cur = cur % M;
    src->current = (((cur * MULT) % M) + INC) % M;
    double res = (double) src->current / M;
    return res;
}

RandomSource *
random_linear_factory(const char *params)
{
    RandomSource *obj = calloc(1, sizeof(*obj));
    if (obj == NULL) {
        return NULL;
    }

    errno = 0;
    char *eptr = NULL;
    unsigned long long el = strtoull(params, &eptr, BASE);
    if (errno || *eptr || eptr == params) {
        fprintf(stderr, "Wrong value\n");
        free(obj);
        return NULL;
    }
    el = el & MASK;
    obj->current = el;
    obj->ops = calloc(1, sizeof(*(obj->ops)));
    if (obj->ops == NULL) {
        free(obj);
        return NULL;
    }
    obj->ops->next = next_linear;
    obj->ops->destroy = destroy_linear;
    return obj;
}
