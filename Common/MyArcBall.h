#ifndef MyArcBall_H
#define MyArcBall_H

// a quickly hacked together implementation of ArcBall, with lots of
// bits lifted from Ken's original source
// made into a nice C++ thing
//
// it is simplified a lot - the controller is always at the origin,
// it has a fixed radius, it doesn't have constraints

// written October, 2001, Michael L. Gleicher

// this typedef is useful for lots of stuff
typedef float HMatrix[4][4];

// if you need to pass that to OpenGL, try...
inline float* asGlMatrix(HMatrix m) { return (float*)m; };

// we need a quick and dirty Quaternion class
class Quat {
public:
	Quat();						/* gives the identity */
	Quat(float x, float y, float z, float w);
	Quat(const Quat&);			/* copy constructor */

	// conversions
	void toMatrix(HMatrix) const;

	// operations
	Quat conjugate() const;
	// make multiply look like multiply
	Quat operator* (const Quat&) const;

	void renorm();

	// the data
	float x, y, z, w;
};


////////////////////////////////////////////////
// the arcball implementation
class ArcBall {
public:
	ArcBall();

	Quat start;			// orientation at the start of drag
	Quat now;			// quaternion "now" (while dragging)

	float downX, downY;	// where the mouse went down

	void down(const float x, const float y);

	void computeNow(const float nowX, const float nowY);

	// this gets the global matrix (start and now)
	void getMatrix(HMatrix) const;
};

#endif // MyArcBall_H