#include "Ship.h"
#include "AllShips.h"

int ShipStates[] = {	10,	1,	1,	1,		//Lebigrand
						2,	1,	1,	1,		//Cooge
						1,	1,	1,	1		//Nosu				
					};	

int ShipCaptainPos[] = {	0,	0,	0,			//Lebigrand
							0,	0,	0,			//Cooge
							0,	0,	0			//Nosu
						};		

Ship::Ship(int Type)
{
	Acc				= ShipStates[Type];
	Speed			= ShipStates[Type + 1];
	RotationSpeedL	= ShipStates[Type + 2];
	RotationSpeedR	= ShipStates[Type + 3];

	CaptainPos = &XMVectorSet(ShipCaptainPos[Type], ShipCaptainPos[Type + 1], ShipCaptainPos[Type + 2], 0.0f);
}

Ship::Ship(int A, int S, int R, int L)
{
	Acc = A;
	Speed = S;
	RotationSpeedR = R;
	RotationSpeedL = L;
}

Ship::~Ship()
{
}
