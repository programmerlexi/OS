#include "math.h"

int abs(int x) {
    if (x < 0) {
        return -x;
    }
    return x;
}

int min(int x, int y) {
    if (x < y) {
        return x;
    }
    return y;
}

int max(int x, int y) {
    if (x > y) {
        return x;
    }
    return y;
}

int clamp(int x, int min, int max) {
    if (x < min) {
        return min;
    }
    if (x > max) {
        return max;
    }
    return x;
}

float clamp(float x, float min, float max) {
    if (x < min) {
        return min;
    }
    if (x > max) {
        return max;
    }
    return x;
}

int sign(int x) {
    if (x < 0) {
        return -1;
    }
    if (x > 0) {
        return 1;
    }
    return 0;
}

int round(float x) {
    return (int) (x + 0.5f);
}

float floor(float x) {
    return (float) (int) x;
}

float ceil(float x) {
    return (float) (int) x + 1;
}

float fract(float x) {
    return x - floor(x);
}

int pow(int x, int i) {
    int r = x;
    for (int t = 1; t < i; t++) {
        r = r*x;
    }
    return r;
}

int sqrt(int x) {
    long r = 0;
    while (pow(r,2) <= x) {
        r++;
    }
    r--;
    return (int)r;
}