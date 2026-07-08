#pragma once

#include "singletons/Singleton.h"
#include <random>

class Random : public Singleton<Random> {
public:
    float randomFloatRange(float low, float high) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(low, high);
    return dis(gen);
    }
};