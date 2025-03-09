#pragma once

namespace M5MF
{
    enum class CTRL_MODE_LIST : uint16_t
    {
        STAY,
        POSITION,
        VELOCITY,
        TORQUE,
        // ADD YOUR CONTROL MODE
        POSIPOSI = 20,
        POSITRQ = 21,
        VELOTRQ = 22,
        CIRCPOSIPOSI = 23,
        CIRCPOSITRQ = 24,
        MOTION
    };
}

