#include "random_source.h"
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

const char name_linear[] = "linear";
const char name_random[] = "random";
const char name_linear_fact[] = "random_linear_factory";
const char name_random_fact[] = "random_random_factory";

enum
{
	AMOUNT_ARGS = 3,
	MAX_AMOUNT = 100000
};

int
main(int argc, char **argv)
{
	if (argc < AMOUNT_ARGS + 1){
		fprintf(stderr, "Wrong amount of arguments\n");
        	return 2;
	}
	void *handle = dlopen(NULL, RTLD_LAZY);
	if (handle == NULL){
		fprintf(stderr, "Error in opening\n");
        	return 3;
	}

	RandomSource *(*Function)(const char *) = NULL;
	if (strcmp(argv[1], name_linear) == 0){
		Function = dlsym(handle, name_linear_fact);
	} else if (strcmp(argv[1], name_random) == 0){
		Function = dlsym(handle, name_random_fact);
	} else {
		fprintf(stderr, "Wrong generator\n");
		return 4;
	}

	RandomSource *obj = Function(argv[3]);
	char *eptr = NULL;
	errno = 0;
	long n = strtol(argv[2], &eptr, BASE);
	if (errno || *eptr || eptr == argv[2] || n > MAX_AMOUNT || n < 0){
		fprintf(stderr, "Wrong amount\n");
		return 5;
	}
	int amount = (int) n;
	for (int i = 0; i < amount; ++i){
		printf("%.10g\n", obj->ops->next(obj));
	}
	obj->ops->destroy(obj);
	return 0;
}
