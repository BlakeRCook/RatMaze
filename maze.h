#pragma once

const int M = 7; //x
const int N = 6; //y

class Cell
{
public:
	Cell();
	void Draw(int i, int j);
	void DrawQ(int i, int j);
	void setV(bool v);
	bool getV();
	void setL(bool bo);
	void setT(bool bo);
	void setR(bool bo);
	void setB(bool bo);
	bool getL();
	bool getT();
	bool getB();
	bool getR();
	double getred();
	double getgreen();
	double getblue();
	double getrandomdouble();
private:
	bool l, t, r, b; //left top right bottom are they up?
	bool visited = false;
	double red;
	double green;
	double blue;
};

class Maze
{
public:
	Maze();
	void Draw();
	void RemoveWalls();
	void destroywalls(int i, int j);
	int randomint(int low, int high);
	int getStartI();
	int getStartJ();
	int getEndI();
	int getEndJ();
	bool issafe(double x, double y, double radius);
private:
	Cell cells[M][N];
	int startI;
	int startJ;
	int endI;
	int endJ;
};