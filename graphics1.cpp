// OpenGL/GLUT starter kit for Windows 7 and Visual Studio 2010
// Created spring, 2011
//
// This is a starting point for OpenGl applications.
// Add code to the "display" function below, or otherwise
// modify this file to get your desired results.
//
// For the first assignment, add this file to an empty Windows Console project
//		and then compile and run it as is.
// NOTE: You should also have glut.h,
// glut32.dll, and glut32.lib in the directory of your project.
// OR, see GlutDirectories.txt for a better place to put them.

#include <cmath>
#include <cstring>
#include "glut.h"
#include "maze.h"
#include "rat.h"
#include "graphics1.h"
#include <ctype.h>
#include <conio.h>
#include <iostream>
///// texture
using namespace std;
const int num_textures = 4; //just add to a header file
static GLuint texName[num_textures]; //can change static to extern

// Global Variables (Only what you need!)
double screen_x = 700;
double screen_y = 500;

Maze gMaze;
Rat gRat;
viewtype current_view = top_view;

bool gSpinLeft = false;
bool gSpinRight = false;
bool gMoveForward = false;

//tga image loader
#include "tga.h"
gliGenericImage *readTgaImage(char *filename)
{
	FILE *file;
	gliGenericImage *image;

	file = fopen(filename, "rb");
	if (file == NULL) {
		printf("Error: could not open \"%s\"\n", filename);
		return NULL;
	}
	image = gliReadTGA(file, filename);
	fclose(file);
	if (image == NULL) {
		printf("Error: could not decode file format of \"%s\"\n", filename);
		return NULL;
	}
	return image;
}

// Generic image loader code.
gliGenericImage *readImage(char *filename)
{
	size_t size = strlen(filename);
	if (toupper(filename[size - 3]) == 'T' && toupper(filename[size - 2]) == 'G' && toupper(filename[size - 1]) == 'A')
	{
		gliGenericImage * result = readTgaImage(filename);
		if (!result)
		{
			cerr << "Error opening " << filename << endl;
			_getch();
			exit(1);
		}
		return result;
	}
	else
	{
		cerr << "Unknown Filetype!\n";
		_getch();
		exit(1);
	}
}

// This resets the edges of the texture image to a given "border color".
// You must call this for clamped images that do not take up the whole polygon.
// Otherwise, the texture edges will smear outward across the rest
// of the polygon.
void SetBorder(gliGenericImage * image)
{
	// set a border color.
	unsigned int border_r = 50;
	unsigned int border_g = 50;
	unsigned int border_b = 255;
	int x, y;
	y = 0;
	for (x = 0; x<image->width; x++)
	{
		image->pixels[3 * (y*image->width + x) + 0] = border_r;
		image->pixels[3 * (y*image->width + x) + 1] = border_g;
		image->pixels[3 * (y*image->width + x) + 2] = border_b;
	}
	y = 1;
	for (x = 0; x<image->width; x++)
	{
		image->pixels[3 * (y*image->width + x) + 0] = border_r;
		image->pixels[3 * (y*image->width + x) + 1] = border_g;
		image->pixels[3 * (y*image->width + x) + 2] = border_b;
	}
	y = image->height - 1;
	for (x = 0; x<image->width; x++)
	{
		image->pixels[3 * (y*image->width + x) + 0] = border_r;
		image->pixels[3 * (y*image->width + x) + 1] = border_g;
		image->pixels[3 * (y*image->width + x) + 2] = border_b;
	}
	y = image->height - 2;
	for (x = 0; x<image->width; x++)
	{
		image->pixels[3 * (y*image->width + x) + 0] = border_r;
		image->pixels[3 * (y*image->width + x) + 1] = border_g;
		image->pixels[3 * (y*image->width + x) + 2] = border_b;
	}

	x = 0;
	for (y = 0; y<image->height; y++)
	{
		image->pixels[3 * (y*image->width + x) + 0] = border_r;
		image->pixels[3 * (y*image->width + x) + 1] = border_g;
		image->pixels[3 * (y*image->width + x) + 2] = border_b;
	}
	x = 1;
	for (y = 0; y<image->height; y++)
	{
		image->pixels[3 * (y*image->width + x) + 0] = border_r;
		image->pixels[3 * (y*image->width + x) + 1] = border_g;
		image->pixels[3 * (y*image->width + x) + 2] = border_b;
	}
	x = image->width - 1;
	for (y = 0; y<image->height; y++)
	{
		int index = 3 * (y*image->width + x);
		image->pixels[index + 0] = border_r;
		image->pixels[index + 1] = border_g;
		image->pixels[index + 2] = border_b;
	}
	x = image->width - 2;
	for (y = 0; y<image->height; y++)
	{
		int index = 3 * (y*image->width + x);
		image->pixels[index + 0] = border_r;
		image->pixels[index + 1] = border_g;
		image->pixels[index + 2] = border_b;
	}
}

// 
// Functions that draw basic primitives
//
void DrawCircle(double x1, double y1, double radius)
{
	glBegin(GL_POLYGON);
	for(int i=0; i<32; i++)
	{
		double theta = (double)i/32.0 * 2.0 * 3.1415926;
		double x = x1 + radius * cos(theta);
		double y = y1 + radius * sin(theta);
		glVertex2d(x, y);
	}
	glEnd();
}

void DrawRectangle(double x1, double y1, double x2, double y2)
{
	glBegin(GL_QUADS);
	glVertex2d(x1,y1);
	glVertex2d(x2,y1);
	glVertex2d(x2,y2);
	glVertex2d(x1,y2);
	glEnd();
}

void DrawTriangle(double x1, double y1, double x2, double y2, double x3, double y3)
{
	glBegin(GL_TRIANGLES);
	glVertex2d(x1,y1);
	glVertex2d(x2,y2);
	glVertex2d(x3,y3);
	glEnd();
}

// Outputs a string of text at the specified location.
void DrawText(double x, double y, char *string)
{
	void *font = GLUT_BITMAP_9_BY_15;

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
	
	int len, i;
	glRasterPos2d(x, y);
	len = (int) strlen(string);
	for (i = 0; i < len; i++) 
	{
		glutBitmapCharacter(font, string[i]);
	}

    glDisable(GL_BLEND);
}


//
// GLUT callback functions
//

// This callback function gets called by the Glut
// system whenever it decides things need to be redrawn.
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (current_view == perspective_view)
	{
		glEnable(GL_DEPTH_TEST);
		glLoadIdentity();
		gluLookAt(-3, -3, 7, 3, 3, 0, 0, 0, 1);
		gMaze.Draw();
		gRat.Draw();
	}
	else if (current_view == top_view)
	{
		glDisable(GL_DEPTH_TEST);
		glLoadIdentity();
		gMaze.Draw();
		gRat.Draw();
	}
	else // current_view == rat_view
	{
		glEnable(GL_DEPTH_TEST);

		glLoadIdentity();

		double z_level = .25;
		double x = gRat.GetX();
		double y = gRat.GetY();
		double dx = gRat.GetDX();
		double dy = gRat.GetDY();
		double at_x = x + dx;
		double at_y = y + dy;
		double at_z = z_level;
		gluLookAt(x, y, z_level, at_x, at_y, at_z, 0, 0, 1);

		glEnable(GL_TEXTURE_2D);
		//glBindTexture(GL_TEXTURE_2D, texName[2]);
		gMaze.Draw();

		glDisable(GL_TEXTURE_2D);
	}
	//glClear(GL_COLOR_BUFFER_BIT);
	//glColor3d(gMaze.Cell.getred(),gMaze.getgreen(),gMaze.getblue());
	if (gSpinLeft)
	{
		gRat.SpinLeft();
	}
	if (gSpinRight)
	{
		gRat.SpinRight();
	}
	if (gMoveForward)
	{
		gRat.moveforward();
	}
	//gMaze.Draw();
	//gRat.Draw();

	glutPostRedisplay();
	glutSwapBuffers();
}

void setTexture(int tex) {
	if (tex == 1) { //b
		glBindTexture(GL_TEXTURE_2D, texName[2]);
	}
	else if (tex == 2) { //l
		glBindTexture(GL_TEXTURE_2D, texName[1]);
	}
	else if (tex == 3) { //r
		glBindTexture(GL_TEXTURE_2D, texName[0]);
	}
	else { //t
		glBindTexture(GL_TEXTURE_2D, texName[2]);
	}
}
void SetTopView(int w, int h)
{
	// go into 2D mode
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	double world_margin_x = 0.5;
	double world_margin_y = 0.5;
	gluOrtho2D(-world_margin_x, M + world_margin_x,
		-world_margin_y, N + world_margin_y);
	glMatrixMode(GL_MODELVIEW);
}

void SetPerspectiveView(int w, int h)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	double aspectRatio = (GLdouble)w / (GLdouble)h;
	gluPerspective(
		/* field of view in degree */ 38.0,
		/* aspect ratio */ aspectRatio,
		/* Z near */ .1, /* Z far */ 30.0);
	glMatrixMode(GL_MODELVIEW);
}


// This callback function gets called by the Glut
// system whenever a key is pressed.
void keyboard(unsigned char c, int x, int y)
{
	switch (c) 
	{
		case 27: // escape character means to quit the program
			exit(0);
			break;
		case 'r':
			current_view = rat_view;
			SetPerspectiveView(screen_x, screen_y);
			break;
		case 'p':
			current_view = perspective_view;
			SetPerspectiveView(screen_x, screen_y);
			break;
		case 't':
			current_view = top_view;
			SetTopView(screen_x, screen_y);
			break;
		default:
			return; // if we don't care, return without glutPostRedisplay()
	}

	glutPostRedisplay();
}


// This callback function gets called by the Glut
// system whenever the window is resized by the user.
void reshape(int w, int h)
{
	screen_x = w;
	screen_y = h;
	glViewport(0, 0, w, h);

	if (current_view == top_view)
	{
		SetTopView(w, h);
	}
	else if (current_view == perspective_view)
	{
		SetPerspectiveView(w, h);
	}
	else // current_view == rat_view
	{
		SetPerspectiveView(w, h);
	}
	//// Reset our global variables to the new width and height.
	//screen_x = w;
	//screen_y = h;

	//// Set the pixel resolution of the final picture (Screen coordinates).
	//glViewport(0, 0, w, h);

	//// Set the projection mode to 2D orthographic, and set the world coordinates:
	//glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();
	//gluOrtho2D(-0.5, M+0.5, -0.5, N+0.5);
	//glMatrixMode(GL_MODELVIEW);

}

// This callback function gets called by the Glut
// system whenever any mouse button goes up or down.
void mouse(int mouse_button, int state, int x, int y)
{
	if (mouse_button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) 
	{
		gSpinLeft = true;
	}
	if (mouse_button == GLUT_LEFT_BUTTON && state == GLUT_UP) 
	{
		gSpinLeft = false;
	}
	if (mouse_button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		gSpinRight = true;
	}
	if (mouse_button == GLUT_RIGHT_BUTTON && state == GLUT_UP)
	{
		gSpinRight = false;
	}
	if (mouse_button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN) 
	{
		gMoveForward = true;
	}
	if (mouse_button == GLUT_MIDDLE_BUTTON && state == GLUT_UP) 
	{
		gMoveForward = false;
	}
	glutPostRedisplay();
}

// Return true if h is a perfect power of 2 (up to 4096)
bool PowerOf2(int h)
{
	if (h != 2 && h != 4 && h != 8 && h != 16 && h != 32 && h != 64 && h != 128 &&
		h != 256 && h != 512 && h != 1024 && h != 2048 && h != 4096)
		return false;
	else
		return true;
}


// Your initialization code goes here.
void InitializeMyStuff()
{
	gMaze.RemoveWalls();
	gRat.Init(&gMaze);
	////////////////
	////////////////
	gliGenericImage *image[num_textures];
	int n = 0;
	image[n++] = readImage("stone.tga");
	image[n++] = readImage("brick.tga");
	image[n++] = readImage("graffiti.tga");
	image[n++] = readImage("Seattle.tga");
	if (n != num_textures)
	{
		printf("Error: Wrong number of textures\n");
		_getch();
		exit(1);;
	}

	glGenTextures(num_textures, texName);

	for (int i = 0; i<num_textures; i++)
	{
		glBindTexture(GL_TEXTURE_2D, texName[i]);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
		int repeats = false;
		if (i == 1) {
			repeats = true;
		}

		int needs_border = false;// Needed if clamping and not filling the whole polygon. !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! need these for hw
		if (i == 0) {
			needs_border = true;
		}
		if (repeats)
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		}
		else
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		}
		if (needs_border)
		{
			// set a border.
			SetBorder(image[i]);
		}

		bool mipmaps = false;
		if (!PowerOf2(image[i]->height) || !PowerOf2(image[i]->width))
		{
			// WARNING: Images that do not have width and height as 
			// powers of 2 MUST use mipmaps.
			mipmaps = true;
		}

		if (mipmaps)
		{
			gluBuild2DMipmaps(GL_TEXTURE_2D, image[i]->components,
				image[i]->width, image[i]->height,
				image[i]->format, GL_UNSIGNED_BYTE, image[i]->pixels);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
				//GL_LINEAR_MIPMAP_LINEAR);
				GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
				//GL_LINEAR);
				GL_NEAREST);
		}
		else
		{
			glTexImage2D(GL_TEXTURE_2D, 0, image[i]->components,
				image[i]->width, image[i]->height, 0,
				image[i]->format, GL_UNSIGNED_BYTE, image[i]->pixels);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}
	}
}


int main(int argc, char **argv)
{
	//srand(
	glutInit(&argc, argv);

	//glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(screen_x, screen_y);
	glutInitWindowPosition(50, 50);

	int fullscreen = 0;
	if (fullscreen) 
	{
		glutGameModeString("800x600:32");
		glutEnterGameMode();
	} 
	else 
	{
		glutCreateWindow("This appears in the title bar");
	}

	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);

	glColor3d(0,0,0); // forground color
	glClearColor(1, 1, 1, 0); // background color
	InitializeMyStuff();

	glutMainLoop();

	return 0;
}