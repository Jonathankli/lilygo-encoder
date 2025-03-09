#ifndef DEBOUNCER_H
#define DEBOUNCER_H

#include <Arduino.h>
#include <vector>
#include <functional>

class Debounce
{
private:
    unsigned long debounceTime;
    unsigned long lastCallTime;
    bool pending;
    std::function<void()> callback;

    // Declare the static member properly (but don't define it here)
    static std::vector<Debounce *> debounceInstances;

public:
    Debounce(int delay);
    ~Debounce();
    void call(std::function<void()> callback);
    void update();

    static void updateAll();
};

#endif // DEBOUNCER_H
