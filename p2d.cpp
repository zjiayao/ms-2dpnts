//////////////////////////////////
//		2D Point Class 			//	
// 	  Modelling 2D Points		//
// 								//
// 	(C) 2017 Jiayao Zhang		//
// 		v0.1 27-Apr-2017		//
//////////////////////////////////
// p2d.cpp
#include "p2d.h"

p2d p2d::operator()(const p2d &p)
{
	this->x = p.x; this->y = p.y; return *this;
}

void p2d::set(const p2d &p)
{ this->x = p.x; this->y = p.y; }