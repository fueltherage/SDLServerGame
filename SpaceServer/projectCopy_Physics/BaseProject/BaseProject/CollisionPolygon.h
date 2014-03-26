#pragma once
#include <vector>
#include "Vector2D.h"
class CollisionPolygon
{
public:

	CollisionPolygon(void);
	~CollisionPolygon(void);
	CollisionPolygon(std::vector<Vector2D> _points);
	void AddLocalPoint(Vector2D newPoint);
	int NumberOfVertices(){return points.size();};
	Vector2D position;
	Vector2D scale;
	float rotation;
	void Draw();

	std::vector<Vector2D> WorldPoisitionOfPoints()
	{
		std::vector<Vector2D> WorldPoints;
		for each (Vector2D point in points)
		{
			WorldPoints.push_back(Vector2D((cos(rotation)*point.x*scale.x - sin(rotation)*point.y*scale.y ) +position.x,
										   (sin(rotation)*point.x*scale.x + cos(rotation)*point.y*scale.y ) +position.y));
		}
		return WorldPoints;
	};
private:
	std::vector<Vector2D> points;

};

