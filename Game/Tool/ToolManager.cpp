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

    if (m_pScene && m_pScene->m_pToolGrid)
    {
        ImGui::Text("ToolGrid Bounding Box:");
        BoundingOrientedBox boundingBox = m_pScene->m_pToolGrid->GetBOB();

        // 그리드 메쉬 바운딩 박스 중심
        XMFLOAT3 center = boundingBox.Center;
        ImGui::Text("Center: (%.1f, %.1f, %.1f)", center.x, center.y, center.z);

        // 그리드 메쉬 바운딩 박스 크기
        XMFLOAT3 extents = boundingBox.Extents;
        ImGui::Text("Extents: (%.1f, %.1f, %.1f)", extents.x, extents.y, extents.z);

        // 그리드 메쉬 바운딩 박스 회전
        XMFLOAT4 orientation = boundingBox.Orientation;
        ImGui::Text("Orientation: (%.2f, %.2f, %.2f, %.2f)", orientation.x, orientation.y, orientation.z, orientation.w);
    }

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
    ray.origin = CameraPostionfloat3;
    XMFLOAT3 rayDirection;

    rayDirection.x = worldSpace.x - CameraPostionfloat3.x;
    rayDirection.y = worldSpace.y - CameraPostionfloat3.y;
    rayDirection.z = worldSpace.z - CameraPostionfloat3.z;

    XMVECTOR normalizedrayDirection = XMVector3Normalize(XMLoadFloat3(&rayDirection));
    ray.direction = normalizedrayDirection;

    //광선 충돌 검사
    RayHitResult hitResult = CastRay(ray, pScene->m_pToolGrid);

    return hitResult.hit ? hitResult.position : XMFLOAT3(0.0f, 0.0f, 0.0f);
}

RayHitResult ToolManager::CastRay(const Ray& ray, const shared_ptr<ToolGridObject>& Grid)
{
    RayHitResult result;
    result.hit = false;

    //충돌 검사
    float intersectionDistance;
    if (Grid->GetBOB().Intersects(XMLoadFloat3(&ray.origin), ray.direction, intersectionDistance))
    {
        // 광선이 박스와 교차하면 true를 반환하고, 충돌 지점을 계산
        XMVECTOR intersectionPointVector = XMVectorAdd(XMLoadFloat3(&ray.origin), XMVectorScale(ray.direction, intersectionDistance));
        XMStoreFloat3(&result.position, intersectionPointVector);
        result.hit = true;
    }
    else
    {
        result.hit = false;
    }

    return result;
}

