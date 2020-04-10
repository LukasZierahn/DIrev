#include "Cooge.h"
#include "Model.h"
#include "RenderClass.h"
#include "Ship.h"

Cooge::Cooge(RenderClass* RndCls)
{
	Render = RndCls;

	mod = new Model(Render, Model_Type_Cooge, true);

	ShipStats = new Ship(Model_Type_Cooge);
}

Cooge::~Cooge()
{
	delete mod;
	delete ShipStats;
}
