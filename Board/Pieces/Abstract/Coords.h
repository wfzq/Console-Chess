#pragma once
struct Coords
{
	int startX = 0, startY = 0, endX = 0, endY = 0;
	char promotion = 'Q'; // Default promotion is Queen

	static Coords copyCoords(const Coords& original, Coords& copy)
	{
		copy.startX = original.startX;
		copy.startY = original.startY;
		copy.endX = original.endX;
		copy.endY = original.endY;

		return copy;
	}
	static bool cmpCoords(Coords a, Coords b)
	{
		return a.startX == b.startX &&
			a.startY == b.startY &&
			a.endX == b.endX &&
			a.endY == b.endY;
	}
};