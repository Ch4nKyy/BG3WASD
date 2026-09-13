#pragma once

struct Vector3
{
    float x, y, z;

    Vector3 operator-(const Vector3& other) const
    {
        return { x - other.x, y - other.y, z - other.z };
    }

    Vector3 operator*(float scalar) const { return { x * scalar, y * scalar, z * scalar }; }

    Vector3 operator/(float scalar) const { return { x / scalar, y / scalar, z / scalar }; }

    Vector3 operator+(const Vector3& other) const
    {
        return { x + other.x, y + other.y, z + other.z };
    }

    Vector3 operator+=(const Vector3& other)
    {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }

    float Length() const { return std::sqrt(x * x + y * y + z * z); }

    void Normalize()
    {
        const float len_sq = x * x + y * y + z * z;

        if (len_sq <= 0.0f)
            return;

        const float inv_len = 1.0f / std::sqrt(len_sq);

        x *= inv_len;
        y *= inv_len;
        z *= inv_len;
    }
};
static_assert(sizeof(Vector3) == 0xC);
