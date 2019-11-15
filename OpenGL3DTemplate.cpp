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

int roundOneScore = 0;
int roundTwoScore = 0;
int roundThreeScore = 0;

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

char* instructions = "SCORING Red -1 Green 2 Blue 3 Purple 4 Cyan 5 ";
int shootLeft = 3;
int totalScore = 0;
int partialScore = 0;

bool laser = false;
bool screenUp = false;
bool greenScreenUp = false;
bool gunEx = false;
bool powerUp = false;
bool nextPhase = false;
bool afterNextPhase = false;
bool hitJarNear = false;
bool hitJarFar = false;
double rotateWeap = 0;
double screenAngle = 90;
double greenScreenAngle = 180;
double gunTube = 0;
double powerUpOut = 0;
double powerUpAngle = 0;

double yBubble1 = 1.3;
double yBubble2 = 1;
double yBubble3 = 0.7;
double yBubble4 = 0.4;

double bubbleG1 = 1.97;
double bubbleG2 = 1.87;


void rest() {
	horizontalMove = 0;
	verticalMove = 0;
	horizontalMoveWeapon = 0;
	verticalMoveWeapon = 0;
	hitJarNear = false;
	hitJarFar = false;
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
			roundOneScore = totalScore;
			partialScore = totalScore;
		}
		if (rounds == 2) {
			secondSavedX = currentBulletPositionX;
			secondSavedY = currentBulletPositionY;
			secondSavedZ = currentBulletPositionZ;
			roundTwoScore = totalScore - partialScore;
			partialScore = totalScore;
		}

		if (rounds == 3) {
			thirdSavedX = currentBulletPositionX;
			thirdSavedY = currentBulletPositionY;
			thirdSavedZ = currentBulletPositionZ;
			roundThreeScore = totalScore - partialScore;
			partialScore = totalScore;
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

	//red screen
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

	//green screen

	glPushMatrix();
	glTranslated(4, len / 2 + len + 0.3, 7.5);
	glRotated(greenScreenAngle, 0, 0, 1);
	glTranslated(-4, -(len / 2 + len + 0.3), -7.5);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(0, 1, 0, 0.8);
	glBegin(GL_POLYGON);
	glVertex3f(4 - 0.2, len / 2 + len + 0.4, 7.8);
	glVertex3f(4 + 0.2, len / 2 + len + 0.4, 7.8);
	glVertex3f(4 + 0.2, len / 2 + len + 0.6 + 1.3, 7.8);
	glVertex3f(4 - 0.2, len / 2 + len + 0.6 + 1.3, 7.8);
	glEnd();



	glColor3f(0, 0, 0);
	glBegin(GL_LINE_LOOP);
	glVertex3f(4 - 0.2, len / 2 + len + 0.4, 7.8);
	glVertex3f(4 + 0.2, len / 2 + len + 0.4, 7.8);
	glVertex3f(4 + 0.2, len / 2 + len + 0.6 + 1.3, 7.8);
	glVertex3f(4 - 0.2, len / 2 + len + 0.6 + 1.3, 7.8);
	glEnd();

	glPointSize(15);
	glBegin(GL_POINTS);
	if (rounds <= 3)
		glVertex3f(4, len / 2 + len + 0.8, 7.9);
	if (rounds <= 2)
		glVertex3f(4, len / 2 + len + 1.2, 7.9);
	if (rounds <= 1)
		glVertex3f(4, len / 2 + len + 1.6, 7.9);

	glEnd();

	glPopMatrix();



	//green


	//power up

	for (int i = 0; i < 4; i++) {

		double offSet = 0;
		double offSetX = 0;
		if (i == 0)
			offSet = 0.2;
		if (i == 2)
			offSet = -0.2;
		if (i == 1)
			offSetX = -0.2;
		if (i == 3)
			offSetX = 0.2;
		glPushMatrix();
		glTranslated(0, -0.4, 0.2);

		glTranslated(4, len / 2 + len + 0.3, 7.6);
		if (afterNextPhase)
			glRotated(rotateWeap, 0, 0, 1);
		glTranslated(-4, -(len / 2 + len + 0.3), -7.6);
		if (nextPhase) {

			glTranslated(4 + offSetX, len / 2 + len + 0.3 + offSet, 7.6);
			if (i == 0)
				glRotated(powerUpAngle, -1, 0, 0);

			if (i == 2)
				glRotated(powerUpAngle, 1, 0, 0);

			if (i == 1) {
				glRotated(powerUpAngle, 0, -1, 0);
			}


			if (i == 3)
				glRotated(powerUpAngle, 0, 1, 0);

			glTranslated(-4, -(len / 2 + len + 0.3 + offSet), -7.6);
		}

		glTranslated(4, len / 2 + len + 0.3 + offSet, 7.6);
		glRotated(90 * i, 0, 0, 1);
		glTranslated(-4, -(len / 2 + len + 0.3 + offSet), -7.6);

		glColor4f(1, 1, 1, 1);
		glBegin(GL_POLYGON);
		glVertex3f(4 - 0.2, len / 2 + len + 0.4 + offSet, 7.6);
		glVertex3f(4 + 0.2, len / 2 + len + 0.4 + offSet, 7.6);
		glVertex3f(4 + 0.2, len / 2 + len + 0.4 + powerUpOut + offSet, 7.6);
		glVertex3f(4 - 0.2, len / 2 + len + 0.4 + powerUpOut + offSet, 7.6);
		glEnd();



		glColor3f(0, 0, 0);
		glBegin(GL_LINE_LOOP);
		glVertex3f(4 - 0.2, len / 2 + len + 0.4 + offSet, 7.6);
		glVertex3f(4 + 0.2, len / 2 + len + 0.4 + offSet, 7.6);
		glVertex3f(4 + 0.2, len / 2 + len + 0.4 + powerUpOut + offSet, 7.6);
		glVertex3f(4 - 0.2, len / 2 + len + 0.4 + powerUpOut + offSet, 7.6);
		glEnd();


		glPopMatrix();



	}

	//power up


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
	glTranslated(4, len / 2 + 0.9, 7.7);
	glRotated(180, 1, 0, 0);
	glTranslated(-4, -(len / 2 + 0.9), -7.7);
	glTranslated(4, len / 2 + 0.9, 7.7);
	GLUquadricObj *quad;
	quad = gluNewQuadric();
	gluCylinder(quad, 0.16, 0.16, 0.3 + gunTube, 50, 50);
	glPopMatrix();

	if (laser) {
		glPushMatrix();
		glColor3f(1, 0, 0);
		glBegin(GL_LINES);
		glVertex3f(4, len / 2 + len, 8);
		glVertex3f(4, len / 2 + len, 0);
		glEnd();
		glPopMatrix();
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

void checkJars(double x, double y, double z) {
	if (!replaying && !hitJarNear) {
		if (x <= 2 && x >= 1 && y <= 2 && y >= 0 && z <= 6 && z >= 5) {
			totalScore -= 10;
			hitJarNear = true;
		}
	}

	if (!replaying && !hitJarFar) {
		if (x <= 6.6 && x >= 5.5 && y <= 2 && y >= 0 && z <= 3.5 && z >= 2.5) {
			totalScore -= 10;
			hitJarFar = true;
		}
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
	checkJars(currentBulletPositionX, currentBulletPositionY, currentBulletPositionZ);

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

//red = -1, green = 2, blue = 3, purple = 4, cyan = 5

int getLeftScore(double x, double y, double z) {
	int row = y / 1;
	int column = z / 1;
	row %= 5;
	int returnValue;
	if (row == 0) {
		returnValue = ((column % 5) + 1) == 1 ? -1 : (column % 5) + 1;
	}
	else if (row == 1) {
		returnValue = (((column + 3) % 5) + 1) == 1 ? -1 : ((column + 3) % 5) + 1;
	}
	else if (row == 2) {
		returnValue = (((column + 1) % 5) + 1) == 1 ? -1 : ((column + 1) % 5) + 1;
	}
	else if (row == 3) {
		returnValue = (((column + 4) % 5) + 1) == 1 ? -1 : ((column + 4) % 5) + 1;
	}
	else {
		returnValue = (((column + 2) % 5) + 1) == 1 ? -1 : ((column + 2) % 5) + 1;
	}
	if (returnValue == -1) PlaySound(TEXT("media.io_r.wav"), NULL, SND_FILENAME);
	else if (returnValue == 2) PlaySound(TEXT("media.io_g.wav"), NULL, SND_FILENAME);
	else if (returnValue == 3) PlaySound(TEXT("media.io_b.wav"), NULL, SND_FILENAME);
	else if (returnValue == 4) PlaySound(TEXT("media.io_p.wav"), NULL, SND_FILENAME);
	else if (returnValue == 5) PlaySound(TEXT("media.io_c.wav"), NULL, SND_FILENAME);
	return returnValue;
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
	PlaySound(TEXT("media.io_sticky.wav"), NULL, SND_FILENAME);
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
	c %= 5;
	if (c == 0) glColor3f(1, 0, 0);
	if (c == 1) glColor3f(0, 1, 0);
	if (c == 2) glColor3f(0, 0, 1);
	if (c == 3) glColor3f(1, 0, 1);
	if (c == 4) glColor3f(0, 1, 1);
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
	print(0, 6, 7, (char*)("Toxic Jar Score: -10"));
	char* shots[20];
	sprintf((char*)shots, "Shots left : %d", shootLeft);
	print(6, 7, 7, (char*)shots);
	print(0, 7, 7, (char*)(instructions));
	if (cameraEnhanced)
		print(0, 5, 7, (char*)("Camera mode : Enhanced"));
	else if (!cameraEnhanced)
		print(0, 5, 7, (char*)("Camera mode : Simple"));
	else if (replaying)
		print(0, 5, 7, (char*)("Camera mode : Replaying"));
	if (shootLeft == 0) {
		glColor3f(0, 0, 0);
		print(3, 5, 7, (char*)("Game Over"));
	}
	glColor3f(0, 0, 0);
	print(6, 6, 7, (char*)("Controls: Up Key-> up"));
	print(6, 5, 7, (char*)("Down Key->down"));
	print(6, 4, 7, (char*)("Right Key->right"));
	print(6, 3, 7, (char*)("Left Key->left"));
	char* roundOne[20];
	char* roundTwo[20];
	char* roundThree[20];
	sprintf((char*)roundOne, "Round One: %d", roundOneScore);
	sprintf((char*)roundTwo, "Round Two: %d", roundTwoScore);
	sprintf((char*)roundThree, "Round Three: %d", roundThreeScore);
	print(0, 4, 7, (char*)(roundOne));
	print(0, 3, 7, (char*)(roundTwo));
	print(0, 2, 7, (char*)(roundThree));
}

void jars() {
	glPushMatrix();
	glTranslated(1.5, 2, 5.5);
	glRotated(90, 1, 0, 0);
	glColor3f(0, 0, 1);
	GLUquadricObj *quad;
	quad = gluNewQuadric();
	gluCylinder(quad, 0.5, 0.5, 2, 50, 50);
	glPopMatrix();


	glPopMatrix();

	glColor3f(0, 0, 0);
	glPushMatrix();
	glTranslatef(1.61, 1.4, 6);
	GLUquadric *quadObjz = gluNewQuadric();
	gluDisk(quadObjz, 0, 0.25, 50, 50);
	glPopMatrix();
	

	glColor3f(0, 1, 0);
	glBegin(GL_POLYGON);
	glVertex3f(1.75,1.6,6.1);
	glVertex3f(1.5, 1.6, 6.1);
	glVertex3f(1.5, 0, 6.1);
	glVertex3f(1.75, 0, 6.1);
	glEnd();


	glPushMatrix();
	glTranslatef(1.62, yBubble1, 6.1);
	GLUquadric *quadObj = gluNewQuadric();
	gluDisk(quadObj, 0, 0.2, 50, 50);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1.62, yBubble2, 6.1);
	GLUquadric *quadObj2 = gluNewQuadric();
	gluDisk(quadObj2, 0, 0.2, 50, 50);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1.62, yBubble3, 6.1);
	GLUquadric *quadObj3 = gluNewQuadric();
	gluDisk(quadObj3, 0, 0.2, 50, 50);
	glPopMatrix();


	glPushMatrix();
	glTranslatef(1.62, yBubble4, 6.1);
	GLUquadric *quadObj4 = gluNewQuadric();
	gluDisk(quadObj4, 0, 0.2, 50, 50);
	glPopMatrix();

	//up
	glPushMatrix();
	glTranslated(1.5, 2, 5.5);
	glRotated(90, 1, 0, 0);
	GLUquadric *quadObj5 = gluNewQuadric();
	gluDisk(quadObj5, 0, 0.5, 50, 50);
	glPopMatrix();

	//7az
	glColor3f(0, 0, 0);
	glPushMatrix();
	glTranslated(1.5, 2, 5.5);
	glRotated(90, 1, 0, 0);
	GLUquadric *quadObjk = gluNewQuadric();
	gluDisk(quadObjk, 0, 0.55, 50, 50);
	glPopMatrix();

	glColor3f(0, 0, 0);
	glPushMatrix();
	glTranslated(1.5, 1.25, 5.5);
	glRotated(90, 1, 0, 0);
	GLUquadric *quadObjk2 = gluNewQuadric();
	gluDisk(quadObjk2, 0, 0.55, 50, 50);
	glPopMatrix();

	glColor3f(0, 0, 0);
	glPushMatrix();
	glTranslated(1.5, 0.3, 5.5);
	glRotated(90, 1, 0, 0);
	GLUquadric *quadObjk3 = gluNewQuadric();
	gluDisk(quadObjk3, 0, 0.55, 50, 50);
	glPopMatrix();

	glColor3f(0, 1, 0);
	//bubbles
	glPushMatrix();
	glTranslated(1.5, bubbleG1, 5.5);
	glutSolidSphere(0.11, 20, 20);
	glPopMatrix();

	glPushMatrix();
	glTranslated(1.2, bubbleG2, 5.5);
	glutSolidSphere(0.11, 20, 20);
	glPopMatrix();

	glPushMatrix();
	glTranslated(1.8, bubbleG2, 5.5);
	glutSolidSphere(0.11, 20, 20);
	glPopMatrix();


	glPushMatrix();
	glTranslated(1.35, bubbleG1, 5.7);
	glutSolidSphere(0.11, 20, 20);
	glPopMatrix();

	glPushMatrix();
	glTranslated(1.65, bubbleG1, 5.7);
	glutSolidSphere(0.11, 20, 20);
	glPopMatrix();

}

void jars2() {
	glPushMatrix();
	glTranslated(6, 2, 3);
	glRotated(90, 1, 0, 0);
	glColor3f(0, 0, 1);
	GLUquadricObj *quad;
	quad = gluNewQuadric();
	gluCylinder(quad, 0.5, 0.5, 2, 50, 50);
	glPopMatrix();


	glPopMatrix();

	glColor3f(0, 0, 0);
	glPushMatrix();
	glTranslatef(6.11, 1.4, 2.5);
	GLUquadric *quadObjz = gluNewQuadric();
	gluDisk(quadObjz, 0, 0.25, 50, 50);
	glPopMatrix();


	glColor3f(0, 1, 0);




	//up
	glPushMatrix();
	glTranslated(6, 2, 3);
	glRotated(90, 1, 0, 0);
	GLUquadric *quadObj5 = gluNewQuadric();
	gluDisk(quadObj5, 0, 0.5, 50, 50);
	glPopMatrix();

	//7az
	glColor3f(0, 0, 0);
	glPushMatrix();
	glTranslated(6, 2, 3);
	glRotated(90, 1, 0, 0);
	GLUquadric *quadObjk = gluNewQuadric();
	gluDisk(quadObjk, 0, 0.55, 50, 50);
	glPopMatrix();

	glColor3f(0, 0, 0);
	glPushMatrix();
	glTranslated(6, 1.25, 3);
	glRotated(90, 1, 0, 0);
	GLUquadric *quadObjk2 = gluNewQuadric();
	gluDisk(quadObjk2, 0, 0.55, 50, 50);
	glPopMatrix();

	glColor3f(0, 0, 0);
	glPushMatrix();
	glTranslated(6, 0.3, 3);
	glRotated(90, 1, 0, 0);
	GLUquadric *quadObjk3 = gluNewQuadric();
	gluDisk(quadObjk3, 0, 0.55, 50, 50);
	glPopMatrix();

	glColor3f(0, 1, 0);
	//bubbles
	glPushMatrix();
	glTranslated(6, bubbleG1, 3);
	glutSolidSphere(0.11, 20, 20);
	glPopMatrix();

	glPushMatrix();
	glTranslated(5.7, bubbleG2, 3);
	glutSolidSphere(0.11, 20, 20);
	glPopMatrix();

	glPushMatrix();
	glTranslated(6.3, bubbleG2, 3);
	glutSolidSphere(0.11, 20, 20);
	glPopMatrix();


	glPushMatrix();
	glTranslated(5.85, bubbleG1, 3.2);
	glutSolidSphere(0.11, 20, 20);
	glPopMatrix();

	glPushMatrix();
	glTranslated(6.15, bubbleG1, 3.2);
	glutSolidSphere(0.11, 20, 20);
	glPopMatrix();

}


void Display() {
	setupLights();
	setupCamera();


	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	glScaled(0.05, 0.05, 0.05);

	glPushMatrix();
	jars();
	glPopMatrix();

	glPushMatrix();
	jars2();
	glPopMatrix();

	//dashboard
	dashBoard();
	//

	//room-----------------------------------------------
	glPushMatrix();
	createGround();
	createLeftWall();
	createRightWall();
	createEndWall();
	createCeil();
	glPopMatrix();
	//-----------------------------------------------------

	//weapon------------------------------------------------
	glPushMatrix();
	drawWeapon();
	glPopMatrix();
	//------------------------------------------------------



	glFlush();


}

void Anim()
{

	bubbleG1 = bubbleG1 < 2 ? bubbleG1 + 0.001 : 1.85;
	bubbleG2 = bubbleG2 < 2 ? bubbleG2 + 0.001 : 1.80;

	yBubble1 = yBubble1>=0 ? yBubble1-0.01 : 1.3;
	yBubble2 = yBubble2 >= 0 ? yBubble2 - 0.01 : 1.3;
	yBubble3 = yBubble3 >= 0 ? yBubble3 - 0.01 : 1.3;
	yBubble4 = yBubble4 >= 0 ? yBubble4 - 0.01 : 1.3;
	

	if (powerUp) {
		if (powerUpOut < 1.5) {
			powerUpOut += 0.01;;
		}
		else {
			nextPhase = true;
			if (powerUpAngle < 90) {
				powerUpAngle += 0.1;
			}
			else {
				afterNextPhase = true;
			}
		}
		if (afterNextPhase)
			rotateWeap += 0.1;
	}
	else {
		if (powerUpAngle > 0) {
			powerUpAngle -= 0.1;
		}
		else if (powerUpOut > 0) {
			powerUpOut -= 0.01;
		}
	}

	if (greenScreenUp) {
		if (greenScreenAngle > 90) {
			greenScreenAngle -= 0.1;
		}
	}
	else {
		if (greenScreenAngle < 180) {
			greenScreenAngle += 0.1;
		}
	}
	if (gunEx) {
		if (gunTube < 1) {
			gunTube += 0.01;
		}
	}
	else {
		if (gunTube > 0) {
			gunTube -= 0.01;
		}
	}
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
		case GLUT_KEY_DOWN:
			if (verticalMove > -80) { 
				verticalMove -= 10; 
				verticalMoveWeapon -= 10;
			}
			else {
				PlaySound(TEXT("out_of_bounds.wav"), NULL, SND_FILENAME);
			}
			break;
		case GLUT_KEY_UP:
			if (verticalMove < 80) { 
				verticalMove += 10; 
				verticalMoveWeapon += 10;
			}
			else {
				PlaySound(TEXT("out_of_bounds.wav"), NULL, SND_FILENAME);
			}
			break;
		case GLUT_KEY_LEFT:
			if (horizontalMove > -80) {
				horizontalMove -= 10;
				horizontalMoveWeapon -= 10;
			}
			else {
				PlaySound(TEXT("out_of_bounds.wav"), NULL, SND_FILENAME);
			}
			break;
		case GLUT_KEY_RIGHT:
			if (horizontalMove < 80) { 
				horizontalMove += 10; 
				horizontalMoveWeapon += 10; 
			}
			else { 
				PlaySound(TEXT("out_of_bounds.wav"), NULL, SND_FILENAME); 
			} 
			break;
		}
	}
}

void keyboardOtherButtons(unsigned char key, int x, int y) {
	//printf("%f %f %f %f\n", horizontalMove, verticalMove, horizontalMoveWeapon, verticalMoveWeapon);
	//printf("%f %f %f %f\n", prevHorizontalMove, prevVerticalMove, prevHorizontalMoveWeapon, prevVerticalMoveWeapon);
	if (!shoot) {
		switch (key)
		{
		case ' ': shoot = true & (rounds < 4);
			if (shoot) {
				prevVerticalMove = verticalMove;
				prevVerticalMoveWeapon = verticalMoveWeapon;
				prevHorizontalMove = horizontalMove;
				prevHorizontalMoveWeapon = horizontalMoveWeapon;
				PlaySound(TEXT("media.io_shooting_2.wav"), NULL, SND_FILENAME);
			} break;
		case 'c': cameraEnhanced = !cameraEnhanced; break;
		case 'r': replaying = true & !shoot & (shootLeft < 3); shoot = replaying; break;
		case 'l': laser = !laser; break;
		case 'x': gunEx = !gunEx; break;
		case 'g': greenScreenUp = !greenScreenUp; break;
		case 'p': powerUp = !powerUp; break;
		case 'u': powerUp = !powerUp; greenScreenUp = !greenScreenUp;  gunEx = !gunEx; laser = !laser; screenUp = !screenUp; break;
		case 's': screenUp = !screenUp; if (screenUp) PlaySound(TEXT("media.io_weapon.wav"), NULL, SND_FILENAME); break;
		default:
			break;
		}
	}
}

void main(int argc, char** argv) {
	glutInit(&argc, argv);

	glutInitWindowSize(1200, 800);
	glutInitWindowPosition(50, 50);
	glutCreateWindow("Assignment 2");
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
