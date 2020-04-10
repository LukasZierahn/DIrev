#include "Include.h"
#include "Ship.h"
#include "Model.h"

class Model;
class RenderClass;

#ifndef __H__Nosu
#define __H__Nosu

class Nosu
{
public:
	Model* mod;
	Ship* ShipStats;
	Nosu(RenderClass*);
	~Nosu();

private:
	RenderClass* Render;
};

#endif