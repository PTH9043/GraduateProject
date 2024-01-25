#pragma once

#include "UPawn.h"

BEGIN(Engine)

class UVIBufferRect;

END

BEGIN(Tool)


class TTestObject : public UPawn
{
public:
	virtual void Collision(CSHPTRREF<UPawn> _pEnemy) override;
private:
	SHPTR< UVIBufferRect>  m_spVIBufferRect;
};

END