#include "Lebigrand.h"
#include "Model.h"
#include "RenderClass.h"
#include "Ship.h"

Lebigrand::Lebigrand(RenderClass* RndCls)
{
	Render = RndCls;

	mod = new Model(Render, Model_Type_Lebigrand);

	ShipStats = new Ship(Model_Type_Lebigrand);
}

Lebigrand::~Lebigrand()
{
	delete mod;
	delete ShipStats;
}
