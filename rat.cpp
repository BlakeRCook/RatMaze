#include "rat.h"
#include "graphics1.h"
#include <stdlib.h>
#include "glut.h"
#include <cmath>
//viewtype current_view = top_view;
Rat::Rat()
{

}

void Rat::Draw()
{
	if (current_view == rat_view)
	{
		return; // drawing yourself in rat view looks bad.
	}

	glPushMatrix(); //save copy of transformation matrix
	glTranslated(mX, mY, 0); //permanant fixastion anytime you draw.
	glRotated(mDegrees, 0, 0, 1);
	glColor3d(0.5, 0.5, 0.5);
	DrawCircle(0, 0, .2);
	DrawCircle(.22, 0, .1);
	glPopMatrix();
}

void Rat::Init(Maze * pmaze)
{
	mX = pmaze->getStartI()+.5;
	mY = pmaze->getStartJ()+.5;
	mDegrees = 90;
	m_pMaze = pmaze;
	mHitRadius = .25;
}

void Rat::SpinLeft()
{
	mDegrees += spinspeed;
}

void Rat::SpinRight()
{
	mDegrees -= spinspeed;
}

void Rat::moveforward()
{
	double radians = mDegrees / 180 * 3.141592654;
	double dx = cos(radians);
	double dy = sin(radians);
	//mDX = dx;
	//mDY = dy; // these might not be here

	double newx = mX + dx*movespeed;
	double newy = mY + dy*movespeed;

	if (m_pMaze -> issafe(newx,newy,mHitRadius)) //method of maze
	{
		mX = newx;
		mY = newy;
	}
	else if(m_pMaze->issafe(newx, mY, mHitRadius)) //method of maze
	{
		mX = newx;
	}
	else if (m_pMaze->issafe(mX, newy, mHitRadius)) //method of maze
	{
		mY = newy;
	}

	//mX += dx*movespeed; //ask if its ok to move first
	//mY += dy*movespeed;
}

double Rat::GetX() {
	return mX;
}

double Rat::GetY() {
	return mY;
}

double Rat::GetDX() {
	double radians = mDegrees / 180 * 3.141592654;
	double dx = cos(radians);
	mDX = dx;
	return mDX;
}

double Rat::GetDY() {
	double radians = mDegrees / 180 * 3.141592654;
	double dy = sin(radians);
	mDY = dy; // these might not be here
	return mDY;
}