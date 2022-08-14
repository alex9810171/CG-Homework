#include "swgl.h"
#include <algorithm>

using namespace std;
//---------------------------------------------------------------------------
//cghw2
//---------------------------------------------------------------------------

int pixel_table[800][400];
GLdouble Zbuffer[800][400];
bool ZbufferState[800][400];
void initTable();
bool virtual_BresenhamLine(GLdouble x1, GLdouble y1, GLdouble z1, GLdouble x2, GLdouble y2, GLdouble z2);

//You can change it to speed up
void writepixel(int x, int y, GLdouble r, GLdouble g, GLdouble b)
{
	GLubyte map[1]={255};

	glColor3d(r, g, b);
	glWindowPos2i(x, y);
	glBitmap(1, 1, 0, 0, 0, 0, map);
}

bool BresenhamLine(GLdouble x1, GLdouble y1, GLdouble z1, GLdouble x2, GLdouble y2, GLdouble z2, GLdouble r, GLdouble g, GLdouble b)
{
	bool steep = abs(y2 - y1) > abs(x2 - x1);
	if (steep) {
		swap(x1, y1);
		swap(x2, y2);
	}
	if (x1 > x2) {
		swap(x1, x2);
		swap(y1, y2);
	}
	int delta_x = x2 - x1;
	int delta_y = abs(y2 - y1);
	int error = delta_x / 2;
	int y_step;
	int y = y1;
	if (y1 < y2) y_step = 1;
	else y_step = -1;
	for (int x = x1; x < x2; x++) {
		if (steep) {
			GLdouble z = (y - y1) * (z2 - z1) / (y2 - y1) + z1;
			if (z > Zbuffer[y][x] || ZbufferState[y][x] == false) {
				writepixel(y, x, r, g, b);
				Zbuffer[y][x] = z;
				ZbufferState[y][x] = true;
			}
		}
		else {
			GLdouble z = (x - x1) * (z2 - z1) / (x2 - x1) + z1;
			if (z > Zbuffer[x][y] || ZbufferState[x][y] == false) {
				writepixel(x, y, r, g, b);
				Zbuffer[x][y] = z;
				ZbufferState[x][y] = true;
				writepixel(x, y, r, g, b);
			}
		}
		error -= delta_y;
		if (error < 0) {
			y += y_step;
			error += delta_x;
		}
	}
	return true;
}

bool swTriangle(GLdouble x1, GLdouble y1, GLdouble z1,
			 GLdouble x2, GLdouble y2, GLdouble z2,
			 GLdouble x3, GLdouble y3, GLdouble z3,
			 GLdouble r, GLdouble g, GLdouble b)
{
	initTable();
	virtual_BresenhamLine(x1, y1, z1, x2, y2, z2);
	virtual_BresenhamLine(x2, y2, z2, x3, y3, z3);
	virtual_BresenhamLine(x3, y3, z3, x1, y1, z1);
	pixel_table[((int)x1)][((int)y1)] = 1;
	pixel_table[((int)x2)][((int)y2)] = 1;
	pixel_table[((int)x3)][((int)y3)] = 1;

	for (int y = 0; y < 400; y++) {
		int begin = -1;
		int end = -1;
		for (int x = 0; x < 800; x++)
			if (pixel_table[x][y] == 1) {
				if (begin == -1) {
					begin = x;
					end = x;
				}
				else end = x;
			}
		if(begin != -1)	BresenhamLine((GLdouble)begin, (GLdouble)y, z1, (GLdouble)end, (GLdouble)y, z2, r, g, b);
	}
	return true;
}


bool swInitZbuffer(int width, int height)
{
	swClearZbuffer();
    return true;
}



bool swClearZbuffer()
{
	for (int i = 0; i < 800; i++) for (int j = 0; j < 400; j++) {
		Zbuffer[i][j] = 0;
		ZbufferState[i][j] = false;
	}
    return true;
}

void initTable() {
	for (int i = 0; i < 800; i++) for (int j = 0; j < 400; j++) pixel_table[i][j] = 0;
}

bool virtual_BresenhamLine(GLdouble x1, GLdouble y1, GLdouble z1, GLdouble x2, GLdouble y2, GLdouble z2)
{
	bool steep = abs(y2 - y1) > abs(x2 - x1);
	if (steep) {
		swap(x1, y1);
		swap(x2, y2);
	}
	if (x1 > x2) {
		swap(x1, x2);
		swap(y1, y2);
	}
	int delta_x = x2 - x1;
	int delta_y = abs(y2 - y1);
	int error = delta_x / 2;
	int y_step;
	int y = y1;
	if (y1 < y2) y_step = 1;
	else y_step = -1;
	for (int x = x1; x < x2; x++) {
		if (steep) pixel_table[y][x] = 1;
		else pixel_table[x][y] = 1;
		error -= delta_y;
		if (error < 0) {
			y += y_step;
			error += delta_x;
		}
	}
	return true;
}
