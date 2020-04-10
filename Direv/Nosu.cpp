#include "Nosu.h"
#include "Model.h"
#include "RenderClass.h"
#include "Ship.h"

Nosu::Nosu(RenderClass* RndCls)
{
	Render = RndCls;

	mod = new Model(Render, Model_Type_Nosu);

	ShipStats = new Ship(Model_Type_Nosu);
}

Nosu::~Nosu()
{
	delete mod;
	delete ShipStats;
}
