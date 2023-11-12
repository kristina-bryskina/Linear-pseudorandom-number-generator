#include "random_source.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <limits.h>
#include <sys/types.h>

const char s_for_random[] = "/dev/urandom";

RandomSource *
destroy_random(RandomSource *src)
{
	if (src != NULL){
		free(src->ops);
		if (close(src->fd) < 0){
			fprintf(stderr, "Error in closing\n");
			return NULL;
		}
		free(src);
		src = NULL;
	}
	return NULL;
}

double
next_random(RandomSource *src)
{
	int file = src->fd;
	if (read(file, &(src->current), sizeof(src->current)) != sizeof(src->current)){
		fprintf(stderr, "Error in reading\n");
		destroy_random(src);
		return 1;
	}
	unsigned long long cur = src->current;
	if (cur > 0){
		return (cur - 1.0) / ULLONG_MAX;
	} else {
		return 0.0;	
	}
}

RandomSource *
random_random_factory(const char *params)
{
	RandomSource *obj = calloc(1, sizeof(*obj));
	if (obj == NULL){
		return NULL;
	}
	obj->ops = calloc(1, sizeof(*(obj->ops)));
	if (obj->ops == NULL){
		free(obj);
		return NULL;
	}
	int file = open(s_for_random, O_RDONLY);
	if (file < 0){
		fprintf(stderr, "Error in opening\n");
        	free(obj->ops);
        	free(obj);
        	return NULL;
	}
	obj->fd = file;
	obj->ops->next = next_random;
	obj->ops->destroy = destroy_random;
	return obj;
}
