enum
{
    BASE = 10,
    MASK = 0x7FFFFFFF,
    M = 1u << 31,
    INC = 12345,
    MULT = 1103515245
};

typedef struct RandomSource RandomSource;

typedef struct RandomSourceOperations {
    RandomSource *(*destroy)(RandomSource *);
    double (*next)(RandomSource *);
} RandomSourceOperations;

typedef struct RandomSource {
    unsigned long long current;
    int fd;
    RandomSourceOperations *ops;
} RandomSource;
