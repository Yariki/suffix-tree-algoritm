#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <time.h>
#include <vector>
#include <cmath>

using namespace std;
using std::string;
using std::vector;

#define DELTA (1e-6) 

#define PI (3.141592653589793)

#define SAFE_DELETE(o) if(o != NULL){ delete o;}

struct coordinate
{
	float x1,y1;
	float x2,y2;
	float x3,y3;
	float min;
};

bool read_file(coordinate& coord)
{
	printf("The input data read from 'triangle.in'.\n");
	ifstream in("triangle.in");
	if(!in.is_open())
	{
		printf("Error while opening file.\n");
		system("pause");
		return false;
	}
	string buf;
	
	std::getline(in,buf);
	vector<float> listCoord;
	char* pchr = strtok((char*)buf.c_str()," ");
	while(pchr != NULL)
	{
		listCoord.push_back(atof(pchr));
		pchr = strtok(NULL," ");
	}

	coord.x1 = listCoord.at(0);
	coord.y1 = listCoord.at(1);
	coord.x2 = listCoord.at(2);
	coord.y2 = listCoord.at(3);
	coord.x3 = listCoord.at(4);
	coord.y3 = listCoord.at(5);
	in.close();

	return true;
}

float max(float a, float b)
{
	return a > b ? a : b;
}

float min(float a, float b)
{
	return a < b ? a : b;
}

void expand_angle(float& a, float x, float y)
{
	if(x < 0)
		a = a + PI;
}


void find_qxy(float a, float x1, float x2, float y1, float y2, float& q, float& x, float& y)
{
	float _x1,_y1, _x2,_y2;
	_x1 = x1*cos(a)-y1*sin(a);
	_y1 = y1*cos(a)+x1*sin(a);
	_x2 = x2*cos(a)-y2*sin(a);
	_y2 = y2*cos(a)+x2*sin(a);
	x = max(max(0,_x1),_x2)-min(min(0,_x1),_x2);
	y = max(max(0,_y1),_y2)-min(min(0,_y1),_y2);
	q = max(x,y);
}

void find_min_lenght(coordinate& coord)
{
	int k;
	float sc_product;
	float _x1, _x2, _x3;
	float _y1, _y2, _y3;
	float a1,a2,minq,la,ha,xa,lq,hq,xq,lx,hx,xx,ly,hy,xy,direction;
	minq = 1e15;
	for(k = 1; k <= 3;k++)
	{
		switch(k)
		{
		case 1:
			_x1 = coord.x2-coord.x1;
			_x2 = coord.x3-coord.x1;
			_y1 = coord.y2-coord.y1; 
			_y2 = coord.y3-coord.y1;
			break;
		case 2:
			_x1 = coord.x1-coord.x2; 
			_x2 = coord.x3-coord.x2;
			_y1 = coord.y1-coord.y2; 
			_y2 = coord.y3-coord.y2;
			break;
		case 3:
			_x1 = coord.x1-coord.x3; 
			_x2 = coord.x2-coord.x3;
			_y1 = coord.y1-coord.y3; 
			_y2 = coord.y2-coord.y3;
			break;
		}
		sc_product =_x1*_x2+_y1*_y2;

		if (sc_product < 0 )
			continue;

		if (abs(_x1)<abs(_y1))
		{
			a1 = atan(_x1/_y1);
			if(a1 < 0) 
				a1 = - PI / 2 - a1;
			else 
			 a1 = PI / 2 - a1;
		}
		else
			a1 = atan(_y1/_x1);

		if (abs(_x2)<abs(_y2)) 
		{
			a2 = atan(_x2/_y2);
			if (a2<0) 
				a2 = - PI/2-a2;
			else 
				a2 = PI/2-a2;
		}
		else
			a2 = atan(_y2/_x2);

		expand_angle(a1,_x1,_y1);
		expand_angle(a2,_x2,_y2);

		if(max(a1,a2) - min(a1,a2) >= PI)
		{
			if(a1 < a2)
			{
				a1 = a1 + 2 * PI;
			}
			else
			{
				a2 = a2 + 2 * PI;
			}
		}
		la = min(a1,a2);
		ha = max(a1,a2);
		ha = -la + PI / 2 - (ha-la);
		la = -la;
		find_qxy(la,_x1,_x2,_y1,_y2,lq,lx,ly);
		find_qxy(ha,_x1,_x2,_y1,_y2,hq,hx,hy);
		if (hq<minq) 
			minq = hq;
		if (lq<minq) 
			minq = lq;
		xq = lq;

		if ((lx-ly)<(hx-hy))
			direction = 1;
		else 
			direction = -1;

		if ((lx-hx)*(ly-hy)>0) 
			continue;
		if ((lx-ly)*(hx-hy)>0) 
			continue;

		while(abs( lx - ly) > DELTA)
		{
			xa = (la+ha)/2;
			find_qxy(xa,_x1,_x2,_y1,_y2,xq,xx,xy);
			if ((xx-xy)*direction<0) 
				la = xa;
			else 
				ha = xa;
			find_qxy(la,_x1,_x2,_y1,_y2,lq,lx,ly);
			find_qxy(ha,_x1,_x2,_y1,_y2,hq,hx,hy);
		}
		if (xq<minq) 
			minq = xq;
	}
	coord.min = minq;
}

void main()
{

	coordinate coord;
	
	if(read_file(coord))
	{
		coord.x1 = coord.x1 - coord.x3;
		coord.y1 = coord.y1 - coord.y3;
		coord.x2 = coord.x2 - coord.x3;
		coord.y2 = coord.y2 - coord.y3;
		coord.x3 = coord.y3 = 0;
		find_min_lenght(coord);
		cout << "Min lenght = " << coord.min << "\n";
	}
	
	system("pause");
}
