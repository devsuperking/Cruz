#include "time.h"

float Time::deltaTime = 0.0f;

void Time::SetDeltaTime(float dt)
{
    deltaTime = dt;
}

float Time::GetDeltaTime()
{
    return deltaTime;
}