#pragma once

struct Vector3
{
	float x, y, z;

	float Distance(Vector3& Destination)
	{
		float xDelta = Destination.x - x;
		float yDelta = Destination.y - y;
		float zDelta = Destination.z - z;

		xDelta = xDelta * xDelta;
		yDelta = yDelta * yDelta;
		zDelta = zDelta * zDelta;

		float Intermediate = xDelta + yDelta + zDelta;

		return sqrt(Intermediate);
	};
};