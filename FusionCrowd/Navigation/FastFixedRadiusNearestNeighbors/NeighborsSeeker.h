#pragma once

#include "GpuCalculator.h"


class Point;

class NeighborsSeeker
{
private:
	struct GridCell {
		int pointsCount = 0;
		int startIndex = 0;
	};
	struct PointNeighbors;

	Point* points = nullptr;
	GridCell* cells = nullptr;
	int* pointsCells = nullptr;
	int* pointsInCells = nullptr;
	int* sortedPointsId = nullptr;
	PointNeighbors* pointNeighbors = nullptr;

	int numberOfPoints;
	int numberOfCells;
	static const int NUMBER_OF_NEIGHBORS = 100;
	float worldSize;
	float searchRadius;
	int maxNearCells;
	float cellSize;
	int cellsInRow;

	GpuCalculator _calculator;
	InputBufferDesc _bufferDescriptions[3];
	bool _isCalculatorReady = false;

	void FillCellDictioanary();
	void CountIndeces();
	void SortPoints();

	int GetCellOffset(float coordinateOffset);
	int GetCellIndex(int cellX, float cellY);
	int GetCellIndex(float xCoord, float yCoord);
	bool IsInsideCell(float pointX, float pointY, int cellX, int cellY);
	bool IsCircleCrossesCell(float circleX, float circleY, int cellX, int cellY);

	void AllocateMemory();
	void FreeMemory();
	void ClearOldData(bool useGpu);
	void FindNeighborsCpu();
	void FindNeighborsGpu();
	void PrepareGpuCalculator();

	struct CpuConstants {
		float cellSize;
		int cellsInRow;
		float searchRadius;
	};

public:
	NeighborsSeeker();
	~NeighborsSeeker();

	static float gridCellCoeff;

	struct PointNeighbors {
		int pointID;
		int neighborsCount = 0;
		int neighborsID[NUMBER_OF_NEIGHBORS];
	};

	void Init(Point* points, int numberOfPoints, float worldSize, float searchRadius);
	PointNeighbors* FindNeighbors(bool useGpu = true);
};


class Point {
public:
	float x;
	float y;
	bool InRange(Point otherPoint, float range);
};

