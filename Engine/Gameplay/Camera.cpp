#include "Camera.h"
#include <math.h>
#include <algorithm>
#include "../Math/MathHelpers.h"

Camera::Camera(const Vector3& inPosition, const Vector3& inDirection, const Vector3& inWorldUp, float inFOVdeg, float inAspectRatio, float inNearPlane, float inFarPlane)
	: ViewMatrix()
	, ProjectionMatrix()
	, ViewProjectionMatrix()
	, Position(inPosition)
	, Rotation()
	, Direction(inDirection)
	, bRefreshMatrices(false)
{
	Vector3 Right = inWorldUp.Cross(inDirection);
	Right.Normalize();
	Vector3 Up = Right.Cross(inDirection);
	Up.Normalize();
	ViewMatrix = Matrix44(Vector4(Right, 0.f), Vector4(Up, 0.f), Vector4(inDirection, 0.f), Vector4(inPosition, 1.f));

	const float FOV = MathHelpers::DegreesToRadians(inFOVdeg);
	const float TanFOVRad = tanf(FOV / 2.f);
	const float Depth = inFarPlane - inNearPlane;
	ProjectionMatrix = Matrix44(Vector4(1.f / (inAspectRatio * TanFOVRad), 0.f, 0.f, 0.f), Vector4(0.f, 1.f / TanFOVRad, 0.f, 0.f), Vector4(0.f, 0.f, inFarPlane / Depth, 1.f), Vector4(0.f, 0.f, -inFarPlane * inNearPlane / Depth, 0.f));

	ViewProjectionMatrix = ViewMatrix * ProjectionMatrix;

	//Extract camera Rotation
	float rx, ry, rz;
	ry = -atan2f(ViewMatrix.M20, ViewMatrix.M22);
	rz = 0;
	if (ry < -MathHelpers::Pi || ry > MathHelpers::Pi)
	{
		rx = asinf(-ViewMatrix.M21);
	}
	else
	{
		rx = asinf(ViewMatrix.M21);
	}

	Rotation = Vector3(MathHelpers::RadiansToDegrees(rx), MathHelpers::RadiansToDegrees(ry), MathHelpers::RadiansToDegrees(rz));
}

void Camera::Rotate(const Vector3& inDeltaRotation)
{
	Rotation += inDeltaRotation;

	constexpr float RotationClamp = 89.9999f;
	Rotation.X = std::clamp(Rotation.X, -RotationClamp, RotationClamp);

	Direction = VectorHelpers::GetDirectionFromEulerDegrees(Rotation);

	bRefreshMatrices = true;
}

void Camera::Translate(const Vector3& inDeltaTranslation)
{
	Vector3 OrientedTranslationDelta = inDeltaTranslation;
	VectorHelpers::RotateDegrees(OrientedTranslationDelta, Rotation);
	Position += OrientedTranslationDelta;

	bRefreshMatrices = true;
}

void Camera::Update()
{
	if (bRefreshMatrices)
	{
		const Vector3 Right = Vector3::UnitY.Cross(Direction).GetNormalized();
		const Vector3 Up = Right.Cross(Direction).GetNormalized();
		ViewMatrix = Matrix44(Vector4(Right, 0.f), Vector4(Up, 0.f), Vector4(Direction, 0.f), Vector4(Position, 1.f));
		ViewProjectionMatrix = ViewMatrix * ProjectionMatrix;

		bRefreshMatrices = false;
	}
}