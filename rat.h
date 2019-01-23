#pragma once
#include <stdlib.h>
#include "glut.h"
#include "maze.h"
const double spinspeed = .1;
const double movespeed = .0009; //.0005
class Rat
{
public:
	Rat();
	void Draw();
	void Init(Maze * pmaze);
	void SpinLeft();
	void SpinRight();
	void moveforward();
	double GetX();
	double GetY();
	double GetDX();
	double GetDY();
	//bool isSafe();

private:
	double mX, mY, mDegrees; //mdegrees are between 0-360
	double mDX;
	double mDY;
	double mHitRadius; //make sures the rat is drawn within this distance of the orgin
	Maze * m_pMaze;
};