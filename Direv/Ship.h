#pragma once
#include "Include.h"

#ifndef __H__Ship
#define __H__Ship

class Ship
{
public:
	Ship(int);
	Ship(int, int, int, int);
	~Ship();

private:
	int Acc;
	int Speed;
	int RotationSpeedL;
	int RotationSpeedR;

	XMVECTOR* CaptainPos;
};

#endif