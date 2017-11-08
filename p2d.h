//////////////////////////////////
//          2D Point Class      //
// 	  Modelling 2D Points   //
//                              //
//      (C) 2017 Jiayao Zhang   //
//          v0.1 27-Apr-2017    //
//////////////////////////////////
// p2d.h
#ifndef P2D_H
#define P2D_H

typedef double p2d_type;

struct p2d
{
	p2d_type x; p2d_type y;
	p2d(p2d_type x, p2d_type y) : x(x), y(y) {}
	p2d(const p2d &p) : p2d(p.x, p.y) {}
	p2d operator()(const p2d&);
	void set(const p2d&);
};

inline p2d operator-(const p2d &l, const p2d &r)
{
	return p2d(l.x - r.x, l.y - r.y);
}
inline p2d operator+(const p2d &l, const p2d &r)
{
	return p2d(l.x + r.x, l.y + r.y);
}
inline p2d operator*(const p2d &p, double d)
{
	return p2d(p.x * d, p.y * d);
}

inline double operator*(const p2d &l, const p2d &r)
{
	return (l.x - r.x) * (l.x - r.x) + (l.y - r.y) * (l.y - r.y);
}

inline p2d operator+=(p2d &l, const p2d &r)
{
	l.x += r.x; l.y += r.y;
	return l;
}
inline p2d operator+=(p2d &p, double d)
{
	p.x += d; p.y += d;
	return p;
}
inline p2d operator/=(p2d &p, double d)
{
	if(d != 0)
	{
		p.x /= d; p.y /= d;
	}
	return p;
}

#endif

