#pragma once
//#include "Tga.h"
void DrawCircle(double x1, double y1, double radius);

void DrawRectangle(double x1, double y1, double x2, double y2);

void DrawTriangle(double x1, double y1, double x2, double y2, double x3, double y3);

//gliGenericImage *readTgaImage(char *filename);

//gliGenericImage *readImage(char *filename);

//void SetBorder(gliGenericImage * image);
void setTexture(int tex);
bool PowerOf2(int h);
//double randomdouble();
enum viewtype { top_view, perspective_view, rat_view };
extern viewtype current_view;