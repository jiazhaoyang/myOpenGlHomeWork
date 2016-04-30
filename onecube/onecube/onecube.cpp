// onecube.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "gl/glut.h"
#include <math.h>
#include <stdio.h>

// 窗口大小
int width = 800;
int height = 600;

// 鼠标转换
GLfloat xRotation = 0.0f, yRotation = 0.0f;
GLfloat scale = 1.0f, lastScl = 1.0f;
GLfloat xTransl = 0.0f, yTransl = 0.0f, zTransl = 0.0f;
const GLfloat rotSpeed = 0.01f;
GLfloat translSpeed = 0.05f;

//鼠标状态
bool bLButtonDown = false, bMButtonDown = false, bRButtonDown = false;
GLint lastX, lastY;

// 立方体
GLfloat cubic_size = 0.5f;
GLuint icube = NULL;

void RenderCube();

void init()
{

	// 深度测试
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);


	icube = glGenLists(1);
	glNewList(icube, GL_COMPILE);
	RenderCube();
	glEndList();

}

void destroy()
{

	if (icube != NULL)
		glDeleteLists(icube, 1);

}

void mouse(int button, int state, int x, int y)
{

	switch (button) {

	case GLUT_LEFT_BUTTON://当鼠标左键点击的时候旋转立方体
		lastX = x;
		lastY = y;
		bLButtonDown = state == GLUT_DOWN;
		break;

	case GLUT_MIDDLE_BUTTON:// 当鼠标中建点击的时候移动立方体
		bMButtonDown = state == GLUT_DOWN;
		if (bMButtonDown) {

			lastX = x;
			lastY = y;

		}
		break;

	case GLUT_RIGHT_BUTTON:// 当鼠标右键点击的时候放大立方体
		bRButtonDown = state == GLUT_DOWN;
		if (bRButtonDown) {

			lastScl = scale;
			lastX = x;
			lastY = y;

		}
		break;

	default:
		break;

	}

}

void motion(int x, int y)
{

	GLfloat dx = x - lastX;
	GLfloat dy = y - lastY;

	if (bLButtonDown) {
		// 旋转
		yRotation += dx * rotSpeed;
		xRotation += dy * rotSpeed;

	}
	if (bRButtonDown) {
		//转移
		if (dy > 0.0f) {

			dy = sqrt(dy);
			scale = lastScl * dy;

		}
		else if (dy < 0.0f) {

			dy = sqrt(-dy);
			scale = lastScl / dy;

		}

	}

	if (bMButtonDown) {
		// 缩放
		GLfloat ratio = translSpeed / (0.5f * height) / scale;
		xTransl += dx * ratio;
		yTransl -= dy * ratio;

	}

	glutPostRedisplay();

}

void RenderCube()
{

	glBegin(GL_QUADS);

	glColor3ub(255, 255, 0);
	glVertex3f(-cubic_size, cubic_size, cubic_size);
	glVertex3f(-cubic_size, -cubic_size, cubic_size);
	glVertex3f(cubic_size, -cubic_size, cubic_size);
	glVertex3f(cubic_size, cubic_size, cubic_size);

	glColor3ub(0, 255, 255);
	glVertex3f(cubic_size, cubic_size, cubic_size);
	glVertex3f(cubic_size, -cubic_size, cubic_size);
	glVertex3f(cubic_size, -cubic_size, -cubic_size);
	glVertex3f(cubic_size, cubic_size, -cubic_size);

	glColor3ub(255, 0, 255);
	glVertex3f(cubic_size, cubic_size, -cubic_size);
	glVertex3f(cubic_size, -cubic_size, -cubic_size);
	glVertex3f(-cubic_size, -cubic_size, -cubic_size);
	glVertex3f(-cubic_size, cubic_size, -cubic_size);

	glColor3ub(0, 0, 255);
	glVertex3f(-cubic_size, cubic_size, -cubic_size);
	glVertex3f(-cubic_size, -cubic_size, -cubic_size);
	glVertex3f(-cubic_size, -cubic_size, cubic_size);
	glVertex3f(-cubic_size, cubic_size, cubic_size);

	glColor3ub(0, 255, 0);
	glVertex3f(-cubic_size, -cubic_size, cubic_size);
	glVertex3f(-cubic_size, -cubic_size, -cubic_size);
	glVertex3f(cubic_size, -cubic_size, -cubic_size);
	glVertex3f(cubic_size, -cubic_size, cubic_size);

	glColor3ub(255, 0, 0);
	glVertex3f(cubic_size, cubic_size, cubic_size);
	glVertex3f(cubic_size, cubic_size, -cubic_size);
	glVertex3f(-cubic_size, cubic_size, -cubic_size);
	glVertex3f(-cubic_size, cubic_size, cubic_size);
	glEnd();

}

void display()
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	glMatrixMode(GL_MODELVIEW);

	glPushMatrix();
	glScalef(scale, scale, scale);
	glTranslatef(xTransl, yTransl, 0.0f);
	glRotatef(xRotation+45, 1.0f, 0.0f, 0.0f);
	glRotatef(yRotation+45, 0.0f, 1.0f, 0.0f);
	glCallList(icube);
	glPopMatrix();

	glFlush();

}

void reshape(int w, int h)
{

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, 0.0, 10.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);


	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	GLdouble aspect = (GLdouble)w / (GLdouble)h;
	gluPerspective(45.0*0.5, aspect, 0.1, 100.0);


	glViewport(0, 0, w, h);

	width = w;
	height = h;

}


int main(int argc, char *argv[])
{

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(width, height);
	glutCreateWindow(argv[0]);

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);

	init();

	glutMainLoop();
	destroy();

	return 0;

}