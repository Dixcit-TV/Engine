#pragma once
#include "../../Math/Vector.h"
#include "../../Math/Matrix.h"
#include "../Component.h"

class CameraComponent : public RenderTransformComponent
{
public:
	explicit CameraComponent() : FOVdeg(0.f), AspectRatio(0.f), NearPlane(0.f), FarPlane(1.f) {}
	explicit CameraComponent(const Vector3& inPosition, const Vector3& inDirection, float inFOVdeg, float inAspectRatio, float inNearPlane, float inFarPlane);

	virtual void CreateRenderState() override {}
	virtual void DestroyRenderState() override {}

	void SetFOV(float inFOVdeg) { FOVdeg = inFOVdeg; MarkRenderStateDirty(); }
	void SetAspectRatio(float inAspectRatio) { AspectRatio = inAspectRatio; MarkRenderStateDirty(); }
	void SetNearPlane(float inNearPlane) { NearPlane = inNearPlane; MarkRenderStateDirty(); }
	void SetFarPlane(float inFarPlane) { FarPlane = inFarPlane; MarkRenderStateDirty(); }

	//Matrix44 GetViewMatrix() const { return ViewMatrix; }
	//Matrix44 GetProjectionMatrix() const { return ProjectionMatrix; }
	//Matrix44 GetViewProjMatrix() const { return ViewProjectionMatrix; }

	void Rotate(const Vector3& DeltaRotation);
	void Translate(const Vector3& DeltaTranslation);

private:
	//Matrix44 ViewMatrix;
	//Matrix44 ProjectionMatrix;
	//Matrix44 ViewProjectionMatrix;
	float FOVdeg;
	float AspectRatio;
	float NearPlane;
	float FarPlane;
};