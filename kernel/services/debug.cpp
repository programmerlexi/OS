#include "debug.h"

char* location;
char* scope;

char* get_debug_location() {
    return location;
}

void enter_debug_scope(char* scope) {
    location = scope;
}

void exit_debug_scope() {
    location = (char*)"";
}