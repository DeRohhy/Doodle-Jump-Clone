#pragma once

template<typename type>
class Singleton {
public:
    Singleton(const Singleton&) = delete;
    Singleton(Singleton&&) = delete;
    Singleton& operator=(const Singleton&) = delete;
    Singleton& operator=(const Singleton&&) = delete;

    static type& getInstance() {
        static type instance;
        return instance;
    }
protected:
    Singleton() = default;
};