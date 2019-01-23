#include "maze.h"
#include <stdlib.h>
#include "glut.h"
#include "graphics1.h"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <ostream>
#include <random>
#include <chrono>
//#include "tga.h"

Cell::Cell()
{
	l = t = r = b = true;
	red = getrandomdouble();
	green = getrandomdouble();
	blue = getrandomdouble();
}

void Cell::Draw(int i, int j)
{
	if (b == true)
	{
		glBegin(GL_LINES);
		glVertex2i(i, j);
		glVertex2i(i + 1, j); // bottom , left
		glEnd();
	}//repeat for other sides
	if (l == true)
	{
		glBegin(GL_LINES);
		glVertex2i(i, j);
		glVertex2i(i, j+1);
		glEnd();
	}//repeat for other sides
	if (r == true)
	{
		glBegin(GL_LINES);
		glVertex2i(i+1, j);
		glVertex2i(i+1, j+1);
		glEnd();
	}//repeat for other sides
	if (t == true)
	{
		glBegin(GL_LINES);
		glVertex2i(i , j + 1);
		glVertex2i(i + 1, j+1);
		glEnd();
	}//repeat for other sides
}

void Cell::DrawQ(int i, int j) {
	if (b == true) //1
	{
		setTexture(int(1));
		glBegin(GL_QUADS);
		glTexCoord2f(0,0); glVertex3i(i, j, 0);
		glTexCoord2f(0, 1); glVertex3i(i , j, 1);
		glTexCoord2f(1, 1); glVertex3i(i+1, j, 1);
		glTexCoord2f(1, 0); glVertex3i(i+1, j, 0);
		glEnd();
	}//repeat for other sides
	if (l == true) //2
	{
		setTexture(int(2));
		glBegin(GL_QUADS);
		glTexCoord2f(0, 0); glVertex3i(i, j+1, 0);
		glTexCoord2f(0, 3); glVertex3i(i, j+1, 1);
		glTexCoord2f(3, 3); glVertex3i(i, j, 1);
		glTexCoord2f(3, 0); glVertex3i(i, j, 0);
		glEnd();
	}//repeat for other sides
	if (r == true) //3
	{
		setTexture(int(3));
		glBegin(GL_QUADS);
		glTexCoord2f(-1, -1); glVertex3i(i+1, j+1, 0);
		glTexCoord2f(-1, 2); glVertex3i(i+1, j+1, 1);
		glTexCoord2f(2, 2); glVertex3i(i+1, j, 1);
		glTexCoord2f(2, -1); glVertex3i(i+1, j, 0);
		glEnd();
		//(-1, -1), (2,-1) (2,2) (-1,2)
	}//repeat for other sides
	if (t == true) //4
	{
		setTexture(int(4));
		glBegin(GL_QUADS);
		glTexCoord2f(0, 0); glVertex3i(i, j+1, 0);
		glTexCoord2f(0, 1); glVertex3i(i, j+1, 1);
		glTexCoord2f(1, 1); glVertex3i(i+1, j+1, 1);
		glTexCoord2f(1, 0); glVertex3i(i+1, j+1, 0);
		glEnd();
	}//repeat for other sides
}

void Cell::setV(bool v)
{
	visited = v;
}

bool Cell::getV()
{
	return visited;
}

void Cell::setL(bool bo)
{
	l = bo;
}

void Cell::setT(bool bo)
{
	t = bo;
}

void Cell::setR(bool bo)
{
	r = bo;
}

void Cell::setB(bool bo)
{
	b = bo;

}

bool Cell::getT() {
	return t;
}
bool Cell::getB() {
	return b;
}
bool Cell::getL() {
	return l;
}
bool Cell::getR() {
	return r;
}
//////////////////////////////////
Maze::Maze()
{
	//do nothing
}


void Maze::Draw()
{
	if (current_view == top_view)
	{
		// draw walls as GL_LINES
		for (int i = 0; i < M; i++)
		{
			for (int j = 0; j < N; j++)
			{
				glColor3d(cells[i][j].getred(), cells[i][j].getgreen(), cells[i][j].getblue());
				cells[i][j].Draw(i, j);
			}

		}
	}
	else
	{
		for (int i = 0; i < M; i++)
		{
			for (int j = 0; j < N; j++)
			{
				glColor3d(cells[i][j].getred(), cells[i][j].getgreen(), cells[i][j].getblue());
				cells[i][j].DrawQ(i, j);
			}

		}
		// draw walls as GL_QUADS
		// figure out a way to draw each wall in a different color. (you don't have to save the color of the wall)
		// figure out a way to prevent two co-planar wall from 'bleeding' on top of each other when drawing.
	}

}

void Maze::RemoveWalls()
{
	// randomly remove a bottom wall from one of the bottom cells.
	//then randomly remove a top wall from top cells.
	//wall recursive takes an i,j start at cell 0,0

	//removewallsrecursive(i,j) (0,0) 
	//add a visited flag. boolian type
	//mark cell i,j visted
		//while there is at least 1 unvisted neighbor
			//randomly pic 1 unvisted neighbor cell
			//knock out a wall
			//recall function.
	//kill a random top wall
	int x = randomint(0, M-1);
	int y = N-1;
	cells[x][y].setT(false);
	endI = x;
	endJ = y;
	//kill a random bottom wall
	x = randomint(0, M-1);
	y = 0;
	cells[x][y].setB(false);
	startI = x;
	startJ = y;
	///
	int i = 0;
	int j = 0;

	destroywalls(i, j);
}

int Maze::randomint(int low, int high)
{
	unsigned int seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::default_random_engine generator(seed);
	std::uniform_int_distribution<int> distribution(low, high);

	int bot = distribution(generator);

	return bot;
}
void Maze::destroywalls(int i, int j)
{
	std::vector<char> neighbors;
	cells[i][j].setV(true);
	while (true)
	{
		neighbors.clear();

		if (i > 0 && !(cells[i - 1][j].getV()))
		{
			neighbors.push_back('w');
		}
		if (j > 0 && !(cells[i][j - 1].getV()))
		{
			neighbors.push_back('n');

		}
		if (i < M-1 && !(cells[i+1][j].getV()))
		{
			neighbors.push_back('e'); //stuck in this if loop for some reason
		}
		if (j < N - 1 && !(cells[i][j + 1].getV()))
		{
			neighbors.push_back('s');
		}

		if (neighbors.size() == 0)
		{
			break;
		}
		else
		{
			int high = neighbors.size() - 1;
			int r = randomint(0,high);
			if (neighbors[r] == 's')
			{
				cells[i][j].setT(false);
				cells[i][j + 1].setB(false);
				destroywalls(i, j + 1);

			}
			if (neighbors[r] == 'n')
			{
				cells[i][j-1].setT(false);
				cells[i][j].setB(false);
				destroywalls(i, j - 1);

			}
			if (neighbors[r] == 'e')
			{
				cells[i][j].setR(false);
				cells[i+1][j].setL(false);
				destroywalls(i+1, j);

			}
			if (neighbors[r] == 'w')
			{
				cells[i-1][j].setR(false);
				cells[i][j].setL(false);
				destroywalls(i-1, j);

			}
		}
	}
}

int Maze::getStartI() {
	return startI;
}

int Maze::getStartJ() {
	return startJ;
}

int Maze::getEndI() {
	return endI;
}

int Maze::getEndJ() {
	return endJ;
}

bool Maze::issafe(double x, double y, double radius) {
	int i = (int)x;
	int j = (int)y;
	double cx = x - i;
	double cy = y - j;
	if (cells[i][j].getL() && cx - radius < 0){
		return false;
	}
	if (cells[i][j].getR() && cx + radius > 1) {
		return false;
	}
	if (cells[i][j].getT() && cy + radius > 1) {
		return false;
	}
	if (cells[i][j].getB() && cy - radius < 0) {
		return false;
	}
	//this checks corners
	if (cx + radius > 1 && cy - radius < 0) { //bottom right
		return false;
	}
	if (cx + radius > 1 && cy + radius > 1) { //top right
		return false;
	}
	if (cx - radius < 0 && cy - radius < 0) { //bottom left
		return false;
	}
	if (cx - radius < 0 && cy + radius > 1) {// top left
		return false;
	}
	return true;
}
//find what cell they are in and what walls are up
//{ int i = (int)x;
// int j = (int)y;
//double cx = x - i;
//double cy = y -j;
//if mCell[i][j].left && cx - radius < 0
//return flase
//if mCell[i][j].right && cx + radius > 1
//return flase
//repeat for top and bottom
//check bottom right corner
// if cx + radius > 1 and cy - radius < 0
//return false
//repeat for each corner
//return true
//}

double Cell::getrandomdouble() {
	double randnum = ((double)rand() / (RAND_MAX));
	return randnum;
}

double Cell::getred() {
	return red;
}

double Cell::getgreen() {
	return green;
}

double Cell::getblue() {
	return blue;
}