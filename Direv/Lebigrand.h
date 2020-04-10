#pragma once
#include "Include.h"
#include "Ship.h"
#include "Model.h"

class RenderClass;
class Ship;

#ifndef __H__Lebigrand
#define __H__Lebigrand

class Lebigrand
{
public:
	Model* mod;
	Ship* ShipStats;
	Lebigrand(RenderClass*);
	~Lebigrand();

private:
	RenderClass* Render;
};

#endif