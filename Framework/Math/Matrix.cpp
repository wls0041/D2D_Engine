#include "Framework.h"
#include "Matrix.h"
#include "Vector3.h"

const Matrix Matrix::Identity
(
	1, 0, 0, 0,
	0, 1, 0, 0,
	0, 0, 1, 0,
	0, 0, 0, 1
);

const Matrix Matrix::Scaling(const float & x, const float & y, const float & z)
{
	return Matrix
	(
		x, 0, 0, 0,
		0, y, 0, 0,
		0, 0, z, 0,
		0, 0, 0, 1
	);
}

const Matrix Matrix::Scaling(const float & s)
{
	return Scaling(s, s, s);
}

const Matrix Matrix::Scaling(const Vector3 & s)
{
	return Scaling(s.x, s.y, s.z);
}

const Matrix Matrix::RotationX(const float & radian)
{
	float s = sinf(radian);
	float c = cosf(radian);

	return Matrix
	(
		1, 0, 0, 0,
		0, c, s, 0,
		0, -s, c, 0,
		0, 0, 0, 1
	);
}

const Matrix Matrix::RotationY(const float & radian)
{
	float s = sinf(radian);
	float c = cosf(radian);

	return Matrix
	(
		c, 0, -s, 0,
		0, 1, 0, 0,
		s, 0, c, 0,
		0, 0, 0, 1
	);
}

const Matrix Matrix::RotationZ(const float & radian)
{
	float s = sinf(radian);
	float c = cosf(radian);

	return Matrix
	(
		c, s, 0, 0,
		-s, c, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	);
}

const Matrix Matrix::RotationYawPitchRoll(const float & yaw, const float & pitch, const float & roll)
{
	float sa = sinf(roll), sb = sinf(pitch), sc = sinf(yaw);
	float ca = cosf(roll), cb = cosf(pitch), cc = cosf(yaw);

	return Matrix
	(
		(ca * cc + sa * sb * sc), (-sa * cc + ca * sb * sc), (cb * sc), 0,
		(sa * cb), (ca * cb), -sb, 0.0f,
		(-ca * sc + sa * sb * cc), (sa * sc + ca * sb * cc), (cb * cc), 0,
		0, 0, 0, 1
	);
}

const Matrix Matrix::RotationYawPitchRoll(const Vector3 & rotation)
{
	return RotationYawPitchRoll(rotation.y, rotation.x, rotation.z);
}

const Matrix Matrix::Translation(const float & x, const float & y, const float & z)
{
	return Matrix
	(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		x, y, z, 1
	);
}

const Matrix Matrix::Translation(const float & p)
{
	return Translation(p, p, p);
}

const Matrix Matrix::Translation(const Vector3 & p)
{
	return Translation(p.x, p.y, p.z);
}

const Matrix Matrix::LookAtLH(const Vector3 & eye, const Vector3 & target, const Vector3 & up)
{
	Vector3 zAxis = (target - eye).Normalize();
	Vector3 xAxis = Vector3::Cross(up, zAxis).Normalize();
	Vector3 yAxis = Vector3::Cross(zAxis, xAxis).Normalize();

	return Matrix
	(
		xAxis.x, yAxis.x, zAxis.x, 0,
		xAxis.y, yAxis.y, zAxis.y, 0,
		xAxis.z, yAxis.z, zAxis.z, 0,
		-xAxis.Dot(eye), -yAxis.Dot(eye), -zAxis.Dot(eye), 1
	);
}

const Matrix Matrix::OrthoLH(const float & w, const float & h, const float & zn, const float & zf)
{
	return Matrix
	(
		2 / w, 0, 0, 0,
		0, 2 / h, 0, 0,
		0, 0, 1 / (zf - zn), 0,
		0, 0, zn / (zn - zf), 1
	);
}

const Matrix Matrix::OrthoOffCenterLH(const float & l, const float & r, const float & b, const float & t, const float & zn, const float & zf)
{
	return Matrix
	(
		2 / (r - 1), 0, 0, 0,
		0, 2 / (t - b), 0, 0,
		0, 0, 1 / (zf - zn), 0,
		(l + r) / (l - r), (t + b) / (b - t), zn / (zn - zf), 1
	);
}

Matrix Matrix::Inverse(const Matrix & rhs)
{
	float v0 = rhs._31 * rhs._42 - rhs._32 * rhs._41;
	float v1 = rhs._31 * rhs._43 - rhs._33 * rhs._41;
	float v2 = rhs._31 * rhs._44 - rhs._34 * rhs._41;
	float v3 = rhs._32 * rhs._43 - rhs._33 * rhs._42;
	float v4 = rhs._32 * rhs._44 - rhs._34 * rhs._42;
	float v5 = rhs._33 * rhs._44 - rhs._34 * rhs._43;

	float i11 = (v5 * rhs._22 - v4 * rhs._23 + v3 * rhs._24);
	float i21 = -(v5 * rhs._21 - v2 * rhs._23 + v1 * rhs._24);
	float i31 = (v4 * rhs._21 - v2 * rhs._22 + v0 * rhs._24);
	float i41 = -(v3 * rhs._21 - v1 * rhs._22 + v0 * rhs._23);

	float invDet = 1.0f / (i11 * rhs._11 + i21 * rhs._12 + i31 * rhs._13 + i41 * rhs._14);

	i11 *= invDet;
	i21 *= invDet;
	i31 *= invDet;
	i41 *= invDet;

	float i12 = -(v5 * rhs._12 - v4 * rhs._13 + v3 * rhs._14) * invDet;
	float i22 = (v5 * rhs._11 - v2 * rhs._13 + v1 * rhs._14) * invDet;
	float i32 = -(v4 * rhs._11 - v2 * rhs._12 + v0 * rhs._14) * invDet;
	float i42 = (v3 * rhs._11 - v1 * rhs._12 + v0 * rhs._13) * invDet;

	v0 = rhs._21 * rhs._42 - rhs._22 * rhs._41;
	v1 = rhs._21 * rhs._43 - rhs._23 * rhs._41;
	v2 = rhs._21 * rhs._44 - rhs._24 * rhs._41;
	v3 = rhs._22 * rhs._43 - rhs._23 * rhs._42;
	v4 = rhs._22 * rhs._44 - rhs._24 * rhs._42;
	v5 = rhs._23 * rhs._44 - rhs._24 * rhs._43;

	float i13 = (v5 * rhs._12 - v4 * rhs._13 + v3 * rhs._14) * invDet;
	float i23 = -(v5 * rhs._11 - v2 * rhs._13 + v1 * rhs._14) * invDet;
	float i33 = (v4 * rhs._11 - v2 * rhs._12 + v0 * rhs._14) * invDet;
	float i43 = -(v3 * rhs._11 - v1 * rhs._12 + v0 * rhs._13) * invDet;

	v0 = rhs._32 * rhs._21 - rhs._31 * rhs._22;
	v1 = rhs._33 * rhs._21 - rhs._31 * rhs._23;
	v2 = rhs._34 * rhs._21 - rhs._31 * rhs._24;
	v3 = rhs._33 * rhs._22 - rhs._32 * rhs._23;
	v4 = rhs._34 * rhs._22 - rhs._32 * rhs._24;
	v5 = rhs._34 * rhs._23 - rhs._33 * rhs._24;

	float i14 = -(v5 * rhs._12 - v4 * rhs._13 + v3 * rhs._14) * invDet;
	float i24 = (v5 * rhs._11 - v2 * rhs._13 + v1 * rhs._14) * invDet;
	float i34 = -(v4 * rhs._11 - v2 * rhs._12 + v0 * rhs._14) * invDet;
	float i44 = (v3 * rhs._11 - v1 * rhs._12 + v0 * rhs._13) * invDet;

	return Matrix
	(
		i11, i12, i13, i14,
		i21, i22, i23, i24,
		i31, i32, i33, i34,
		i41, i42, i43, i44
	);
}

Matrix Matrix::Transpose(const Matrix & rhs)
{
	return Matrix
	(
		rhs._11, rhs._21, rhs._31, rhs._41,
		rhs._12, rhs._22, rhs._32, rhs._42,
		rhs._13, rhs._23, rhs._33, rhs._43,
		rhs._14, rhs._24, rhs._34, rhs._44
	);
}

Matrix::Matrix()
{
	SetIdentity();
}

Matrix::Matrix(float _11, float _12, float _13, float _14, float _21, float _22, float _23, float _24, float _31, float _32, float _33, float _34, float _41, float _42, float _43, float _44)
	: _11(_11), _12(_12), _13(_13), _14(_14)
	, _21(_21), _22(_22), _23(_23), _24(_24)
	, _31(_31), _32(_32), _33(_33), _34(_34)
	, _41(_41), _42(_42), _43(_43), _44(_44)
{
}

const Vector3 Matrix::GetScale()
{
	int signX = Math::Sign(_11 * _12 * _13) ? -1 : 1;
	int signY = Math::Sign(_21 * _22 * _23) ? -1 : 1;
	int signZ = Math::Sign(_31 * _32 * _33) ? -1 : 1;

	return Vector3
	(
		signX * sqrt(pow(_11, 2) + pow(_12, 2) + pow(_13, 2)),
		signY * sqrt(pow(_21, 2) + pow(_22, 2) + pow(_23, 2)),
		signZ * sqrt(pow(_31, 2) + pow(_32, 2) + pow(_33, 2))
	);
}

const Vector3 Matrix::GetRotation()
{
	Vector3 scale = GetScale();

	bool bCheck = false;
	bCheck |= scale.x == 0.0f;
	bCheck |= scale.y == 0.0f;
	bCheck |= scale.z == 0.0f;

	if (bCheck)
		return Vector3();

	float factorX = 1.0f / scale.x;
	float factorY = 1.0f / scale.y;
	float factorZ = 1.0f / scale.z;

	Matrix mat;
	mat._11 = _11 * factorX; mat._12 = _12 * factorX; mat._13 = _13 * factorX; mat._14 = 0.0f;
	mat._21 = _21 * factorY; mat._22 = _22 * factorY; mat._23 = _23 * factorY; mat._24 = 0.0f;
	mat._31 = _31 * factorZ; mat._32 = _32 * factorZ; mat._33 = _33 * factorZ; mat._34 = 0.0f;
	mat._41 = 0.0f;	mat._42 = 0.0f; mat._43 = 0.0f; mat._44 = 1.0f;

	//Y - X - Z
	return Vector3
	(
		atan2(mat._31, mat._33),
		atan2(-mat._32, sqrt(pow(mat._12, 2) + pow(mat._22, 2))),
		atan2(mat._12, mat._22)
	);
}

const Vector3 Matrix::GetTranslation()
{
	return Vector3(_41, _42, _43);
}

void Matrix::Decompose(Vector3 & scale, Vector3 & rotation, Vector3 & translation)
{
	scale = GetScale();
	rotation = GetRotation();
	translation = GetTranslation();
}

void Matrix::SetIdentity()
{
	_11 = 1; _12 = 0; _13 = 0; _14 = 0;
	_21 = 0; _22 = 1; _23 = 0; _24 = 0;
	_31 = 0; _32 = 0; _33 = 1; _34 = 0;
	_41 = 0; _42 = 0; _43 = 0; _44 = 1;
}

const Matrix Matrix::operator*(const Matrix & rhs) const
{
	return Matrix
	(
		_11 * rhs._11 + _12 * rhs._21 + _13 * rhs._31 + _14 * rhs._41,
		_11 * rhs._12 + _12 * rhs._22 + _13 * rhs._32 + _14 * rhs._42,
		_11 * rhs._13 + _12 * rhs._23 + _13 * rhs._33 + _14 * rhs._43,
		_11 * rhs._14 + _12 * rhs._24 + _13 * rhs._34 + _14 * rhs._44,
		_21 * rhs._11 + _22 * rhs._21 + _23 * rhs._31 + _24 * rhs._41,
		_21 * rhs._12 + _22 * rhs._22 + _23 * rhs._32 + _24 * rhs._42,
		_21 * rhs._13 + _22 * rhs._23 + _23 * rhs._33 + _24 * rhs._43,
		_21 * rhs._14 + _22 * rhs._24 + _23 * rhs._34 + _24 * rhs._44,
		_31 * rhs._11 + _32 * rhs._21 + _33 * rhs._31 + _34 * rhs._41,
		_31 * rhs._12 + _32 * rhs._22 + _33 * rhs._32 + _34 * rhs._42,
		_31 * rhs._13 + _32 * rhs._23 + _33 * rhs._33 + _34 * rhs._43,
		_31 * rhs._14 + _32 * rhs._24 + _33 * rhs._34 + _34 * rhs._44,
		_41 * rhs._11 + _42 * rhs._21 + _43 * rhs._31 + _44 * rhs._41,
		_41 * rhs._12 + _42 * rhs._22 + _43 * rhs._32 + _44 * rhs._42,
		_41 * rhs._13 + _42 * rhs._23 + _43 * rhs._33 + _44 * rhs._43,
		_41 * rhs._14 + _42 * rhs._24 + _43 * rhs._34 + _44 * rhs._44
	);
}

const bool Matrix::operator==(const Matrix & rhs) const
{
	const float* plhs = Data();
	const float* prhs = rhs.Data();

	for (uint i = 0; i < 16; i++)
	{
		if (!Math::Equals(plhs[i], prhs[i]))
			return false;
	}

	return true;
}
