#include "Include.h"

#ifndef __H__Gravity
#define __H__Gravity

const float acceleration = 9.81f;

class Gravity
{
public:
	Gravity();
	~Gravity();
private:
	void StartFalling();
	void StopFalling();

	float downSpeed = 0;

};

#endif