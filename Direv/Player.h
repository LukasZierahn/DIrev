#pragma once
#include "Include.h"

class RenderClass;
class Input;
class Model;
class Ship;

#ifndef __H__Player
#define __H__PLayer

class Player
{

public:
	Player(RenderClass*);
	void tick(int);
	~Player();

private:
	RenderClass* Render;
	Input* inp;

	bool InAShip = false;
	Model* mod;
	Ship* Ship;
};

#endif