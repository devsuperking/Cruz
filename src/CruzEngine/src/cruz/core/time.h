#pragma once

class Time {
private:
    static float deltaTime;

public:
    static void SetDeltaTime(float dt);
    static float GetDeltaTime();
};