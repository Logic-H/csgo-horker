#pragma once

#include "../sdk/vector.h"

#include <cfloat>
#include <cmath>
#include <complex>

#define M_PI 3.14159265358979323846
#define M_PI_F ((float)(M_PI))
#define DEG2RAD( x )  ( (float)(x) * (float)( M_PI_F / 180.f ) )
#define RAD2DEG( x )  ( (float)(x) * (float)( 180.f/M_PI_F ) )

class HMath {
    public:
        static void MakeVector(Vector angle, Vector& vector);
        static float Dot(const Vector& v1, Vector& v2);
        static Vector VectorAngles(const Vector &dir);
        static Vector CalcAngle(const Vector& localPos, const Vector &targetPos);
        static Vector ClampAngle(const Vector& v);
        static float GetFov(const Vector& eyePos, const Vector& localHeadPos, const Vector& targetHeadPos);
        static float Clampf(float val, float low, float high, float correction = 0.4f);
    private:
        static constexpr double PI = 3.14159265358979323846;
        static constexpr double RADPI = 180.0f / PI;
};

inline float HMath::Clampf(float val, float low, float high, float correction)
{
    if (val < -correction && val > -1.f)
        return -1.f;
    if (val > correction && val < 1.f)
        return 1.f;
    if (val < low)
        return low;
    if (val > high)
        return high;
    return val;
}

inline Vector HMath::CalcAngle(const Vector& localPos, const Vector &targetPos)
{
    constexpr float fRadPi = 180.0f / (float)(HMath::PI);
    Vector aim = {};
    Vector delta = localPos - targetPos;

    double hyp = std::sqrt(delta.x * delta.x + delta.y * delta.y);
    aim.x = (float)(std::atan(delta.z / hyp) * fRadPi);
    aim.y = (float)(std::atan(delta.y / delta.x) * fRadPi);

    if (delta.x >= 0.0) {
        aim.y += 180.0f;
    }
    return aim;
}

inline float HMath::GetFov(const Vector& eyePos, const Vector& localHeadPos, const Vector& targetHeadPos)
{
    float dist = localHeadPos.DistTo(targetHeadPos);
    Vector ang = HMath::CalcAngle(localHeadPos, targetHeadPos);
    float yaw = ((float) std::sin(std::abs(eyePos.y - ang.y) * M_PI_F / 180.0f)) * dist;
    float pitch = ((float) std::sin(std::abs(eyePos.x - ang.x) * M_PI_F / 180.0f)) * dist;
    return (float) std::sqrt((yaw*yaw) + (pitch * pitch));
}

inline Vector HMath::VectorAngles(const Vector& dir)
{
    Vector angles = dir;
    float hyp = std::sqrt((dir.x * dir.x) + (dir.y * dir.y));
    angles.x = (float) std::atan(dir.z / hyp) * HMath::RADPI;
    angles.y = (float) std::atan(dir.y / dir.x) * HMath::RADPI;
    if (dir.x >= 0.0) {
        angles.y += 180.0f;
    }
    return angles;
}

inline Vector HMath::ClampAngle(const Vector& v)
{
    Vector vang = v;
    if (vang.x > 89.0f && vang.y <= 180.0f)
        vang.x = 89.0f;
    if (vang.x > 180.0f)
        vang.x = vang.x - 360.0f;
    if (vang.x < -89.0f)
        vang.x = -89.0f;
    if (vang.y > 180.0f)
        vang.y = vang.y - 360.0f;
    if (vang.y < -180.0f)
        vang.y = vang.y + 360.0f;
    vang.z = 0;

    return vang;
}
