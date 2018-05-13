// a quickly hacked together implementation of ArcBall, with lots of
// bits lifted from Ken's original source
// made into a nice C++ thing

// written October, 2001, Michael L. Gleicher

#include "MyArcBall.H"
#include <math.h>

enum QuatPart { X = 0, Y = 1, Z = 2, W = 3, QuatLen };

///////////////////////////
// minimal quaternion class
Quat::Quat(float ix, float iy, float iz, float iw) :
x(ix), y(iy), z(iz), w(iw)
{
}
Quat::Quat() :
x(0), y(0), z(0), w(1)
{
}
Quat::Quat(const Quat& q) :
x(q.x), y(q.y), z(q.z), w(q.w)
{
}

// renormalize, in case things got messed up
void Quat::renorm()
{
	float Nq = 1.f / (float)(x*x + y*y + z*z + w*w);
	x *= Nq;
	y *= Nq;
	z *= Nq;
	w *= Nq;
}

// conversions
void Quat::toMatrix(HMatrix out) const
{
	float Nq = x*x + y*y + z*z + w*w;
	float s = (Nq > 0.f) ? (2.0f / Nq) : 0.f;
	float xs = x*s, ys = y*s, zs = z*s;
	float wx = w*xs, wy = w*ys, wz = w*zs;
	float xx = x*xs, xy = x*ys, xz = x*zs;
	float yy = y*ys, yz = y*zs, zz = z*zs;
	out[X][X] = 1.0f - (yy + zz); out[Y][X] = xy + wz;          out[Z][X] = xz - wy;
	out[X][Y] = xy - wz;          out[Y][Y] = 1.0f - (xx + zz); out[Z][Y] = yz + wx;
	out[X][Z] = xz + wy;          out[Y][Z] = yz - wx;          out[Z][Z] = 1.0f - (xx + yy);
	out[X][W] = out[Y][W] = out[Z][W] = out[W][X] = out[W][Y] = out[W][Z] = 0.0f;
	out[W][W] = 1.0f;
}

//////////////////////////////////////////////////////////////
// quaternion operations
Quat Quat::conjugate() const
{
	return Quat(-x, -y, -z, w);
}

Quat Quat::operator* (const Quat& qR) const
{
	Quat qq;
	qq.w = w*qR.w - x*qR.x - y*qR.y - z*qR.z;
	qq.x = w*qR.x + x*qR.w + y*qR.z - z*qR.y;
	qq.y = w*qR.y + y*qR.w + z*qR.x - x*qR.z;
	qq.z = w*qR.z + z*qR.w + x*qR.y - y*qR.x;
	return (qq);
}


/////////////////////////////////////////////////////////////////
ArcBall::ArcBall()
{
}

// when the mouse goes down, remember where. also, clear out the now
// position by putting it into start
void ArcBall::down(const float x, const float y)
{
	start = now * start;
	now = Quat();		// identity

	downX = x;
	downY = y;
}

// get the whole matrix!
void ArcBall::getMatrix(HMatrix m) const
{
	Quat qAll = now * start;
	qAll = qAll.conjugate();   // since Ken does everything transposed
	qAll.toMatrix(m);
}

// the gory guts

// a helper - figure out where in XYZ a mouse down goes
// assumes sphere of unit radius
static void onUnitSphere(const float mx, const float my,
	float& x, float& y, float& z)
{
	x = mx;		// should divide radius
	y = my;
	float mag = x*x + y*y;
	if (mag > 1.0f) {
		float scale = 1.0f / ((float)sqrt(mag));
		x *= scale;
		y *= scale;
		z = 0;
	}
	else {
		z = (float)sqrt(1 - mag);
	}
}

void ArcBall::computeNow(const float nowX, const float nowY)
{
	float dx, dy, dz;
	float mx, my, mz;
	onUnitSphere(downX, downY, dx, dy, dz);
	onUnitSphere(nowX, nowY, mx, my, mz);

	// here we compute the quaternion between these two points
	now.x = dy*mz - dz*my;
	now.y = dz*mx - dx*mz;
	now.z = dx*my - dy*mx;
	now.w = dx*mx + dy*my + dz*mz;

	now.renorm();		// just in case...
}