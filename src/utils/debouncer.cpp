#include "debouncer.h"

// Define the static member here (only once in the .cpp file)
std::vector<Debounce *> Debounce::debounceInstances;

Debounce::Debounce(int delay) : debounceTime(delay), lastCallTime(0), pending(false)
{
    debounceInstances.push_back(this);
}

Debounce::~Debounce()
{
    debounceInstances.erase(std::remove(debounceInstances.begin(), debounceInstances.end(), this), debounceInstances.end());
}

void Debounce::call(std::function<void()> callback)
{
    this->callback = callback;
    unsigned long currentTime = millis();

    if (currentTime - lastCallTime >= debounceTime)
    {
        callback();
        lastCallTime = currentTime;
        pending = false;
    }
    else
    {
        pending = true;
    }
}

void Debounce::update()
{
    if (pending && millis() - lastCallTime >= debounceTime)
    {
        callback();
        lastCallTime = millis();
        pending = false;
    }
}

void Debounce::updateAll()
{
    for (size_t i = 0; i < debounceInstances.size(); i++)
    {
        debounceInstances[i]->update();
    }
}
