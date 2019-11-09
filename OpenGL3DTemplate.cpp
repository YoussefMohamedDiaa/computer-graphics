#include <stdlib.h> 
#include <cmath>
#include <string.h>
#include <glut.h>
#include <math.h>
#include <ctime>
#include <cstdio>
#include <windows.h>
#include <mmsystem.h>

// forwardBullet, bool replaying, dont increment score, store previous horizontal vertical move aka previous position

int getLeftScore(double x, double y, double z);
int getRightScore(double x, double y, double z);
int getCeilScore(double x, double y, double z);
int getEndScore(double x, double y, double z);
int getFloorScore(double x, double y, double z);
void setupLights();

double move = 0.5;
int inc = 1;
int rounds = 1;

double horizontalMove = 0;
double verticalMove = 0;
double prevHorizontalMove = 0;
double prevVerticalMove = 0;
double horizontalMoveWeapon = 0;
double verticalMoveWeapon = 0;
double prevHorizontalMoveWeapon = 0;
double prevVerticalMoveWeapon = 0;
int numberOfHorizontalReflections = 0;
int numberOfVerticalReflections = 0;


bool shoot = false;

bool going = true;
bool cameraEnhanced = true;
double forwardBullet = 0;
double backwadBullet = 0;
double forwardCam = 0;
double horizontalCam = 0;
double verticalCam = 0;

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

double thirdSavedX = 0;
double thirdSavedY = 0;
double thirdSavedZ = 0;

bool replaying = false;

char* instructions = "SCORING Red: -1 Green: 2 Blue: 3 ";
int shootLeft = 3;
int totalScore = 0;

bool laser = false;
bool screenUp = false;
double screenAngle = 90;

void rest() {
	horizontalMove = 0;
	verticalMove = 0;
	horizontalMoveWeapon = 0;
	verticalMoveWeapon = 0;
	if (numberOfHorizontalReflections % 2 == 1)
		prevHorizontalMove *= -1;
	if (numberOfVerticalReflections % 2 == 1)
		prevVerticalMove *= -1;
	numberOfHorizontalReflections = 0; numberOfVerticalReflections = 0;
	shoot = false;

	going = true;
	forwardBullet = 0;
	backwadBullet = 0;


	intialX = 4;
	intialY = 1 / 2 + 1 + 0.4;
	intialZ = 7.4;
	if (!replaying) {
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

		if (rounds == 3) {
			thirdSavedX = currentBulletPositionX;
			thirdSavedY = currentBulletPositionY;
			thirdSavedZ = currentBulletPositionZ;
		}
		currentBulletPositionX = -1;
		currentBulletPositionY = -1;
		currentBulletPositionZ = -1;
	}
	if (!replaying)
		rounds++;
	replaying = false;
}

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
	glScaled(thick, len + 0.5, thick);
	glutSolidCube(1.0);
	glPopMatrix();
}

void drawWeaponTop(double thick, double len) {
	glColor3f(0, 0, 0);
	glPushMatrix();
	glColor3f(1, 183.0 / 255.0, 51.0 / 255.0);
	glTranslated(4, len / 2 + len, 8);
	glScaled(2 * thick, 0.5, 4 * thick);
	glutSolidCube(1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslated(4, len / 2 + len + 0.3, 8);
	glScaled((2 * thick) / 2.0, 0.5 / 2.0, (4 * thick) / 4.0);
	glutSolidCube(1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslated(4, len / 2 + len + 0.3, 8);
	glRotated(screenAngle, 1, 0, 0);
	glTranslated(-4, -(len / 2 + len + 0.3), -8);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(1, 0, 0, 0.5);
	glBegin(GL_POLYGON);
	glVertex3f(4 - 0.4, len / 2 + len + 0.4, 8);
	glVertex3f(4 + 0.4, len / 2 + len + 0.4, 8);
	glVertex3f(4 + 0.4, len / 2 + len + 0.4 + 0.9, 8);
	glVertex3f(4 - 0.4, len / 2 + len + 0.4 + 0.9, 8);
	glEnd();



	glColor3f(0, 0, 0);
	glBegin(GL_LINE_LOOP);
	glVertex3f(4 - 0.4, len / 2 + len + 0.4, 8);
	glVertex3f(4 + 0.4, len / 2 + len + 0.4, 8);
	glVertex3f(4 + 0.4, len / 2 + len + 0.4 + 0.9, 8);
	glVertex3f(4 - 0.4, len / 2 + len + 0.4 + 0.9, 8);
	glEnd();

	glBegin(GL_LINES);
	glVertex3f(4 - 0.3, len / 2 + len + 0.4 + 0.45, 8.01);
	glVertex3f(3.9, len / 2 + len + 0.4 + 0.45, 8.01);
	glVertex3f(4.1, len / 2 + len + 0.4 + 0.45, 8.01);
	glVertex3f(4 + 0.3, len / 2 + len + 0.4 + 0.45, 8.01);

	glVertex3f(4, len / 2 + len + 0.4 + 0.45 + 0.1, 8.01);
	glVertex3f(4, len / 2 + len + 0.4 + 0.45 + 0.3, 8.01);
	glVertex3f(4, len / 2 + len + 0.4 + 0.45 - 0.1, 8.01);
	glVertex3f(4, len / 2 + len + 0.4 + 0.45 - 0.3, 8.01);

	glEnd();


	glPopMatrix();

	glPushMatrix();
	glTranslated(4, len / 2 + len + 0.3, 8);
	glScaled((2 * thick) / 2.0, 0.5 / 2.0, (4 * thick) / 4.0);
	glutSolidCube(1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslated(4, len / 2 + len, 8.5);
	glScaled((2 * thick) / 2.0, 0.5 / 2.0, (4 * thick) / 2.0);
	glutSolidCube(1.0);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1, 183.0 / 255.0, 51.0 / 255.0);
	glTranslated(4, len / 2 + len, 8.8);
	glScaled((2 * thick) / 4.0, 0.5, (4 * thick) / 4.0);
	glutSolidCube(1.0);
	glPopMatrix();

	glColor3f(0, 0, 0);
	glPushMatrix();
	glTranslated(4, len / 2 + 0.6, 8.3);
	glScaled(thick / 3, len, thick / 3);
	glutSolidCube(1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslated(4, len / 2 + 0.77, 7.8);
	glRotated(30, 1, 0, 0);
	glTranslated(-4, -(len / 2 + 0.77), -7.8);
	glTranslated(4, len / 2 + 0.77, 7.8);
	glScaled(thick / 3, len, thick / 3);
	glutSolidCube(1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslated(4, len / 2 + 0.9, 6.5);
	GLUquadricObj *quad;
	quad = gluNewQuadric();
	gluCylinder(quad, 0.16, 0.16, 1.3, 50, 50);
	glPopMatrix();

	if (laser) {
		glPushMatrix();
		glColor3f(1, 0, 0);
		glBegin(GL_LINES);
		glVertex3f(4, len / 2 + len, 8);
		glVertex3f(4, len / 2 + len, 0);
		glEnd();
	}

	glPushMatrix();
	glColor3f(0, 0, 0);
	glTranslated(4, len / 2 + len, 7.8);
	glutSolidTorus(0.1, 0.3, 50, 90);
	glEnd();

	glPopMatrix();
}


void controlReflection(double x, double y, double z) {
	//if(shoot)
	if (x <= 0 || x >= 8) {
		if (!replaying) {
			if (x <= 0) {
				totalScore += getLeftScore(x, y, z);
				//printf("Left score is %d\n", getLeftScore(x, y, z));
			}
			if (x >= 8) {
				totalScore += getRightScore(x, y, z);
				//printf("Right score is %d\n", getRightScore(x, y, z));
			}
		}
		if (replaying) {
			prevHorizontalMove *= -1;
			numberOfHorizontalReflections++;
		}
		else
			horizontalMove *= -1;
		intialZ = z;
		intialX = x;
		intialY = y;
		forwardBullet = 0;

	}
	if (y <= 0.3 || y >= 7.7) {
		if (!replaying) {
			if (y <= 0.3) {
				totalScore += getFloorScore(x, y, z);
				//printf("Floor score is %d\n", getFloorScore(x, y, z));
			}
			if (y >= 7.7) {
				totalScore += getCeilScore(x, y, z);
				//printf("Ceil score is %d\n", getCeilScore(x, y, z));
			}
		}
		if (replaying) {
			prevVerticalMove *= -1;
			numberOfVerticalReflections++;
		}
		else
			verticalMove *= -1;
		intialZ = z;
		intialX = x;
		intialY = y;
		forwardBullet = 0;
	}

}


void drawBullet(double thick, double len) {

	glColor3f(1, 1, 1);


	glPushMatrix();

	//glTranslated(intialX, intialY, intialZ - forwadBullet + backwadBullet);
	glTranslated(currentBulletPositionX, currentBulletPositionY, currentBulletPositionZ);
	currentBulletPositionZ = intialZ - forwardBullet + backwadBullet;
	if ((replaying ? prevVerticalMove : verticalMove) > 0)
		currentBulletPositionY = intialY + abs(currentBulletPositionZ - intialZ)*tan(abs(replaying ? prevVerticalMove : verticalMove)*0.0174533);
	else
		currentBulletPositionY = intialY - abs(currentBulletPositionZ - intialZ)*tan(abs(replaying ? prevVerticalMove : verticalMove)*0.0174533);

	if ((replaying ? prevHorizontalMove : horizontalMove) > 0) {
		currentBulletPositionX = intialX + abs(currentBulletPositionZ - intialZ) * tan(abs(replaying ? prevHorizontalMove : horizontalMove)*0.0174533);
	}
	else {
		currentBulletPositionX = intialX - abs(currentBulletPositionZ - intialZ) * tan(abs(replaying ? prevHorizontalMove : horizontalMove)*0.0174533);
	}
	controlReflection(currentBulletPositionX, currentBulletPositionY, currentBulletPositionZ);


	if (currentBulletPositionZ <= 0) {
		if (!replaying)
			totalScore += getEndScore(currentBulletPositionX, currentBulletPositionY, currentBulletPositionZ);
		//printf("End score is %d\n", getEndScore(currentBulletPositionX, currentBulletPositionY, currentBulletPositionZ));
		going = false;
		if (rounds < 4 && !replaying)
			shootLeft--;
		rest();

	}

	//glScaled(2 * thick, 1, 4 * thick);
	glutSolidSphere(0.15, 20, 20);
	glPopMatrix();
}

void preserveHistory() {
	glPushMatrix();
	glColor3f(1, 1, 1);
	if (rounds > 1) {
		glPushMatrix();
		glTranslated(firstSavedX, firstSavedY, firstSavedZ);
		glutSolidSphere(0.2, 20, 20);
		glPopMatrix();
	}

	if (rounds > 2) {
		glPushMatrix();
		glTranslated(secondSavedX, secondSavedY, secondSavedZ);
		glutSolidSphere(0.2, 20, 20);
		glPopMatrix();
	}

	if (rounds > 3) {
		glPushMatrix();
		glTranslated(thirdSavedX, thirdSavedY, thirdSavedZ);
		glutSolidSphere(0.2, 20, 20);
		glPopMatrix();
	}

	glPopMatrix();
}

void drawWeapon() {
	preserveHistory();
	glPushMatrix();
	drawWeaponBase(0.2, 1);
	//responsible for the movement of the upper part of the weapon
	//if (!shoot) {
	glPushMatrix();
	glTranslated(4, (1 / 2 + 1), 8);
	glRotated(replaying ? prevVerticalMoveWeapon : verticalMoveWeapon, 1, 0, 0);
	glRotated(replaying ? -prevHorizontalMoveWeapon : -horizontalMoveWeapon, 0, 1, 0);
	glTranslated(-4, -(1 / 2 + 1), -8);
	//for moving lights
	//setupLights();
	drawWeaponTop(0.2, 1);
	glPopMatrix();
	//}

	if (!shoot) {
		glPushMatrix();
		glTranslated(4, (1 / 2 + 1), 8);
		glRotated(replaying ? prevVerticalMove : verticalMove, 1, 0, 0);
		glRotated(replaying ? prevHorizontalMove : -horizontalMove, 0, 1, 0);
		glTranslated(-4, -(1 / 2 + 1), -8);

		if (rounds < 4)
			drawBullet(0.2, 1);
		glPopMatrix();

	}
	else {

		if (rounds < 4 || replaying)
			drawBullet(0.2, 1);

	}


	glPopMatrix();
}

//weapon end------------------------------------

//score

//red = -1, green = 2, blue = 3

int getLeftScore(double x, double y, double z) {
	int row = y / 1;
	int column = z / 1;
	row %= 3;
	if (row == 0) {
		return ((column % 3) + 1) == 1 ? -1 : (column % 3) + 1;
	}
	if (row == 1) {
		return (((column + 2) % 3) + 1) == 1 ? -1 : ((column + 2) % 3) + 1;
	}
	else {
		return (((column + 1) % 3) + 1) == 1 ? -1 : ((column + 1) % 3) + 1;
	}
}

int getRightScore(double x, double y, double z) {
	return getLeftScore(x - 8, y, z);
}

int getFloorScore(double x, double y, double z) {
	return getLeftScore(y, x, z);
}
int getCeilScore(double x, double y, double z) {
	return getFloorScore(x, y, z);
}
int getEndScore(double x, double y, double z) {
	return getLeftScore(z, x, y);
}

//score

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
	gluLookAt(-0.31 + move - horizontalCam, -0.3 + move + verticalCam, 0.25 + move - forwardCam, 0.21, 0.2, 0.0, 0.0, 1.0, 0.0);
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

void print(int x, int y, int z, char* string)
{
	int len, i;

	glRasterPos3f(x, y, z);

	len = (int)strlen(string);

	for (i = 0; i < len; i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, string[i]);
	}
}

void dashBoard() {
	char* score[20];
	sprintf((char*)score, "Score : %d", totalScore);
	print(4, 7, 7, (char*)score);
	char* shots[20];
	sprintf((char*)shots, "Shots left : %d", shootLeft);
	print(6, 7, 7, (char*)shots);
	print(0, 7, 7, (char*)(instructions));
	if (cameraEnhanced)
		print(0, 6, 7, (char*)("Camera mode : Enhanced"));
	else if (!cameraEnhanced)
		print(0, 6, 7, (char*)("Camera mode : Simple"));
	else if (replaying)
		print(0, 6, 7, (char*)("Camera mode : Replaying"));
}

void Display() {
	setupLights();
	setupCamera();


	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	glScaled(0.05, 0.05, 0.05);


	//dashboard
	dashBoard();
	//

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
	if (screenUp) {
		if (screenAngle > 0) {
			screenAngle -= 0.1;
		}
	}
	else {
		if (screenAngle < 90) {
			screenAngle += 0.1;
		}
	}
	if (shoot) {
		if (going) {
			if (!replaying)
				forwardBullet += 0.01;
			else
				forwardBullet += 0.001;
			float shift = replaying ? 0.000025 : 0.00015;
			forwardCam += shift;
			if (cameraEnhanced) {
				if ((replaying ? prevHorizontalMove : horizontalMove) > 0)
					horizontalCam += shift;
				if ((replaying ? prevHorizontalMove : horizontalMove) < 0)
					horizontalCam -= shift;
				if ((replaying ? prevVerticalMove : verticalMove) > 0)
					verticalCam += shift;
				if ((replaying ? prevVerticalMove : verticalMove) < 0)
					verticalCam -= shift;
			}
		}
	}
	else {
		verticalCam = 0;
		horizontalCam = 0;
		forwardCam = 0;
	}
	glutPostRedisplay();
}

void keyboardFunc(int key, int x, int y) {
	if (!replaying && !shoot) {
		switch (key) {
		case GLUT_KEY_DOWN:verticalMove -= 10; verticalMoveWeapon -= 10; break;
		case GLUT_KEY_UP:verticalMove += 10; verticalMoveWeapon += 10; break;
		case GLUT_KEY_LEFT:horizontalMove -= 10; horizontalMoveWeapon -= 10; break;
		case GLUT_KEY_RIGHT:horizontalMove += 10; horizontalMoveWeapon += 10; break;
		}
	}
}

void keyboardOtherButtons(unsigned char key, int x, int y) {
	//printf("%f %f %f %f\n", horizontalMove, verticalMove, horizontalMoveWeapon, verticalMoveWeapon);
	//printf("%f %f %f %f\n", prevHorizontalMove, prevVerticalMove, prevHorizontalMoveWeapon, prevVerticalMoveWeapon);
	if (!shoot) {
		switch (key)
		{
		case ' ': shoot = true & (rounds < 4); if (shoot) { prevVerticalMove = verticalMove; prevVerticalMoveWeapon = verticalMoveWeapon; prevHorizontalMove = horizontalMove; prevHorizontalMoveWeapon = horizontalMoveWeapon; } break;
		case 'c': cameraEnhanced = !cameraEnhanced; break;
		case 'r': replaying = true & !shoot & (shootLeft < 3); shoot = replaying; break;
		case 'l': laser = !laser; break;
		case 's': screenUp = !screenUp; break;
		default:
			break;
		}
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
