#pragma once

struct Vector3
{
	float x = 0;
	float y = 0;
	float z = 0;
	float w = 1;
};

class VectorOperations
{

public:
	Vector3 Add(const Vector3& v1, const Vector3& v2)
	{
		return { v1.x + v2.x, v1.y + v2.y, v1.z + v2.z };
	}

	Vector3 Subtract(const Vector3& v1, const Vector3& v2)
	{
		return { v1.x - v2.x, v1.y - v2.y, v1.z - v2.z };
	}

	Vector3 Multiply(const Vector3& v1, const float k)
	{
		return { v1.x * k, v1.y * k, v1.z * k };
	}

	Vector3 Divide(const Vector3& v1, const float k)
	{
		return { v1.x / k, v1.y / k, v1.z / k };
	}

	float Dot(const Vector3& v1, const Vector3& v2)
	{
		return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
	}

	float Length(const Vector3& v)
	{
		return sqrtf(Dot(v, v));
	}

	Vector3 Normalise(const Vector3& v)
	{
		const float l = Length(v);
		return { v.x / l, v.y / l, v.z / l };
	}

	Vector3 Cross(const Vector3& v1, const Vector3& v2)
	{
		Vector3 v;
		v.x = v1.y * v2.z - v1.z * v2.y;
		v.y = v1.z * v2.x - v1.x * v2.z;
		v.z = v1.x * v2.y - v1.y * v2.x;
		return v;
	}

	Vector3 IntersectPlane(const Vector3& plane_p, const Vector3& plane_n, const Vector3& lineStart, const Vector3& lineEnd)
	{
		Vector3 newPlane_n = Normalise(plane_n);
		float plane_d = -Dot(newPlane_n, plane_p);
		float ad = Dot(lineStart, newPlane_n);
		float bd = Dot(lineEnd, newPlane_n);
		float t = (-plane_d - ad) / (bd - ad);
		Vector3 lineStartToEnd = Subtract(lineEnd, lineStart);
		Vector3 lineToIntersect = Multiply(lineStartToEnd, t);
		return Add(lineStart, lineToIntersect);
	}

	float MagnitudeSquared(const Vector3& v)
	{
		return v.x * v.x + v.y * v.y + v.z * v.z;
	}
};