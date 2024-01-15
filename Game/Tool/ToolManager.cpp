#include "ToolManager.h"
#include "ToolGridObject.h"

void ToolManager::InitCamera(shared_ptr<CCamera>& pCamera)
{
    m_pCamera = pCamera;
}

void ToolManager::InitScene(shared_ptr<ToolScene>& pScene)
{
    m_pScene = pScene;
}

void ToolManager::DisplayWindow()
{
    static float f = 0.0f;
    static int counter = 0;

    ImGui::Begin("Mini Engine");                         

   //마우스 좌표 반환
    ImVec2 mousePos = ImGui::GetMousePos();
    ImGui::Text("Mouse Position: (%.1f, %.1f)", mousePos.x, mousePos.y);

    if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
    {
        clickPos = ImVec2(mousePos.x, mousePos.y);
        worldPos = ScreenToWorldCoordinates(clickPos.x, clickPos.y, m_pScene);
    }

    ImGui::Text("World Position on Click: (%.1f, %.1f, %.1f)", worldPos.x, worldPos.y, worldPos.z);

    ImGui::End();
}

XMFLOAT3 ToolManager::ScreenToWorldCoordinates(float screenX, float screenY, const shared_ptr<ToolScene>& pScene)
{
    XMFLOAT4X4 viewMatrixBeforeLoad = m_pCamera->GetViewMatrix();
    XMFLOAT4X4 projectionMatrixBeforeLoad = m_pCamera->GetProjectionMatrix();

    XMMATRIX viewMatrix = XMLoadFloat4x4(&viewMatrixBeforeLoad);
    XMMATRIX invViewMatrix = XMMatrixInverse(nullptr, viewMatrix);

    XMFLOAT3 CameraPostionfloat3;
    XMStoreFloat3(&CameraPostionfloat3, invViewMatrix.r[3]);

    XMVECTOR CameraPositionVector = XMLoadFloat3(&CameraPostionfloat3);

    XMMATRIX projectionMatrix = XMLoadFloat4x4(&projectionMatrixBeforeLoad);
    XMMATRIX viewProjectionInverse = XMMatrixInverse(nullptr, viewMatrix * projectionMatrix);

    float clipX = (2.0f * screenX / m_pCamera->GetViewPort().Width - 1.0f) / projectionMatrixBeforeLoad._11;
    float clipY = (-2.0f * screenY / m_pCamera->GetViewPort().Height + 1.0f) / projectionMatrixBeforeLoad._22;

    XMVECTOR clipSpacePos = XMVectorSet(clipX, clipY, 0.0f, 1.0f);
    XMVECTOR worldSpacePos = XMVector4Transform(clipSpacePos, viewProjectionInverse);
    worldSpacePos /= XMVectorGetW(worldSpacePos);

    XMFLOAT3 worldSpace;
    XMStoreFloat3(&worldSpace, worldSpacePos);

    //광선 생성
    Ray ray;
    ray.origin = CameraPositionVector;
    XMFLOAT3 rayDirection;

    rayDirection.x = worldSpace.x - CameraPostionfloat3.x;
    rayDirection.y = worldSpace.y - CameraPostionfloat3.y;
    rayDirection.z = worldSpace.z - CameraPostionfloat3.z;

    XMVECTOR normalizedrayDirection = XMVector3Normalize(XMLoadFloat3(&rayDirection));
    ray.direction = normalizedrayDirection;

    //광선 충돌 검사
    RayHitResult hitResult = CastRay(ray, pScene->GetObjects()[0], pScene->GetObjects()[0]->GetWorld4X4());

    return hitResult.hit ? hitResult.position : XMFLOAT3(0.0f, 0.0f, 0.0f);
}

RayHitResult ToolManager::CastRay(const Ray& ray, const shared_ptr<CGameObject>& Grid, const XMFLOAT4X4& worldMatrix)
{
    RayHitResult result;
    result.hit = false;

    // Iterate over each submesh of the grid object
    if(Grid->m_nMeshes != 0)
    {
        for (int i = 0; i < Grid->m_nMeshes; ++i)
        {
            shared_ptr<CMesh> subMesh = Grid->GetMeshes()[i];

            for (size_t j = 0; j < subMesh->GetSubSetIndices().size(); j++)
            {
                for (size_t k = 0; k + 2 < subMesh->GetSubSetIndices()[j].size(); k++)
                {
                    XMFLOAT3 vertex0 = subMesh->TransformPoint(subMesh->GetPositions()[subMesh->GetSubSetIndices()[j][k]], worldMatrix);
                    XMFLOAT3 vertex1 = subMesh->TransformPoint(subMesh->GetPositions()[subMesh->GetSubSetIndices()[j][k + 1]], worldMatrix);
                    XMFLOAT3 vertex2 = subMesh->TransformPoint(subMesh->GetPositions()[subMesh->GetSubSetIndices()[j][k + 2]], worldMatrix);

                    float Distance;
                    if (DirectX::TriangleTests::Intersects(ray.origin, ray.direction,
                        XMLoadFloat3(&vertex0), XMLoadFloat3(&vertex1), XMLoadFloat3(&vertex2), Distance))
                    {
                        XMVECTOR intersectionPointVector = XMVectorAdd(ray.origin, XMVectorScale(ray.direction, Distance));
                        XMFLOAT3 intersectionPoint;
                        XMStoreFloat3(&intersectionPoint, intersectionPointVector);

                        result.position = intersectionPoint;
                        result.hit = true;
                        break;
                    }
                }
            }
        }
    }
    return result;
}


