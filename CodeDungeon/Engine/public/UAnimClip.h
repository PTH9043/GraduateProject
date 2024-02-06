#pragma once

#include "UBase.h"

BEGIN(Engine)
class UAnimation;
class UChannel;
class UBoneNode;

class UAnimClip final : public UBase {
public:
	UAnimClip();
	NO_COPY(UAnimClip)
	DESTRUCTOR(UAnimClip)
public:


private:
	virtual void Free() override;
private:
};

END