#pragma once

class Singleton {
public:
    Singleton(const Singleton&) = delete;
    Singleton(Singleton&&) = delete;
    Singleton& operator=(const Singleton&) = delete;
    Singleton& operator=(const Singleton&&) = delete;

    static Singleton& getInstance() {
        static Singleton instance;
        return instance;
    }
protected:
    Singleton() = default;
};