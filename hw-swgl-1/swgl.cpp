#include "swgl.h"
#include <cmath>
#include <stack>

GLdouble CTM_MV[16];	//Current Transformation Matrix: ModelView
GLdouble CTM_P[16];		//Current Transformation Matrix: Projection
GLdouble *CTM;			//Pointer to Current Transformation Matrix

int _viewport[4];
const GLdouble PI = 3.14159265;
struct Matrix
{
	GLdouble m[16];

	Matrix() { memset(m, 0, sizeof(m)); }
	Matrix(const GLdouble loadMatrix[16])
	{
		memcpy(m, loadMatrix, sizeof(GLdouble) * 16);
	}
};
std::stack<Matrix> MatrixStack;

void swTranslated(GLdouble x, GLdouble y, GLdouble z)
{
	GLdouble tmp[16];
	tmp[0] = 1.0;   tmp[4] = 0.0;   tmp[8] = 0.0;   tmp[12] = x;
	tmp[1] = 0.0;   tmp[5] = 1.0;   tmp[9] = 0.0;   tmp[13] = y;
	tmp[2] = 0.0;   tmp[6] = 0.0;   tmp[10] = 1.0;  tmp[14] = z;
	tmp[3] = 0.0;   tmp[7] = 0.0;   tmp[11] = 0.0;  tmp[15] = 1.0;

	swMultMatrixd(tmp);
}
void swScaled(GLdouble x, GLdouble y, GLdouble z)
{
	GLdouble tmp[16];
	tmp[0] = x;     tmp[4] = 0.0;   tmp[8] = 0.0;   tmp[12] = 0.0;
	tmp[1] = 0.0;   tmp[5] = y;     tmp[9] = 0.0;   tmp[13] = 0.0;
	tmp[2] = 0.0;   tmp[6] = 0.0;   tmp[10] = z;    tmp[14] = 0.0;
	tmp[3] = 0.0;   tmp[7] = 0.0;   tmp[11] = 0.0;  tmp[15] = 1.0;
	swMultMatrixd(tmp);
}
void swRotated(GLdouble angle, GLdouble x, GLdouble y, GLdouble z)
{
	const GLdouble cAng = cos(angle*PI / 180);
	const GLdouble cAngcmp = 1 - cAng;
	const GLdouble sAng = sin(angle*PI / 180);

	GLdouble tmp[16];
	tmp[0] = x * x*cAngcmp + cAng;    tmp[4] = x * y*cAngcmp - z * sAng;   tmp[8] = x * z*cAngcmp + y * sAng; tmp[12] = 0.0;
	tmp[1] = x * y*cAngcmp + z * sAng;    tmp[5] = y * y*cAngcmp + cAng;     tmp[9] = y * z*cAngcmp - x * sAng; tmp[13] = 0.0;
	tmp[2] = x * z*cAngcmp - y * sAng;    tmp[6] = y * z*cAngcmp + x * sAng;   tmp[10] = z * z*cAngcmp + cAng;  tmp[14] = 0.0;
	tmp[3] = 0.0;                   tmp[7] = 0.0;                  tmp[11] = 0.0;               tmp[15] = 1.0;

	swMultMatrixd(tmp);
}

void swMatrixMode(GLenum mode)
{
	if (mode == GL_PROJECTION)
	{
		CTM = CTM_P;
	}
	else if (mode == GL_MODELVIEW)
	{
		CTM = CTM_MV;
	}
}

void swLoadIdentity(void)
{
	CTM[0] = 1;   CTM[1] = 0;   CTM[2] = 0;   CTM[3] = 0;
	CTM[4] = 0;   CTM[5] = 1;   CTM[6] = 0;   CTM[7] = 0;
	CTM[8] = 0;   CTM[9] = 0;   CTM[10] = 1;  CTM[11] = 0;
	CTM[12] = 0;  CTM[13] = 0;  CTM[14] = 0;  CTM[15] = 1;
}

void swLoadMatrixd(const GLdouble * m)
{
	memcpy(CTM, m, sizeof(GLdouble) * 16);
}

void swMultMatrixd(const GLdouble * m)
{
	multMatrix(CTM, m);
}

void swPushMatrix(void)
{
	Matrix tmp = Matrix(CTM);
	MatrixStack.push(tmp);
}

void swPopMatrix(void)
{
	Matrix tmp = MatrixStack.top();
	MatrixStack.pop();
	swLoadMatrixd(tmp.m);
}

void swuLookAt(GLdouble eyeX, GLdouble eyeY, GLdouble eyeZ,
	GLdouble centerX, GLdouble centerY, GLdouble centerZ,
	GLdouble upX, GLdouble upY, GLdouble upZ)
{
	GLdouble F[3] = { centerX - eyeX, centerY - eyeY, centerZ - eyeZ };
	GLdouble Fr = sqrt(F[0] * F[0] + F[1] * F[1] + F[2] * F[2]);
	for (int i = 0; i < 3; ++i) F[i] /= Fr;

	GLdouble UP[3] = { upX, upY, upZ };
	GLdouble UPr = sqrt(UP[0] * UP[0] + UP[1] * UP[1] + UP[2] * UP[2]);
	for (int i = 0; i < 3; ++i) UP[i] /= UPr;
	/** side = F X UP **/
	GLdouble side[3] = { F[1] * UP[2] - F[2] * UP[1],
						F[2] * UP[0] - F[0] * UP[2],
						F[0] * UP[1] - F[1] * UP[0] };
	/** UP = side X F **/
	UP[0] = side[1] * F[2] - side[2] * F[1];
	UP[1] = side[2] * F[0] - side[0] * F[2];
	UP[2] = side[0] * F[1] - side[1] * F[0];
	/** set matrix **/
	GLdouble tmp[16];
	tmp[0] = side[0];   tmp[4] = side[1];   tmp[8] = side[2];   tmp[12] = 0.0;
	tmp[1] = UP[0];     tmp[5] = UP[1];     tmp[9] = UP[2];     tmp[13] = 0.0;
	tmp[2] = -F[0];     tmp[6] = -F[1];     tmp[10] = -F[2];    tmp[14] = 0.0;
	tmp[3] = 0.0;       tmp[7] = 0.0;       tmp[11] = 0.0;      tmp[15] = 1.0;

	swMultMatrixd(tmp);
	swTranslated(-eyeX, -eyeY, -eyeZ);
}

void swuPerspective(GLdouble fovy, GLdouble aspect, GLdouble zNear, GLdouble zFar)
{
	const GLdouble f = 1.0 / tan((fovy / 2)*PI / 180);
	GLdouble tmp[16];
	tmp[0] = f / aspect;    tmp[4] = 0.0;   tmp[8] = 0.0;                           tmp[12] = 0.0;
	tmp[1] = 0.0;           tmp[5] = f;     tmp[9] = 0.0;                           tmp[13] = 0.0;
	tmp[2] = 0.0;           tmp[6] = 0.0;   tmp[10] = (zFar + zNear) / (zNear - zFar);    tmp[14] = 2 * zFar*zNear / (zNear - zFar);
	tmp[3] = 0.0;           tmp[7] = 0.0;   tmp[11] = -1.0;                         tmp[15] = 0.0;

	swMultMatrixd(tmp);
}

void swViewport(GLint x, GLint y, GLsizei width, GLsizei height)
{
	_viewport[0] = x;
	_viewport[1] = y;
	_viewport[2] = width;
	_viewport[3] = height;
}

//h: input the vertex in world space(h)
//w: vertex in windows space(w)
bool swTransformation(const GLdouble h[4], GLdouble w[4])
{
	//p = CTM_P*CTM_MV*h
	GLdouble tmp[16], p[4];
	multMatrix(tmp, CTM_P, CTM_MV);

	p[0] = tmp[0] * h[0] + tmp[4] * h[1] + tmp[8] * h[2] + tmp[12] * h[3];
	p[1] = tmp[1] * h[0] + tmp[5] * h[1] + tmp[9] * h[2] + tmp[13] * h[3];
	p[2] = tmp[2] * h[0] + tmp[6] * h[1] + tmp[10] * h[2] + tmp[14] * h[3];
	p[3] = tmp[3] * h[0] + tmp[7] * h[1] + tmp[11] * h[2] + tmp[15] * h[3];

	//prespective division
	for (int i = 0; i < 4; i++) p[i] /= p[3];

	//viewport transformation
	w[0] = (p[0] + 1) * _viewport[2] / 2 + _viewport[0];
	w[1] = (p[1] + 1) * _viewport[3] / 2 + _viewport[1];
	w[2] = p[2];

	return true;
}

void multMatrix(GLdouble * r, const GLdouble * m, const GLdouble * n)
{
	r[0] = m[0] * n[0] + m[4] * n[1] + m[8] * n[2] + m[12] * n[3];
	r[1] = m[1] * n[0] + m[5] * n[1] + m[9] * n[2] + m[13] * n[3];
	r[2] = m[2] * n[0] + m[6] * n[1] + m[10] * n[2] + m[14] * n[3];
	r[3] = m[3] * n[0] + m[7] * n[1] + m[11] * n[2] + m[15] * n[3];

	r[4] = m[0] * n[4] + m[4] * n[5] + m[8] * n[6] + m[12] * n[7];
	r[5] = m[1] * n[4] + m[5] * n[5] + m[9] * n[6] + m[13] * n[7];
	r[6] = m[2] * n[4] + m[6] * n[5] + m[10] * n[6] + m[14] * n[7];
	r[7] = m[3] * n[4] + m[7] * n[5] + m[11] * n[6] + m[15] * n[7];

	r[8] = m[0] * n[8] + m[4] * n[9] + m[8] * n[10] + m[12] * n[11];
	r[9] = m[1] * n[8] + m[5] * n[9] + m[9] * n[10] + m[13] * n[11];
	r[10] = m[2] * n[8] + m[6] * n[9] + m[10] * n[10] + m[14] * n[11];
	r[11] = m[3] * n[8] + m[7] * n[9] + m[11] * n[10] + m[15] * n[11];

	r[12] = m[0] * n[12] + m[4] * n[13] + m[8] * n[14] + m[12] * n[15];
	r[13] = m[1] * n[12] + m[5] * n[13] + m[9] * n[14] + m[13] * n[15];
	r[14] = m[2] * n[12] + m[6] * n[13] + m[10] * n[14] + m[14] * n[15];
	r[15] = m[3] * n[12] + m[7] * n[13] + m[11] * n[14] + m[15] * n[15];
}

void multMatrix(GLdouble * m, const GLdouble * n)
{
	GLdouble t[16]; memcpy(t, m, sizeof(t));
	m[0] = t[0] * n[0] + t[4] * n[1] + t[8] * n[2] + t[12] * n[3];
	m[1] = t[1] * n[0] + t[5] * n[1] + t[9] * n[2] + t[13] * n[3];
	m[2] = t[2] * n[0] + t[6] * n[1] + t[10] * n[2] + t[14] * n[3];
	m[3] = t[3] * n[0] + t[7] * n[1] + t[11] * n[2] + t[15] * n[3];

	m[4] = t[0] * n[4] + t[4] * n[5] + t[8] * n[6] + t[12] * n[7];
	m[5] = t[1] * n[4] + t[5] * n[5] + t[9] * n[6] + t[13] * n[7];
	m[6] = t[2] * n[4] + t[6] * n[5] + t[10] * n[6] + t[14] * n[7];
	m[7] = t[3] * n[4] + t[7] * n[5] + t[11] * n[6] + t[15] * n[7];

	m[8] = t[0] * n[8] + t[4] * n[9] + t[8] * n[10] + t[12] * n[11];
	m[9] = t[1] * n[8] + t[5] * n[9] + t[9] * n[10] + t[13] * n[11];
	m[10] = t[2] * n[8] + t[6] * n[9] + t[10] * n[10] + t[14] * n[11];
	m[11] = t[3] * n[8] + t[7] * n[9] + t[11] * n[10] + t[15] * n[11];

	m[12] = t[0] * n[12] + t[4] * n[13] + t[8] * n[14] + t[12] * n[15];
	m[13] = t[1] * n[12] + t[5] * n[13] + t[9] * n[14] + t[13] * n[15];
	m[14] = t[2] * n[12] + t[6] * n[13] + t[10] * n[14] + t[14] * n[15];
	m[15] = t[3] * n[12] + t[7] * n[13] + t[11] * n[14] + t[15] * n[15];
}

/*
GLdouble* mutiMatrix(const GLdouble *m, const GLdouble *n)
{
	printf("%f\n", (sizeof(n) / sizeof(GLdouble)));
	for (int i = 0; i < 16; i++) {
		if (i % 4 == 0) printf("\n");
		printf("%f ", m[i]);
	}
	printf("n\n");
	for (int i = 0; i < 16; i++) {
		if (i % 4 == 0) printf("\n");
		printf("%f ", n[i]);
	}
	if ((sizeof(n) / sizeof(GLdouble)) == 16)
	{
		GLdouble result[16];
		printf("hellp\n");
		for (int i = 0; i < 16; i += 4)
			for (int j = 0; j < 4; j++)
			{
				result[i + j] = n[i] * m[j] + n[i + 1] * m[j + 4] + n[i + 2] * m[j + 8] + n[i + 3] * m[j + 12];
			}
		for(int i = 0 ; i < 4; i ++) printf("%f %f %f %f\n", result[4*i+0], result[4 * i + 1], result[4 * i + 2], result[4 * i + 3]);
		return result;
	}
	else if ((sizeof(n) / sizeof(GLdouble)) == 4)
	{
		GLdouble result[4];
		for (int i = 0; i < 4; i ++)
		{
			result[i] = m[i] * n[0] + m[i + 4] * n[1] + m[i + 8] * n[2] + m[i + 12] * n[3];
		}
		printf("%f %f %f %f\n", result[0], result[1], result[2], result[3]);
		return result;
	}
}
*/
