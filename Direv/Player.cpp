#include "Player.h"
#include "RenderClass.h"
#include "Input.h"

Player::Player(RenderClass* rend)
{
	Render = rend;
	inp = Render->GetInput();
}

void Player::tick(int t)
{
	if (InAShip)
	{
		inp->ShipTick(t);
	}
	else
	{
		inp->RunTick(t);
	}
}

Player::~Player()
{
}
