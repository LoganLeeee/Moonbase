#include <time.h>  
#include <math.h>  
#include <stdlib.h>  
#include <iostream>
#include "fractal.h"  
#include "glut.h"
using namespace std;

// height : ֵԽ�󣬵������Խ�� 
// times  : ��������������Խ�࣬tireԽ��
// step   : ����ʱһ������ĳ̶�
// D      : �����˥�����ӣ�Խ��������������ȡֵԽС
// seed   : ���������
fractal::fractal(int _height, int _times, float _step, int _D, int seed)
{
	srand(seed);
	step = _step;
	times = _times;
	height = _height;
	D = _D;
	n = pow(2, times) + 1;
	indiceNum = 6 * (n - 1)*(n - 1);
	vertex = new float[3 * n*n];
	indice = new int[indiceNum];
	data = new float*[n];
	for (int i = 0; i < n; i++) {
		data[i] = new float[n];
		for (int j = 0; j < n; j++) {
			data[i][j] = 0;
		}
	}
}

// ���ɶ�����������
void fractal::genIndice()
{
	for (int i = 0; i < n - 1; i++) {
		for (int j = 0; j < n - 1; j++) {
			indice[3 * ((n - 1) * i + j)] = (n * i + j);
			indice[3 * ((n - 1) * i + j) + 1] = (n * i + j + 1);
			indice[3 * ((n - 1) * i + j) + 2] = (n * (i + 1) + j + 1);

		}
	}
	cout << endl;
	int off = 3 * (n - 1)*(n - 1);
	for (int i = 0; i < n - 1; i++) {
		for (int j = 0; j < n - 1; j++) {
			indice[off + 3 * ((n - 1) * i + j)] = (n * i + j);
			indice[off + 3 * ((n - 1) * i + j) + 1] = (n * (i + 1) + j);
			indice[off + 3 * ((n - 1)* i + j) + 2] = (n * (i + 1) + j + 1);

		}
	}
}

// ����[-num,num]֮��������
static float randnum(float num)
{
	float max = num;
	float min = -num;
	int r;
	float	x;

	r = rand();
	x = (float)(r & 0x7fff) /
		(float)0x7fff;
	return (x * (max - min) + min);
}

// ���㶥��߶�
void fractal::calculate()
{
	int size = n - 1;
	int number = 1;
	int ratio = pow(2, D);
	roughness = height;

	//timesΪ��������
	for (int t = 0; t < times; t++) {
		// diamand�׶�
		for (int i = 0; i < number; i++) {
			for (int j = 0; j < number; j++) {
				float r = randnum(.5) * roughness;
				int center_x = (size >> 1) + size * i;
				int center_y = (size >> 1) + size * j;
				data[center_x][center_y] =
					(data[size * i][size * j]
						+ data[size*i + size][size * j]
						+ data[size * i][size * j + size]
						+ data[size * i + size][size * j + size]) / 4 + r;
			}
		}

		// square�׶�
		int pointNum = ((t + 1) << 1) + 1;
		int pointStep = (n - 1) / (pointNum - 1);
		for (int i = 0; i < pointNum; i++) {
			for (int j = 0; j < pointNum; j++) {
				if ((i + j) % 2 == 1) {
					float r = randnum(.5) * roughness;
					if (i == 0) {
						data[i*pointStep][j*pointStep] =
							(data[n - pointStep][j*pointStep] +
								data[(i + 1)*pointStep][j*pointStep] +
								data[i*pointStep][(j + 1)*pointStep] +
								data[i*pointStep][(j - 1)*pointStep]) / 4 + r;
					}
					else if (j == 0) {
						data[i*pointStep][j*pointStep] =
							(data[(i - 1)*pointStep][j*pointStep] +
								data[(i + 1)*pointStep][j*pointStep] +
								data[i*pointStep][(j + 1)*pointStep] +
								data[i*pointStep][n - pointStep]) / 4 + r;
					}
					else if (i == pointNum - 1) {
						data[i*pointStep][j*pointStep] =
							(data[pointStep][j*pointStep] +
								data[(i - 1)*pointStep][j*pointStep] +
								data[i*pointStep][(j + 1)*pointStep] +
								data[i*pointStep][(j - 1)*pointStep]) / 4 + r;
					}
					else if (j == pointNum - 1) {
						data[i*pointStep][j*pointStep] =
							(data[(i - 1)*pointStep][j*pointStep] +
								data[(i + 1)*pointStep][j*pointStep] +
								data[i*pointStep][pointStep] +
								data[i*pointStep][(j - 1)*pointStep]) / 4 + r;
					}
					else {
						data[i*pointStep][j*pointStep] =
							(data[(i - 1)*pointStep][j*pointStep] +
								data[(i + 1)*pointStep][j*pointStep] +
								data[i*pointStep][(j + 1)*pointStep] +
								data[i*pointStep][(j - 1)*pointStep]) / 4 + r;
					}
				}
			}
		}
		roughness = roughness / ratio;
		size >>= 1;
		number <<= 1;
	}

	// ��dataӳ�䵽vertex�����ϣ���ɾ��data,ͬʱ��������߶Ⱥ���С�߶�
	minHeight = 10000;
	maxHeight = -10000;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			vertex[3 * (i*n + j)] = i * step - n * step / 2;
			vertex[3 * (i*n + j) + 1] = data[i][j];
			vertex[3 * (i*n + j) + 2] = j * step - n * step / 2;
			if (maxHeight < data[i][j]) {
				maxHeight = data[i][j];
			}
			if (minHeight > data[i][j]) {
				minHeight = data[i][j];
			}
		}
		delete[] data[i];
	}
	delete[] data;

	// ��������
	genIndice();
}


// ���ݸ߶�ѡ����ɫ
void fractal::chooseColor(float height)
{
	const GLfloat blue[] = { 1.0 * 65 / 255, 1.0 * 127 / 255, 1.0 * 219 / 255 };
	const GLfloat green[] = { 1.0 * 73 / 255, 1.0 * 161 / 255, 1.0 * 101 / 255 };
	const GLfloat yellow[] = { 1.0 * 172 / 255, 1.0 * 189 / 255, 1.0 * 117 / 255 };
	const GLfloat brown[] = { 1.0 * 153 / 255, 1.0 * 123 / 255, 1.0 * 46 / 255 };

	float interval = maxHeight - minHeight;
	if (height < minHeight + interval / 4) {
		glColor3fv(blue);
	}
	else if (height < minHeight + interval / 2) {
		glColor3fv(green);
	}
	else if (height < minHeight + 3 * interval / 4) {
		glColor3fv(yellow);
	}
	else if (height < maxHeight) {
		glColor3fv(brown);
	}
}

void fractal::draw()
{
	glPushMatrix();
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < indiceNum / 3; i++) {
		chooseColor(vertex[3 * indice[3 * i] + 1]);
		glVertex3f(vertex[3 * indice[3 * i]], vertex[3 * indice[3 * i] + 1], vertex[3 * indice[3 * i] + 2]);
		chooseColor(vertex[3 * indice[3 * i + 1] + 1]);
		glVertex3f(vertex[3 * indice[3 * i + 1]], vertex[3 * indice[3 * i + 1] + 1], vertex[3 * indice[3 * i + 1] + 2]);
		chooseColor(vertex[3 * indice[3 * i + 2] + 1]);
		glVertex3f(vertex[3 * indice[3 * i + 2]], vertex[3 * indice[3 * i + 2] + 1], vertex[3 * indice[3 * i + 2] + 2]);
	}
	glColor3f(1, 1, 1);
	glEnd();
	glPopMatrix();
}