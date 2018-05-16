#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <GL/glut.h>
#include <math.h>
#include <unistd.h>
#include <time.h>
#include <GL/freeglut.h>
#include <algorithm>
#include <iostream>
#include "spline.h"

#define WATERSIZE 40
#define DAMP 15

using namespace std;

double water[2][WATERSIZE][WATERSIZE];

int spin_x, spin_y, spin_z; /* x-y rotation and zoom */
int h, w;                   /* height, width  of window */
int old_x, old_y, move_z;
double spline_order = 1;
int slow_mo = 0;
GLfloat light_pos[] = {1.0, 1.0, 1.0, 1.0};

int t = 0, f = 1;

void calculateDropEffect() {
	int x, y;
	double n;

	for(y = 1; y < WATERSIZE - 1; y++) {
		for(x = 1; x < WATERSIZE - 1; x++) {
			n = (water[t][x - 1][y] + water[t][x + 1][y] + water[t][x][y - 1] + water[t][x][y + 1]) / 2 - water[f][x][y];
      		n -= (n / DAMP);
			water[f][x][y] = n;
		}
	}

	y = 0;
	for(x = 1; x < WATERSIZE - 1; x++) {
        n = (water[t][x - 1][y] + water[t][x + 1][y] + water[t][x][y + 1]) / 2 - water[f][x][y];
        n -= (n / DAMP);
        water[f][x][y] = n;
	}

	x = 0;
	for(y = 1; y < WATERSIZE - 1; y++) {
        n = (water[t][x + 1][y] + water[t][x][y - 1] + water[t][x][y + 1]) / 2 - water[f][x][y];
        n -= (n / DAMP);
        water[f][x][y] = n;
	}

	x = WATERSIZE - 1;
	for(y = 1; y < WATERSIZE - 1; y++) {
        n = (water[t][x - 1][y] + water[t][x][y - 1] + water[t][x][y + 1]) / 2 - water[f][x][y];
        n -= (n / DAMP);
        water[f][x][y] = n;
	}

	y = WATERSIZE - 1;
	for(x = 1; x < WATERSIZE - 1; x++) {
        n = (water[t][x-1][y] + water[t][x+1][y] + water[t][x][y-1]) / 2 - water[f][x][y];
        n -= (n / DAMP);
        water[f][x][y] = n;
	}

	Sleep(slow_mo);
}

void reshape(int width, int height) {
    w = width;
    h = height;

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(40.0, (GLfloat) width/(GLfloat) height, 1.0, 2000.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity ();
}

bool stop = false;
void display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity ();
    glPushMatrix();
    glTranslatef(0, 0, spin_z - 220);
    glRotatef(spin_x, 0, 1, 0);
    glRotatef(spin_y - 240, 1, 0, 0);
    glMatrixMode(GL_PROJECTION);

    if (!stop)
        calculateDropEffect();

//---spline surface trial
//    vector<double> X[WATERSIZE], Y[WATERSIZE];
//    tk::spline s[WATERSIZE];
//
//    for(int i = 0; i < WATERSIZE; i+=1) {
//        for(int j = 0; j < WATERSIZE; j+=1) {
//            X[i].push_back(j - WATERSIZE / 2);
//            Y[i].push_back(water[f][i][j]);
////-----------working code for drawing surface for simulation
//            if(i + 1 < WATERSIZE and j + 1 < WATERSIZE) {
//                glColor3f(0.196078, 0.6, 0.8);
//                glBegin(GL_QUADS);
//                    glVertex3f(j - WATERSIZE / 2, i - WATERSIZE / 2, water[f][i][j]);
//                    glVertex3f(j - WATERSIZE / 2 + 1, i - WATERSIZE / 2, water[f][i][j + 1]);
//                    glVertex3f(j - WATERSIZE / 2 + 1, i - WATERSIZE / 2 + 1, water[f][i + 1][j + 1]);
//                    glVertex3f(j - WATERSIZE / 2, i - WATERSIZE / 2 + 1, water[f][i + 1][j]);
//                glEnd();
//            }
//            glColor3f(0, 0, 0);
//            glBegin(GL_LINE_STRIP);
//                glVertex3f(j - WATERSIZE / 2, i - WATERSIZE / 2, water[f][i][j]);
//                glVertex3f(j - WATERSIZE / 2 + 1, i - WATERSIZE / 2, water[f][i][j + 1]);
//                glVertex3f(j - WATERSIZE / 2 + 1, i - WATERSIZE / 2 + 1, water[f][i + 1][j + 1]);
//                glVertex3f(j - WATERSIZE / 2, i - WATERSIZE / 2 + 1, water[f][i + 1][j]);
//            glEnd();
//        }
//        s[i].set_points(X[i], Y[i]);
//    }
//
//
//    for(int i = 0; i < WATERSIZE - 1; i+=1) {
//        for(int j = 0; j < WATERSIZE - 1; j+=1) {
//            for(double k = X[i][j]; k < X[i][j + 1]; k += spline_order){
//                glColor3f(0.196078, 0.6, 0.8);
//                glBegin(GL_QUADS);
//                    glVertex3f(j - WATERSIZE / 2, i - WATERSIZE / 2, s[i](j - WATERSIZE / 2));
//                    glVertex3f(j - WATERSIZE / 2 + spline_order, i - WATERSIZE / 2, s[i](j - WATERSIZE / 2 + spline_order));
//                    glVertex3f(j - WATERSIZE / 2 + spline_order, i - WATERSIZE / 2 + spline_order, s[i + 1](j - WATERSIZE / 2 + spline_order));
//                    glVertex3f(j - WATERSIZE / 2, i - WATERSIZE / 2 + spline_order, s[i + 1](j - WATERSIZE / 2));
//                glEnd();
//
////                glColor3f(0, 0, 0);
////                glBegin(GL_LINE_STRIP);
////                    glVertex3f(j - WATERSIZE / 2, i - WATERSIZE / 2, s[i](j - WATERSIZE / 2));
////                    glVertex3f(j - WATERSIZE / 2 + 1, i - WATERSIZE / 2, s[i](j - WATERSIZE / 2 + 1));
////                    glVertex3f(j - WATERSIZE / 2 + 1, i - WATERSIZE / 2 + 1, s[i + 1](j - WATERSIZE / 2 + 1));
////                    glVertex3f(j - WATERSIZE / 2, i - WATERSIZE / 2 + 1, s[i + 1](j - WATERSIZE / 2));
////                glEnd();
//            }
//        }
//    }

//-------------working code for connected lines for simulation
    for(int i = 0; i < WATERSIZE; i+=1) {
        vector<double> X, Y;
        for(int j = 0; j < WATERSIZE; j+=1) {
//            glColor3f(0, 0, 1);
//            glVertex3f(j - WATERSIZE / 2, i - WATERSIZE / 2, water[f][i][j]);
            X.push_back(j - WATERSIZE / 2);
            Y.push_back(water[f][i][j]);
        }
        glBegin(GL_LINE_STRIP);
        tk::spline s;
        s.set_points(X, Y);
        for(double j = 0; j < WATERSIZE; j += spline_order){
            glColor3f(0, 0, 1);
            glVertex3f(j - WATERSIZE / 2, i - WATERSIZE / 2, s(j - WATERSIZE / 2));
        }
        glEnd();
    }
    for(int i = 0; i < WATERSIZE; i+=1) {
        vector<double> X, Y;
        for(int j = 0; j < WATERSIZE; j+=1) {
//            glColor3f(0, 0, 1);
//            glVertex3f(i - WATERSIZE / 2, j - WATERSIZE / 2, water[f][i][j]);
            X.push_back(j - WATERSIZE / 2);
            Y.push_back(water[f][i][j]);
        }
        glBegin(GL_LINE_STRIP);
        tk::spline s;
        s.set_points(X, Y);
        for(double j = 0; j < WATERSIZE; j += spline_order){
            glColor3f(0, 0, 1);
            glVertex3f(i - WATERSIZE / 2, j - WATERSIZE / 2, s(j - WATERSIZE / 2));
        }
        glEnd();
    }
    swap(f, t);
    glPopMatrix();

    glutSwapBuffers();
}

int num  = 0;
int delay = 50;
void idle()
{
//    num = (num + 1) % delay;
//    if(!num)
//    {
//        int r = rand();
//        water[f][r % WATERSIZE][r % WATERSIZE] = -rand() % 20;
////        water[f][WATERSIZE / 2 - 1][WATERSIZE / 2 - 1] = -rand() % 10;
////        delay = rand() % 100 + 100;
//    }
//    Sleep(10.0);
    glutPostRedisplay();
}

void mouse(int button, int state, int x, int y) {
    if(button == 0){
        old_x = x - spin_x;
        old_y = y - spin_y;
    }
    else if(button == 2){
        old_y = y - spin_z;
        move_z = (move_z ? 0 : 1);
    }
    glutPostRedisplay();
}

void motion(int x, int y) {
    if(!move_z) {
        spin_x = x - old_x;
        spin_y = y - old_y;
    }
    else {
        spin_z = y - old_y;
    }
    glutPostRedisplay();
}


void keyboard(unsigned char key, int x, int y) {
    static int old_x = 50;
    static int old_y = 50;
    static int old_width = 512;
    static int old_height = 512;

    if(key == 'x') {
        exit(0);
    }
    else if(key == 'm') {
        glutPositionWindow(old_x, old_y);
        glutReshapeWindow(old_width, old_height);
    }
    else if(key == 'f') {
        if (glutGet(GLUT_WINDOW_WIDTH) < glutGet(GLUT_SCREEN_WIDTH)) {
            old_x = glutGet(GLUT_WINDOW_X);
            old_y = glutGet(GLUT_WINDOW_Y);
            old_width = glutGet(GLUT_WINDOW_WIDTH);
            old_height = glutGet(GLUT_WINDOW_HEIGHT);
            glutFullScreen();
        }
    }
    else if(key == ' ') {
        water[f][WATERSIZE / 2][WATERSIZE / 2] = -20;
    }
    else if(key == 's') {
        stop = !stop;
    }
    else if(key == 'o') {
        if (spline_order > 0.02){
            spline_order -= 0.01;
        }
    }
    else if(key == 'd') {
        if (spline_order < 1.0)
            spline_order += 0.01;
    }
    else if(key == 'q') {
        slow_mo ++;
    }
    else if(key == 'w') {
        slow_mo --;
    }
}

void init() {
    w = glutGet(GLUT_WINDOW_WIDTH);
    h = glutGet(GLUT_WINDOW_HEIGHT);

//    GLfloat mat_specular[] = {0.0, 0.0, 1.0, 1.0};
//    GLfloat mat_shininess[] = {0.0};

//    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
//    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
//    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
//
//    glEnable(GL_LIGHTING);

    cout << "Press 'f' to allow full screen mode" << endl;
    cout << "Press 'm' to minimize the screen" << endl;
    cout << "Press 'q' to slow down the rendering speed" << endl;
    cout << "Press 'w' to fasten the rendering speed" << endl;
    cout << "Press 'o' to increment spline order" << endl;
    cout << "Press 'd' to decrement spline order" << endl;
    cout << "Press 's' to stop wave propagation" << endl;
    cout << "Press 'x' to close the window" << endl;

    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);

	for(int i = 0; i < WATERSIZE; i++)
		for(int j = 0; j < WATERSIZE; j++)
			water[0][j][i] = water[1][j][i] = 0;

    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glLineWidth(3);
}

int main(int argc, char** argv) {
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowPosition(50, 50);
    glutInitWindowSize(512, 512);
    glutInit(&argc, argv);

    glutCreateWindow("Simulating Water");
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);

    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);

    glEnable (GL_DEPTH_TEST);

	printf("Water Simulation \n");
    init();

    glutIdleFunc(idle);
    glutMainLoop();
    return 0;
}
