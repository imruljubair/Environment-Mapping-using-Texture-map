/*
Jubair, Mohammad Imrul
10159229
HW - 2
*/

#include <iostream>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <math.h>
#include "imageloader.h"

#define PI 3.14159265359
#define ANG 1.5;

using namespace std;

// initial angle of the camera
double _angleX = 0; 
double _angleY = -45;

GLuint _textureId; // ID of the texture 

void DrawSphere(); // function where the sphere is discretized and texture is mapped


void Draw() {
	
	glClear(GL_COLOR_BUFFER_BIT);

	glClearColor(0.0, 0.0, 0.0, 0.0);
	
	glColor3f(1.f, 1.f, 1.f);
	
	DrawSphere();

	glutSwapBuffers();
	glFlush(); 

}

void DrawSphere()
{
	// initializations.....

	double R = 8.5; // radius
	double x,y,z; // x,y,z coordinates from theta, phi, R coordinate
	int a=0, b=0; // index of the array

	double angleInc = PI/61; // steps through theta and phi
	
	// initializing an array to store the atlas of the coordinates
	double **coordX = 0, **coordY = 0, **coordZ = 0;
	coordX = new double *[200] ;
	coordY = new double *[200] ;
	coordZ = new double *[200] ;

	for( int p = 0 ; p < 200 ; p++ ){
	coordX[p] = new double[200];
	coordY[p] = new double[200];
	coordZ[p] = new double[200];
	}


	// discretization starts...
	for (double theta = PI; theta > 0; theta = theta - angleInc)
	{  b = 0;	
		
		for(double phi = 2*PI; phi > 0; phi = phi - angleInc)
		{
			// conversion (theta, phi, R) --> (x,y,z)
			x = R*sin(theta)*cos(phi);
			z = R*sin(theta)*sin(phi);
			y = R*cos(theta);
				
			// store into the array
			coordX[b][a] = x;
			coordY[b][a] = y;
			coordZ[b][a] = z;
			
			b = b + 1;
		}
		a = a + 1;
	}

	
	//texture stuffs....

	 glEnable(GL_TEXTURE_2D);
	 glBindTexture(GL_TEXTURE_2D, _textureId);
	 glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );

	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	
	 glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	 glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	 glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
     glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

	// initialization for u,v coordinate
	double u=0,v=0, ulength, vlength;

	// calculating amount of steps through u and v
	ulength = 1/(double)b;
	vlength = 1/(double)a;
	
	glPushMatrix();

		glRotatef(_angleX, 1, 0 , 0); // rotate w.r.t X axis when key is pressed
			
		glPushMatrix();
			glRotatef(_angleY, 0, 1, 0); // rotate w.r.t Y axis when key is pressed 

	
	glBegin(GL_QUADS); // start quads....
	
	for (int i=0 ; i<b; i=i+1)
	{
		v = 0;
		for (int j=0; j<a; j=j+1)
		{
			// mapping ....

			glTexCoord2f(u,v);
			glVertex3f(coordX[i][j], coordY[i][j], coordZ[i][j]);
			
			glTexCoord2f(u, v+vlength);
			glVertex3f(coordX[i][j+1], coordY[i][j+1], coordZ[i][j+1]);
			
			glTexCoord2f(u+ulength, v+vlength);
			glVertex3f(coordX[i+1][j+1], coordY[i+1][j+1], coordZ[i+1][j+1]);
			
			glTexCoord2f(u+ulength, v); 
			glVertex3f(coordX[i+1][j], coordY[i+1][j], coordZ[i+1][j]);
			
			v = v + vlength; 
			
		}
		u = u + ulength;
	}

	glEnd();

	glPopMatrix();
	glPopMatrix();

	
	delete [] *coordX;
	delete [] *coordY;
	delete [] *coordZ;
}

GLuint loadTexture(Image* image) {
	GLuint textureId;
	glGenTextures(1, &textureId); //Make room for our texture
	glBindTexture(GL_TEXTURE_2D, textureId); //Tell OpenGL which texture to edit
	//Map the image to the texture
	glTexImage2D(GL_TEXTURE_2D,                //Always GL_TEXTURE_2D
				 0,                            //0 for now
				 GL_RGB,                       //Format OpenGL uses for image
				 image->width, image->height,  //Width and height
				 0,                            //The border of the image
				 GL_RGB, //GL_RGB, because pixels are stored in RGB format
				 GL_UNSIGNED_BYTE, //GL_UNSIGNED_BYTE, because pixels are stored
				                   //as unsigned numbers
				 image->pixels);               //The actual pixel data
	return textureId; //Returns the id of the texture
}


void Initialize(int w, int h) {
	
	glViewport(0,0,w,h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (double)w/(double)h, 1.0, 100.0);
	Image* image = loadBMP("./pbrt-8.bmp"); // directory of the image
	
	_textureId = loadTexture(image);
	
	delete image;
	
		
}

// key press handling function
void handleSpecialKeypress(int key, int x, int y)
{
	switch (key) {
		// for DOWN key
		case GLUT_KEY_DOWN:
					
			_angleX = _angleX + ANG;
			if(_angleX >=360)
			{
				_angleX = _angleX - 360; // limiting the angle between 0 to 360
			}
						
			break;
	
		// for UP key
		case GLUT_KEY_UP:
	
			_angleX = _angleX - ANG;
			if(_angleX <=-360)
			{
				_angleX = _angleX + 360; // limiting the angle between 0 to 360
			}
			
			break;

		// for RIGHT key
		case  GLUT_KEY_RIGHT:
				
				_angleY = _angleY + ANG;
				if(_angleY >=360)
				{
					_angleY = _angleY - 360; // limiting the angle between 0 to 360
				}
			
			break;

		// for LEFT key
		case  GLUT_KEY_LEFT:
				
				_angleY = _angleY - ANG;
				if(_angleY <=-360)
				{
					_angleY = _angleY + 360; // limiting the angle between 0 to 360
				}
			
			break;
	}
	glutPostRedisplay();
}

int main(int iArgc, char** cppArgv) {
	glutInit(&iArgc, cppArgv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB );
	glutInitWindowSize(800, 550);
	glutInitWindowPosition(200, 50);
	glutCreateWindow("HW-2");
	glutDisplayFunc(Draw);
	glutReshapeFunc(Initialize);
	glutSpecialFunc(handleSpecialKeypress);
	glutMainLoop();
	return 0;
}
