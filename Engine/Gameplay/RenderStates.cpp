#include "RenderStates.h"
#include "../AssetManagement/Mesh.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CameraComponent.h"
#include "../Math/MathHelpers.h"

CameraRenderStates::CameraRenderStates(const CameraComponent* inComponent)
	: View(inComponent->GetTransform().ToMatrixNoScale())
	, Projection(MatrixHelpers::MakeProjectionMatrix(inComponent->GetFOV(), inComponent->GetAspectRatio(), inComponent->GetNearPlane(), inComponent->GetFarPlane()))
	, ViewProjection(View * Projection)
{
}

StaticMeshRenderStates::StaticMeshRenderStates(const StaticMeshComponent* inComponent)
	: World(inComponent->GetTransform().ToMatrix())
	, MeshRenderData(inComponent->GetMesh()->GetRenderData())
{
}