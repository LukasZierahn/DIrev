#pragma once
#include "Include.h"

#ifndef __H__Input
#define __H__Input

class RenderClass;
class Model;
class Ship;

class Input
{
public:
	Input(RenderClass*, RECT);
	void Key(bool, WPARAM);
	void Mouse(WPARAM, int, int);

	void EnterShipInfront();

	void RunTick(int);
	void ShipTick(int);
	void SetShip(Model*, Ship*);

private:
	RenderClass* Render;
	RECT WndRect;

	Model* mod;
	Ship* CurrentShip;

	int* ShipAcc;
	int* ShipSpeed;
	int* ShipRotationSpeed;

	float speed = 0.005f;

	float MouseSense = 500.0f;

	bool keyW = false;
	bool keyA = false;
	bool keyS = false;
	bool keyD = false;

	bool keyC = false;
	bool keyV = false;

	float x, y, z;
	float Rx, Rz;
};

#endif