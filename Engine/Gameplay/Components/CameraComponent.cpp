#include "CameraComponent.h"
#include <math.h>
#include <algorithm>
#include "../../Math/MathHelpers.h"

CameraComponent::CameraComponent(const Vector3& inPosition, const Vector3& inDirection, float inFOVdeg, float inAspectRatio, float inNearPlane, float inFarPlane)
	: FOVdeg(inFOVdeg)
	, AspectRatio(inAspectRatio)
	, NearPlane(inNearPlane)
	, FarPlane(inFarPlane)
{
	TransformData.SetPosition(inPosition);
	TransformData.SetRotation(VectorHelpers::GetEulerDegreesFrom(inDirection));
}

void CameraComponent::Rotate(const Vector3& inDeltaRotation)
{
	Vector3 NewRotation = TransformData.GetRotation() + inDeltaRotation;

	constexpr float RotationClamp = 89.9999f;
	NewRotation.X = std::clamp(NewRotation.X, -RotationClamp, RotationClamp);
	TransformData.SetRotation(NewRotation);
}

void CameraComponent::Translate(const Vector3& inDeltaTranslation)
{
	Vector3 OrientedTranslationDelta = inDeltaTranslation;
	VectorHelpers::RotateDegrees(OrientedTranslationDelta, TransformData.GetRotation());
	TransformData.Translate(OrientedTranslationDelta);
}