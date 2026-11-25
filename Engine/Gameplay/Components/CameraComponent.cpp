#include "CameraComponent.h"
#include <math.h>
#include <algorithm>
#include "../../Math/MathHelpers.h"
#include "../../Renderer/RenderCommandScheduler.h"
#include "../RenderStates.h"

CameraComponent::CameraComponent(const Vector3& inPosition, const Vector3& inDirection, float inFOVdeg, float inAspectRatio, float inNearPlane, float inFarPlane)
	: FOVdeg(inFOVdeg)
	, AspectRatio(inAspectRatio)
	, NearPlane(inNearPlane)
	, FarPlane(inFarPlane)
	, RenderStates(nullptr)
{
	TransformData.SetPosition(inPosition);
	TransformData.SetRotation(VectorHelpers::GetEulerDegreesFrom(inDirection));
}

void CameraComponent::CreateRenderState()
{
	CameraRenderStates* NewRenderState = new CameraRenderStates(this);
	RenderStates = NewRenderState;

	RenderCommandScheduler::Enqueue([NewRenderState](RHIInterface*, RenderScene& inScene)
		{
			inScene.AddCameraRenderStates(NewRenderState);
		});
}

void CameraComponent::DestroyRenderState()
{
	if (RenderStates)
	{
		RenderCommandScheduler::Enqueue([OldRenderState = std::move(RenderStates)](RHIInterface*, RenderScene& inScene)
			{
				inScene.RemoveCameraRenderStates(OldRenderState);
				delete OldRenderState;
			});
	}

	RenderStates = nullptr;
}

void CameraComponent::Deinitialize()
{
	DestroyRenderState();
}

void CameraComponent::Rotate(const Vector3& inDeltaRotation)
{
	Vector3 NewRotation = TransformData.GetRotation() + inDeltaRotation;

	constexpr float RotationClamp = 89.9999f;
	NewRotation.X = std::clamp(NewRotation.X, -RotationClamp, RotationClamp);
	TransformData.SetRotation(NewRotation);

	MarkRenderStateDirty();
}

void CameraComponent::Translate(const Vector3& inDeltaTranslation)
{
	Vector3 OrientedTranslationDelta = inDeltaTranslation;
	VectorHelpers::RotateDegrees(OrientedTranslationDelta, TransformData.GetRotation());
	TransformData.Translate(OrientedTranslationDelta);
}