#include "Include.h"
#include "Ship.h"
#include "Model.h"

class Model;
class RenderClass;

#ifndef __H__Cooge
#define __H__Cooge

class Cooge
{
public:
	Model* mod;
	Ship* ShipStats;
	Cooge(RenderClass*);
	~Cooge();

private:
	RenderClass* Render;
};

#endif