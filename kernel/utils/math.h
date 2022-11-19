#pragma once
#include <stdint.h>

struct Point{
    int x;
    int y;
};

int abs(int x);
int min(int x, int y);
int max(int x, int y);
int clamp(int x, int min, int max);
float clamp(float x, float min, float max);
int sign(int x);
int round(float x);
float floor(float x);
float ceil(float x);
float fract(float x);
int pow(int x, int i);
int sqrt(int x);