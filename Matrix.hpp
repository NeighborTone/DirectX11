#pragma once
#include <math.h>
class Matrix
{
private:

	float _11, _12, _13, _14;
	float _21, _22, _23, _24;
	float _31, _32, _33, _34;
	float _41, _42, _43, _44;
public:
	Matrix() {};
	Matrix(const Matrix&  m){
		_11 = m._11;  _12 = m._12;	_13 = m._13;	  _14 = m._14;
		_21 = m._21;  _22 = m._22;	_23 = m._23;	  _24 = m._24;
		_31 = m._31;  _32 = m._32;	_33 = m._33;	  _34 = m._34;
		_41 = m._41;  _42 = m._42;	_43 = m._43;	  _44 = m._44;
	}

	~Matrix() {};

	Matrix& Matrix::operator=(const Matrix& m){
		_11 = m._11;  _12 = m._12;	_13 = m._13;	  _14 = m._14;
		_21 = m._21;  _22 = m._22;	_23 = m._23;	  _24 = m._24;
		_31 = m._31;  _32 = m._32;	_33 = m._33;	  _34 = m._34;
		_41 = m._41;  _42 = m._42;	_43 = m._43;	  _44 = m._44;

		return *this;
	}

	void MatrixIdentity(Matrix *mat) {
		mat->_11 = 1.0f; mat->_12 = 0.0f; mat->_13 = 0.0f; mat->_14 = 0.0f;
		mat->_21 = 0.0f; mat->_22 = 1.0f; mat->_23 = 0.0f; mat->_24 = 0.0f;
		mat->_31 = 0.0f; mat->_32 = 0.0f; mat->_33 = 1.0f; mat->_34 = 0.0f;
		mat->_41 = 0.0f; mat->_42 = 0.0f; mat->_43 = 0.0f; mat->_44 = 1.0f;
	}

	void MatrixScaling(Matrix *mat, float sizex, float sizey, float sizez) {
		mat->_11 = sizex; mat->_12 = 0.0f; mat->_13 = 0.0f; mat->_14 = 0.0f;
		mat->_21 = 0.0f; mat->_22 = sizey; mat->_23 = 0.0f; mat->_24 = 0.0f;
		mat->_31 = 0.0f; mat->_32 = 0.0f; mat->_33 = sizez; mat->_34 = 0.0f;
		mat->_41 = 0.0f; mat->_42 = 0.0f; mat->_43 = 0.0f; mat->_44 = 1.0f;
	}

	void MatrixRotationZ(Matrix *mat, float theta) {
		float the = theta * 3.14f / 180.0f;
		mat->_11 = cosf(the); mat->_12 = sinf(the); mat->_13 = 0.0f; mat->_14 = 0.0f;
		mat->_21 = -sinf(the); mat->_22 = cosf(the); mat->_23 = 0.0f; mat->_24 = 0.0f;
		mat->_31 = 0.0f; mat->_32 = 0.0f; mat->_33 = 1.0f; mat->_34 = 0.0f;
		mat->_41 = 0.0f; mat->_42 = 0.0f; mat->_43 = 0.0f; mat->_44 = 1.0f;
	}

	void MatrixTranslation(Matrix *mat, float movx, float movy, float movz) {
		mat->_11 = 1.0f; mat->_12 = 0.0f; mat->_13 = 0.0f; mat->_14 = 0.0f;
		mat->_21 = 0.0f; mat->_22 = 1.0f; mat->_23 = 0.0f; mat->_24 = 0.0f;
		mat->_31 = 0.0f; mat->_32 = 0.0f; mat->_33 = 1.0f; mat->_34 = 0.0f;
		mat->_41 = movx; mat->_42 = movy; mat->_43 = movz; mat->_44 = 1.0f;
	}

	void MatrixMultiply(Matrix *mat, Matrix *mat1, Matrix *mat2) {
		mat->_11 = mat1->_11 * mat2->_11 + mat1->_12 * mat2->_21 + mat1->_13 * mat2->_31 + mat1->_14 * mat2->_41;
		mat->_12 = mat1->_11 * mat2->_12 + mat1->_12 * mat2->_22 + mat1->_13 * mat2->_32 + mat1->_14 * mat2->_42;
		mat->_13 = mat1->_11 * mat2->_13 + mat1->_12 * mat2->_23 + mat1->_13 * mat2->_33 + mat1->_14 * mat2->_43;
		mat->_14 = mat1->_11 * mat2->_14 + mat1->_12 * mat2->_24 + mat1->_13 * mat2->_34 + mat1->_14 * mat2->_44;

		mat->_21 = mat1->_21 * mat2->_11 + mat1->_22 * mat2->_21 + mat1->_23 * mat2->_31 + mat1->_24 * mat2->_41;
		mat->_22 = mat1->_21 * mat2->_12 + mat1->_22 * mat2->_22 + mat1->_23 * mat2->_32 + mat1->_24 * mat2->_42;
		mat->_23 = mat1->_21 * mat2->_13 + mat1->_22 * mat2->_23 + mat1->_23 * mat2->_33 + mat1->_24 * mat2->_43;
		mat->_24 = mat1->_21 * mat2->_14 + mat1->_22 * mat2->_24 + mat1->_23 * mat2->_34 + mat1->_24 * mat2->_44;

		mat->_31 = mat1->_31 * mat2->_11 + mat1->_32 * mat2->_21 + mat1->_33 * mat2->_31 + mat1->_34 * mat2->_41;
		mat->_32 = mat1->_31 * mat2->_12 + mat1->_32 * mat2->_22 + mat1->_33 * mat2->_32 + mat1->_34 * mat2->_42;
		mat->_33 = mat1->_31 * mat2->_13 + mat1->_32 * mat2->_23 + mat1->_33 * mat2->_33 + mat1->_34 * mat2->_43;
		mat->_34 = mat1->_31 * mat2->_14 + mat1->_32 * mat2->_24 + mat1->_33 * mat2->_34 + mat1->_34 * mat2->_44;

		mat->_41 = mat1->_41 * mat2->_11 + mat1->_42 * mat2->_21 + mat1->_43 * mat2->_31 + mat1->_44 * mat2->_41;
		mat->_42 = mat1->_41 * mat2->_12 + mat1->_42 * mat2->_22 + mat1->_43 * mat2->_32 + mat1->_44 * mat2->_42;
		mat->_43 = mat1->_41 * mat2->_13 + mat1->_42 * mat2->_23 + mat1->_43 * mat2->_33 + mat1->_44 * mat2->_43;
		mat->_44 = mat1->_41 * mat2->_14 + mat1->_42 * mat2->_24 + mat1->_43 * mat2->_34 + mat1->_44 * mat2->_44;
	}

	void swap(float *a, float *b) {
		float tmp;
		tmp = *a;
		*a = *b;
		*b = tmp;
	}

	void MatrixTranspose(Matrix *mat) {
		swap(&mat->_12, &mat->_21);
		swap(&mat->_13, &mat->_31);
		swap(&mat->_14, &mat->_41);
		swap(&mat->_23, &mat->_32);
		swap(&mat->_24, &mat->_42);
		swap(&mat->_34, &mat->_43);
	}

	void MatrixLookAtLH(Matrix *mat, float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3) {
		//z軸
		float zx = x2 - x1;
		float zy = y2 - y1;
		float zz = z2 - z1;
		//正規化
		float zzz = sqrtf(zx * zx + zy * zy + zz * zz);
		if (zzz != 0.0f) {
			zx = zx / zzz;
			zy = zy / zzz;
			zz = zz / zzz;
		}

		//x軸(外積)
		float xx = y3 * zz - z3 * zy;
		float xy = z3 * zx - x3 * zz;
		float xz = x3 * zy - y3 * zx;
		float xxx = sqrtf(xx * xx + xy * xy + xz * xz);
		if (xxx != 0.0f) {
			xx = xx / xxx;
			xy = xy / xxx;
			xz = xz / xxx;
		}

		//y軸(外積)
		float yx = zy * xz - zz * xy;
		float yy = zz * xx - zx * xz;
		float yz = zx * xy - zy * xx;

		//平行移動(内積)
		float mx = -(x1 * xx + y1 * xy + z1 * xz);
		float my = -(x1 * yx + y1 * yy + z1 * yz);
		float mz = -(x1 * zx + y1 * zy + z1 * zz);

		mat->_11 = xx; mat->_12 = yx; mat->_13 = zx; mat->_14 = 0.0f;
		mat->_21 = xy; mat->_22 = yy; mat->_23 = zy; mat->_24 = 0.0f;
		mat->_31 = xz; mat->_32 = yz; mat->_33 = zz; mat->_34 = 0.0f;
		mat->_41 = mx; mat->_42 = my; mat->_43 = mz; mat->_44 = 1.0f;
	}

	void MatrixPerspectiveFovLH(Matrix *mat, float theta, float aspect, float Near, float Far) {
		float the = theta * 3.14f / 180.0f;
		//透視変換後y方向スケーリング
		float sy = 1.0f / (tanf(the / 2.0f));
		//x方向スケーリング
		float sx = sy / aspect;
		//z方向スケーリング
		float sz = Far / (Far - Near);

		mat->_11 = sx; mat->_12 = 0.0f; mat->_13 = 0.0f; mat->_14 = 0.0f;
		mat->_21 = 0.0f; mat->_22 = sy; mat->_23 = 0.0f; mat->_24 = 0.0f;
		mat->_31 = 0.0f; mat->_32 = 0.0f; mat->_33 = sz; mat->_34 = 1.0f;
		mat->_41 = 0.0f; mat->_42 = 0.0f; mat->_43 = -(sz * Near); mat->_44 = 0.0f;
	}
};

