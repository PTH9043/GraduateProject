#pragma once
#include "ToolDefines.h"
#include "ToolScene.h"

struct Ray
{
	XMFLOAT3 origin;
	XMVECTOR direction;
};

struct RayHitResult
{
	bool hit;
	XMFLOAT3 position;
};

class ToolManager
{

public:
	void InitCamera(shared_ptr<CCamera>& pCamera); 
	void InitScene(shared_ptr<ToolScene>& pScene);

	void DisplayWindow();
	XMFLOAT3 ScreenToWorldCoordinates(float screenX, float screenY, const shared_ptr<ToolScene>& pScene);
	RayHitResult CastRay(const Ray& ray, const shared_ptr<ToolGridObject>& Grid);
private:
	ImVec2 clickPos;
	XMFLOAT3 worldPos;
	shared_ptr<CCamera> m_pCamera;
	shared_ptr<ToolScene> m_pScene; 


};

