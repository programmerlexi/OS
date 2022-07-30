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

float sqrt(float x) {
    float t = 0;
    float sqrt = x/2;
    while (t != sqrt) {
        t = sqrt;
        sqrt = (x/t + t) / 2;
    }
    return sqrt;
}