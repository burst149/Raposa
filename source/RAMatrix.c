#include "../headers/RAMatrix.h"
#include <stdio.h>
#define PI (4*atan(1))

RAMatrix3 RAMatrix3Make(
	float m11, float m21, float m31,
	float m12, float m22, float m32,
	float m13, float m23, float m33
)
{
	RAMatrix3 m =
	{
		m11, m21, m31,
		m12, m22, m32,
		m13, m23, m33
	};
	return m;

}

RAMatrix3 RAMatrix3MakeTranslate(float tx, float ty)
{
	RAMatrix3 m =
	{
		1.0f, 0.0f, tx,
		0.0f, 1.0f, ty,
		0.0f, 0.0f, 1.0f
	};
	return m;

}

RAMatrix3 RAMatrix3MakeRotate(float theta)
{
	float cosT = cosf(theta);
	float sinT = sinf(theta);
	RAMatrix3 m =
	{
		cosT, -sinT, 0.0f,
		sinT,  cosT, 0.0f,
		0.0f,  0.0f, 1.0f
	};
	return m;
}

RAMatrix3 RAMatrix3MakeScale(float sx, float sy)
{
	RAMatrix3 m =
	{
		  sx, 0.0f, 0.0f,
		0.0f,   sy, 0.0f,
		0.0f, 0.0f, 1.0f
	};
	return m;
}

RAMatrix3 RAMatrix3Concatinate(RAMatrix3 m1, RAMatrix3 m2)
{
	float m11 = m1.m11 * m2.m11 + m1.m21 * m2.m12 + m1.m31 * m2.m13;
	float m21 = m1.m11 * m2.m21 + m1.m21 * m2.m22 + m1.m31 * m2.m23;
	float m31 = m1.m11 * m2.m31 + m1.m21 * m2.m32 + m1.m31 * m2.m33;

	float m12 = m1.m12 * m2.m11 + m1.m22 * m2.m12 + m1.m32 * m2.m13;
	float m22 = m1.m12 * m2.m21 + m1.m22 * m2.m22 + m1.m32 * m2.m23;
	float m32 = m1.m12 * m2.m31 + m1.m22 * m2.m32 + m1.m32 * m2.m33;

	float m13 = m1.m13 * m2.m11 + m1.m23 * m2.m12 + m1.m33 * m2.m13;
	float m23 = m1.m13 * m2.m21 + m1.m23 * m2.m22 + m1.m33 * m2.m23;
	float m33 = m1.m13 * m2.m31 + m1.m23 * m2.m32 + m1.m33 * m2.m33;

	RAMatrix3 m =
	{
		m11, m21, m31,
		m12, m22, m32,
		m13, m23, m33
	};
	return m;
}

RAVector3 RAMatrix3MultiplyWithZ0(RAMatrix3 m, RAVector2 v)
{
	return RAMatrix3Multiply(m, RAVector3Make(v.x, v.y, 0.0f));
}


RAVector3 RAMatrix3MultiplyWithZ1(RAMatrix3 m, RAVector2 v)
{
	return RAMatrix3Multiply(m, RAVector3Make(v.x, v.y, 1.0f));
}

RAVector3 RAMatrix3Multiply(RAMatrix3 m, RAVector3 v)
{
	float vx = m.m11 * v.x + m.m21 * v.y + m.m31 * v.z;
	float vy = m.m12 * v.x + m.m22 * v.y + m.m32 * v.z;
	float vz = m.m13 * v.x + m.m23 * v.y + m.m33 * v.z;
	return RAVector3Make(vx, vy, vz);
}





RAMatrix4 RAMatrix4Make(
	float m11, float m21, float m31, float m41,
	float m12, float m22, float m32, float m42,
	float m13, float m23, float m33, float m43,
	float m14, float m24, float m34, float m44
)
{
	RAMatrix4 m =
	{
		m11, m21, m31, m41,
		m12, m22, m32, m42,
		m13, m23, m33, m43,
		m14, m24, m34, m44
	};
	return m;

}

RAMatrix4 RAMatrix4MakeTranslate(float tx, float ty, float tz)
{
	RAMatrix4 m =
	{
		1.0f, 0.0f, 0.0f, tx,
		0.0f, 1.0f, 0.0f, ty,
		0.0f, 0.0f, 1.0f, tz,
		0.0f, 0.0f, 0.0f, 1.0f
	};
	return m;

}

RAMatrix4 RAMatrix4MakeRotateX(float theta)
{
	float cosT = cosf(theta);
	float sinT = sinf(theta);
	RAMatrix4 m =
	{
		1.0f, 0.0f,  0.0f, 0.0f,
		0.0f, cosT, -sinT, 0.0f,
		0.0f, sinT,  cosT, 0.0f,
		0.0f, 0.0f,  0.0f, 1.0f
	};
	return m;
}

RAMatrix4 RAMatrix4MakeRotateY(float theta)
{
	float cosT = cosf(theta);
	float sinT = sinf(theta);
	RAMatrix4 m =
	{
		 cosT, 0.0f, sinT, 0.0f,
		 0.0f, 1.0f, 0.0f, 0.0f,
		-sinT, 0.0f, cosT, 0.0f,
		 0.0f, 0.0f, 0.0f, 1.0f
	};
	return m;
}

RAMatrix4 RAMatrix4MakeRotateZ(float theta)
{
	float cosT = cosf(theta);
	float sinT = sinf(theta);
	RAMatrix4 m =
	{
		cosT, -sinT, 0.0f, 0.0f,
		sinT,  cosT, 0.0f, 0.0f,
		0.0f,  0.0f, 1.0f, 0.0f,
		0.0f,  0.0f, 0.0f, 1.0f
	};
	return m;
}

RAMatrix4 RAMatrix4MakeRotate(float theta, RAVector3 a)
{
	// Calculate rotation angles
	float alpha = atan2f(a.x, a.y);
	float beta = atan2f(a.y, a.z);

	// Resolve gimbal lock
	if (a.y == 0.0f && a.z == 0.0f)
	{
		if (a.x == 0.0f)
		{
			alpha = 0.0f;
			beta = 0.0f;
			theta = 0.0f;
		}
		else if (a.x > 0.0f)
			beta = PI/2;
		else
			beta = -PI/2;
	}

	// Calculate rotation matrix
	RAMatrix4 m = RAMatrix4Identity;
	m = RAMatrix4Concatinate(m, RAMatrix4MakeRotateZ(-alpha));
	m = RAMatrix4Concatinate(m, RAMatrix4MakeRotateX(-beta));
	m = RAMatrix4Concatinate(m, RAMatrix4MakeRotateZ(theta));
	m = RAMatrix4Concatinate(m, RAMatrix4MakeRotateX(beta));
	m = RAMatrix4Concatinate(m, RAMatrix4MakeRotateZ(alpha));
	return m;
}

RAMatrix4 RAMatrix4MakeScale(float sx, float sy, float sz)
{
	RAMatrix4 m =
	{
		  sx, 0.0f, 0.0f, 0.0f,
		0.0f,   sy, 0.0f, 0.0f,
		0.0f, 0.0f,   sz, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};
	return m;
}

RAMatrix4 RAMatrix4MakeLookAt(RAVector3 eyePoint, RAVector3 spotPoint, RAVector3 upVector)
{
	RAVector3 look = RAVector3Normalize(RAVector3Subtract(spotPoint, eyePoint));
	RAVector3 right = RAVector3Normalize(RAVector3Cross(look, upVector));
	RAVector3 up = RAVector3Normalize(RAVector3Cross(right, look));

	RAMatrix4 b = RAMatrix4Make(
		right.x, right.y, right.z, 0.0f,
		   up.x,    up.y,    up.z, 0.0f,
		-look.x, -look.y, -look.z, 0.0f,
		   0.0f,    0.0f,    0.0f, 1.0f);

	RAMatrix4 t = RAMatrix4MakeTranslate(-eyePoint.x, - eyePoint.y, -eyePoint.z);
	return RAMatrix4Concatinate(b, t);
}

RAMatrix4 RAMatrix4MakeOrthogonal(float minX, float maxX, float minY, float maxY, float minZ, float maxZ)
{
	RAMatrix4 m = RAMatrix4Identity;
	m = RAMatrix4Concatinate(m, RAMatrix4MakeScale(2.0f / (maxX - minX), 2.0f / (maxY - minY), 2.0f / (maxZ - minZ)));
	m = RAMatrix4Concatinate(m, RAMatrix4MakeTranslate(-(maxX - minX) / 2.0f, -(maxY - minY) / 2.0f, -(maxZ - minZ) / 2.0f));
	m = RAMatrix4Concatinate(m, RAMatrix4MakeTranslate(-minX, -minY, -minZ));
	return m;
}

RAMatrix4 RAMatrix4MakeFrustum(float minX, float maxX, float minY, float maxY, float minZ, float maxZ)
{
	float width = (maxX - minX);
	float height = (maxY- minY);
	float length = (maxZ - minZ);

	float sx = 2.0f * minZ / width;
	float sy = 2.0f * minZ / height;
	float sz = -(maxZ + minZ) / length;

	float hx = (maxX + minX) / width;
	float hy = (maxY + minY) / height;

	float tz = -2.0f * maxZ * minZ / length;

	return RAMatrix4Make(
		  sx,  0.0f,    hx,  0.0f,
		0.0f,    sy,    hy,  0.0f,
		0.0f,  0.0f,    sz,    tz,
		0.0f,  0.0f, -1.0f,  0.0f);
}

RAMatrix4 RAMatrix4MakePerspective(float fieldOfView, float aspectRatio, float minZ, float maxZ)
{
	float sx = aspectRatio / tanf(fieldOfView * 0.5f);
	float sy = 1.0f / tanf(fieldOfView * 0.5f);
	float sz = -(maxZ + minZ) / (maxZ - minZ);
	float tz = -2.0f * maxZ * minZ / (maxZ - minZ);

	return RAMatrix4Make(
		  sx,  0.0f,  0.0f,  0.0f,
		0.0f,    sy,  0.0f,  0.0f,
		0.0f,  0.0f,    sz,    tz,
		0.0f,  0.0f, -1.0f,  0.0f);
}

RAMatrix4 RAMatrix4MakeViewport(float x, float y, float width, float height)
{
	RAMatrix4 m = RAMatrix4Identity;
	m = RAMatrix4Concatinate(m, RAMatrix4MakeTranslate(x, y, 0.0f));
	m = RAMatrix4Concatinate(m, RAMatrix4MakeScale(width, height, 1.0f));
	m = RAMatrix4Concatinate(m, RAMatrix4MakeScale(0.5f, 0.5f, 0.5f));
	m = RAMatrix4Concatinate(m, RAMatrix4MakeTranslate(1.0f, 1.0f, 1.0f));
	return m;
}

RAMatrix4 RAMatrix4Concatinate(RAMatrix4 m1, RAMatrix4 m2)
{
	float m11 = m1.m11 * m2.m11 + m1.m21 * m2.m12 + m1.m31 * m2.m13 + m1.m41 * m2.m14;
	float m21 = m1.m11 * m2.m21 + m1.m21 * m2.m22 + m1.m31 * m2.m23 + m1.m41 * m2.m24;
	float m31 = m1.m11 * m2.m31 + m1.m21 * m2.m32 + m1.m31 * m2.m33 + m1.m41 * m2.m34;
	float m41 = m1.m11 * m2.m41 + m1.m21 * m2.m42 + m1.m31 * m2.m43 + m1.m41 * m2.m44;

	float m12 = m1.m12 * m2.m11 + m1.m22 * m2.m12 + m1.m32 * m2.m13 + m1.m42 * m2.m14;
	float m22 = m1.m12 * m2.m21 + m1.m22 * m2.m22 + m1.m32 * m2.m23 + m1.m42 * m2.m24;
	float m32 = m1.m12 * m2.m31 + m1.m22 * m2.m32 + m1.m32 * m2.m33 + m1.m42 * m2.m34;
	float m42 = m1.m12 * m2.m41 + m1.m22 * m2.m42 + m1.m32 * m2.m43 + m1.m42 * m2.m44;

	float m13 = m1.m13 * m2.m11 + m1.m23 * m2.m12 + m1.m33 * m2.m13 + m1.m43 * m2.m14;
	float m23 = m1.m13 * m2.m21 + m1.m23 * m2.m22 + m1.m33 * m2.m23 + m1.m43 * m2.m24;
	float m33 = m1.m13 * m2.m31 + m1.m23 * m2.m32 + m1.m33 * m2.m33 + m1.m43 * m2.m34;
	float m43 = m1.m13 * m2.m41 + m1.m23 * m2.m42 + m1.m33 * m2.m43 + m1.m43 * m2.m44;

	float m14 = m1.m14 * m2.m11 + m1.m24 * m2.m12 + m1.m34 * m2.m13 + m1.m44 * m2.m14;
	float m24 = m1.m14 * m2.m21 + m1.m24 * m2.m22 + m1.m34 * m2.m23 + m1.m44 * m2.m24;
	float m34 = m1.m14 * m2.m31 + m1.m24 * m2.m32 + m1.m34 * m2.m33 + m1.m44 * m2.m34;
	float m44 = m1.m14 * m2.m41 + m1.m24 * m2.m42 + m1.m34 * m2.m43 + m1.m44 * m2.m44;

	RAMatrix4 m =
	{
		m11, m21, m31, m41,
		m12, m22, m32, m42,
		m13, m23, m33, m43,
		m14, m24, m34, m44
	};
	return m;
}

RAVector3 RAMatrix4MultiplyWithW0(RAMatrix4 m, RAVector3 v)
{
	return RAVector4Project(RAMatrix4Multiply(m, RAVector4Make(v.x, v.y, v.z, 0.0f)));
}

RAVector3 RAMatrix4MultiplyWithW1(RAMatrix4 m, RAVector3 v)
{
	return RAVector4Project(RAMatrix4Multiply(m, RAVector4Make(v.x, v.y, v.z, 1.0f)));
}

RAVector4 RAMatrix4Multiply(RAMatrix4 m, RAVector4 v)
{
	float vx = m.m11 * v.x + m.m21 * v.y + m.m31 * v.z + m.m41 * v.w;
	float vy = m.m12 * v.x + m.m22 * v.y + m.m32 * v.z + m.m42 * v.w;
	float vz = m.m13 * v.x + m.m23 * v.y + m.m33 * v.z + m.m43 * v.w;
	float vw = m.m14 * v.x + m.m24 * v.y + m.m34 * v.z + m.m44 * v.w;
	return RAVector4Make(vx, vy, vz, vw);
}