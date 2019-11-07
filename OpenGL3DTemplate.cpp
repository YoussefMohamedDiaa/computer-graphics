#include <stdlib.h> 
#include <cmath>
#include <string.h>
#include <glut.h>
#include <math.h>
#include <ctime>
#include <cstdio>
#include <windows.h>
#include <mmsystem.h>

double move = 0.5;
int inc = 1;
int rounds = 1;

double horizontalMove = 0;
double verticalMove = 0;
double horizontalMoveWeapon = 0;
double verticalMoveWeapon = 0;
bool shoot = false;

bool going = true;
double forwadBullet = 0;
double backwadBullet = 0;

double currentBulletPositionX = -1;
double currentBulletPositionY = -1;
double currentBulletPositionZ = -1;

double intialX = 4;
double intialY = 1 / 2 + 1 + 0.4;
double intialZ = 7.4;


double firstSavedX = 0;
double firstSavedY = 0;
double firstSavedZ = 0;

double secondSavedX = 0;
double secondSavedY = 0;
double secondSavedZ = 0;

void drawWall(double thickness, int offSetX, int OffSetY, int OffSetz) {
	glPushMatrix();
	glTranslated(0.5 + offSetX, 0.5 * thickness + OffSetY, 0.5 + OffSetz);
	glScaled(1.0, thickness, 1.0);
	glutSolidCube(1);
	glPopMatrix();
}

void drawTableLeg(double thick, double len) {
	glColor3f(0, 0, 0);
	glPushMatrix();
	glTranslated(4, len / 2, 8);
	glScaled(thick, len, thick);
	glutSolidCube(1.0);
	glPopMatrix();
}

//weapon begin------------------------------------

void drawWeaponBase(double thick, double len) {
	glColor3f(0, 0, 0);
	glPushMatrix();
	glTranslated(4, len / 2, 8);
	glScaled(thick, len, thick);
	glutSolidCube(1.0);
	glPopMatrix();
}

void drawWeaponTop(double thick, double len) {
	glColor3f(0, 0, 0);
	glPushMatrix();
	glTranslated(4, len / 2 + len, 8);
	glScaled(2 * thick, 1, 4 * thick);
	glutSolidCube(1.0);
	glPopMatrix();
}

void controlReflection(double x, double y, double z) {
	if(shoot)
	if (x <= 0|| x  >= 8) {
		horizontalMove *= -1;
		intialZ = z;
		intialX = x;
		intialY = y;
		forwadBullet = 0;
	}
	if (y <= 0.3 || y >= 7.7) {
		//printf("hooooooooo");
		verticalMove *= -1;
		intialZ = z;
		intialX = x;
		intialY = y;
		forwadBullet = 0;
	}
	
}

void reset() {
	forwadBullet = 0;
	shoot = false;
	if (rounds == 1) {
		firstSavedX = currentBulletPositionX;
		firstSavedY = currentBulletPositionY;
		firstSavedZ = currentBulletPositionZ;
	}
	if (rounds == 2) {
		secondSavedX = currentBulletPositionX;
		secondSavedY = currentBulletPositionY;
		secondSavedZ = currentBulletPositionZ;
	}
	rounds++;
}

void drawBullet(double thick, double len, int ballNumber) {

	if (ballNumber > rounds) return;
	glColor3f(1, 1, 1);
		
	if (ballNumber < rounds) {
		if (ballNumber == 1) {
			glPushMatrix();
			glTranslated(firstSavedX, firstSavedY, firstSavedZ);
			glutSolidSphere(0.2, 20, 20);
			glPopMatrix();
		}
		if (ballNumber == 2) {
			glPushMatrix();
			glTranslated(secondSavedX, secondSavedY, secondSavedZ);
			glutSolidSphere(0.2, 20, 20);
			glPopMatrix();
		}
	}

	glPushMatrix();
	//glTranslated(intialX, intialY, intialZ - forwadBullet + backwadBullet);
	glTranslated(currentBulletPositionX, currentBulletPositionY, currentBulletPositionZ);
	currentBulletPositionZ = intialZ - forwadBullet + backwadBullet;
	if(verticalMove>0)
	currentBulletPositionY = intialY + abs(currentBulletPositionZ-intialZ)*tan(abs(verticalMove)*0.0174533);
	else
	currentBulletPositionY = intialY - abs(currentBulletPositionZ - intialZ)*tan(abs(verticalMove)*0.0174533);
	
	if (horizontalMove > 0) {
		currentBulletPositionX = intialX + abs(currentBulletPositionZ-intialZ) * tan(abs(horizontalMove)*0.0174533);
	}
	else {
	currentBulletPositionX = intialX - abs(currentBulletPositionZ - intialZ) * tan(abs(horizontalMove)*0.0174533);
	}
	controlReflection(currentBulletPositionX, currentBulletPositionY, currentBulletPositionZ);
 
	/*
	
	if (going && shoot) {
		printf(" x ");
		printf("%.3f", currentBulletPositionX);
		printf(" y ");
		printf("%.3f", currentBulletPositionY);
		printf(" z ");
		printf("%.3f", currentBulletPositionZ);

	}
	*/
	
	if (currentBulletPositionZ <= 0) {
		
		if (rounds < 3)
			reset();
		else
			going = false;
	}

	//glScaled(2 * thick, 1, 4 * thick);
	glutSolidSphere(0.2,20,20);
	glPopMatrix();
}

void drawWeapon() {
	glPushMatrix();
	drawWeaponBase(0.2, 1);
	//responsible for the movement of the upper part of the weapon
	//if (!shoot) {
	    glPushMatrix();
		glTranslated(4, (1 / 2 + 1), 8);
		glRotated(verticalMoveWeapon, 1, 0, 0);
		glRotated(-horizontalMoveWeapon, 0, 1, 0);
		glTranslated(-4, -(1 / 2 + 1), -8);
	    drawWeaponTop(0.2, 1);
		glPopMatrix();
	//}

		if (!shoot) {
		glPushMatrix();
		glTranslated(4, (1 / 2 + 1), 8);
		glRotated(verticalMove, 1, 0, 0);
		glRotated(-horizontalMove, 0, 1, 0);
		glTranslated(-4, -(1 / 2 + 1), -8);
		if(rounds== 1)
		drawBullet(0.2, 1, 1);
		if (rounds == 2)
		drawBullet(0.2, 1, 2);
		if (rounds == 3)
		drawBullet(0.2, 1, 3);
		glPopMatrix();
		drawBullet(0.2, 1, 1);
		drawBullet(0.2, 1, 2);
		}
		else {
        /*
		glPushMatrix();
		glTranslated(intialX, intialY, intialZ);
		glRotated(verticalMove, 1, 0, 0);
		glRotated(-horizontalMove, 0, 1, 0);
		glTranslated(-intialX, -intialY, -intialZ);
	    drawBullet(0.2, 1);
		glPopMatrix();
		
		*/
			//glTranslated(currentBulletPositionX, currentBulletPositionY, currentBulletPositionZ);
			drawBullet(0.2, 1, 1);
			drawBullet(0.2, 1, 2);
			drawBullet(0.2, 1, 3);
		}

		
	glPopMatrix();
}

//weapon end------------------------------------

void drawJackPart() {
	glPushMatrix();
	glScaled(0.2, 0.2, 1.0);
	glutSolidSphere(1, 15, 15);
	glPopMatrix();
	glPushMatrix();
	glTranslated(0, 0, 1.2);
	glutSolidSphere(0.2, 15, 15);
	glTranslated(0, 0, -2.4);
	glutSolidSphere(0.2, 15, 15);
	glPopMatrix();
}

void drawJack() {
	glPushMatrix();
	drawJackPart();
	glRotated(90.0, 0, 1, 0);
	drawJackPart();
	glRotated(90.0, 1, 0, 0);
	drawJackPart();
	glPopMatrix();
}

void drawTable(double topWid, double topThick, double legThick, double legLen) {
	glPushMatrix();
	glTranslated(0, legLen, 0);
	glScaled(topWid, topThick, topWid);
	glutSolidCube(1.0);
	glPopMatrix();

	double dist = 0.95*topWid / 2.0 - legThick / 2.0;
	glPushMatrix();
	glTranslated(dist, 0, dist);
	drawTableLeg(legThick, legLen);
	glTranslated(0, 0, -2 * dist);
	drawTableLeg(legThick, legLen);
	glTranslated(-2 * dist, 0, 2 * dist);
	drawTableLeg(legThick, legLen);
	glTranslated(0, 0, -2 * dist);
	drawTableLeg(legThick, legLen);
	glPopMatrix();
}

void setupLights() {
	GLfloat ambient[] = { 0.7f, 0.7f, 0.7, 1.0f };
	GLfloat diffuse[] = { 0.6f, 0.6f, 0.6, 1.0f };
	GLfloat specular[] = { 1.0f, 1.0f, 1.0, 1.0f };
	GLfloat shininess[] = { 50 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, shininess);

	GLfloat lightIntensity[] = { 0.7f, 0.7f, 1, 1.0f };
	GLfloat lightPosition[] = { -7.0f, 6.0f, 3.0f, 0.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, lightIntensity);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightIntensity);
}

void setupCamera() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//glOrtho(-4, 4, -4, 4, -5, 5);
	gluPerspective(60, 640 / 480, 0.001, 100);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(-0.31 + move, -0.3 + move, 0.25 + move, 0.21, 0.2, 0.0, 0.0, 1.0, 0.0);
}

//room begin---------------------------------------
void colorBrick(int c) {
	c %= 3;
	if (c == 0) glColor3f(1, 0, 0);
	if (c == 1) glColor3f(0, 1, 0);
	if (c == 2) glColor3f(0, 0, 1);
}

void buildBricks() {
	int color = 0;
	for (int width = 0; width < 8; width++) {
		for (int height = 0; height < 8; height++) {
			colorBrick(color);
			drawWall(0.2, width, 0, height);
			color++;
		}
	}
}

void createEndWall() {
	int color = 0;
	glPushMatrix();
	glRotated(-90, 1, 0, 0);
	buildBricks();
	glPopMatrix();

}
void createLeftWall() {
	glPushMatrix();
	glColor3f(0, 0, 1);
	glRotated(90, 0, 0, 1);
	buildBricks();
	glPopMatrix();
}

void createRightWall() {
	glPushMatrix();
	glTranslated(8, 0, 0);
	glColor3f(0, 0, 1);
	glRotated(90, 0, 0, 1);
	buildBricks();
	glPopMatrix();
}

void createGround() {
	glPushMatrix();
	int color = 0;
	glColor3f(0, 1, 0);
	buildBricks();
	glPopMatrix();
}

void createCeil() {
	glPushMatrix();
	glTranslated(0, 8, 0);
	int color = 0;
	glColor3f(0, 1, 0);
	buildBricks();
	glPopMatrix();
}
//room end---------------------------------

void Display() {
	setupLights();
	setupCamera();


	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glScaled(0.05, 0.05, 0.05);

	//room-----------------------------------------------
	createGround();
	createLeftWall();
	createRightWall();
	createEndWall();
	createCeil();
	//-----------------------------------------------------

	//weapon------------------------------------------------
	drawWeapon();
	//------------------------------------------------------

	glFlush();


}

void Anim()
{
	//horizontalMove += 1;
	/*
	if (inc == 1) {
		move += 0.0001;
		if (move >= 0.1)
			inc = 0;
	}
	else if (inc == 0) {
		move -= 0.0001;
		if (move <= 0)
			inc = 1;
	}
	*/
	if (shoot) {
		if (going)
			forwadBullet += 0.001;
	}
	glutPostRedisplay();
}

void keyboardFunc(int key, int x, int y) {
	/*
	printf(" angle ");
	printf("%.3f", abs(horizontalMove));
	printf(" value ");
		printf("%.3f", tan(abs(horizontalMove)*0.0174533));
	
	*/
	switch (key) {
	case GLUT_KEY_DOWN:verticalMove -= 10; verticalMoveWeapon -= 10; break;
	case GLUT_KEY_UP:verticalMove += 10; verticalMoveWeapon += 10; break;
	case GLUT_KEY_LEFT:horizontalMove -= 10; horizontalMoveWeapon -= 10; break;
	case GLUT_KEY_RIGHT:horizontalMove += 10; horizontalMoveWeapon += 10; break;
	
	}

}

void keyboardOtherButtons(unsigned char key , int x, int y) {
	switch (key)
	{
	case ' ': shoot = true; break;
	default:
		break;
	}
}

void main(int argc, char** argv) {
	glutInit(&argc, argv);

	glutInitWindowSize(1200, 800);
	glutInitWindowPosition(50, 50);
	glutCreateWindow("Lab 5");
	glutDisplayFunc(Display);

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);

	glShadeModel(GL_SMOOTH);
	glutIdleFunc(Anim);
	glutIdleFunc(Anim);
	glutSpecialFunc(keyboardFunc);
	glutKeyboardFunc(keyboardOtherButtons);
	glutMainLoop();
}



