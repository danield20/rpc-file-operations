#include "file.h"

#define SEARCH "SEARCH"
#define APPEND "APPEND"
#define END "END"
#define FILE "input.txt"

char delimiters[] = {' ', '.', ',', '-', ';', ':', '?', '!', '(', ')', '\n'};

static response YES_RESPONSE = {
    1,
    "YES"
};

static response NO_RESPONSE = {
    1,
    "NO"
};
