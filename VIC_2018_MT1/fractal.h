#pragma once

class fractal
{
private:
	enum { MAX = 10000 };
	float roughness;
	int times;
	int height;
	float minHeight;
	float maxHeight;
	int n;
	int D;
	int indiceNum;
	float** data;
	float* vertex;
	int* indice;
	float step;
	void genIndice();
	void chooseColor(float height);
public:
	void draw();
	void calculate();
	fractal(int _height, int _times, float _step, int D, int seed);
};
