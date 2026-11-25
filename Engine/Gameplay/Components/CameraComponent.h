#pragma once
#include "../../Math/Vector.h"
#include "../../Math/Matrix.h"
#include "../Component.h"

class CameraRenderStates;

class CameraComponent : public RenderTransformComponent
{
public:
	explicit CameraComponent() : FOVdeg(0.f), AspectRatio(0.f), NearPlane(0.f), FarPlane(1.f), RenderStates(nullptr) {}
	explicit CameraComponent(const Vector3& inPosition, const Vector3& inDirection, float inFOVdeg, float inAspectRatio, float inNearPlane, float inFarPlane);

	void Deinitialize() override;

	virtual void CreateRenderState() override;
	virtual void DestroyRenderState() override;

	void SetFOV(float inFOVdeg) { FOVdeg = inFOVdeg; MarkRenderStateDirty(); }
	void SetAspectRatio(float inAspectRatio) { AspectRatio = inAspectRatio; MarkRenderStateDirty(); }
	void SetNearPlane(float inNearPlane) { NearPlane = inNearPlane; MarkRenderStateDirty(); }
	void SetFarPlane(float inFarPlane) { FarPlane = inFarPlane; MarkRenderStateDirty(); }

	float GetFOV() const { return FOVdeg; }
	float GetAspectRatio() const { return AspectRatio; }
	float GetNearPlane() const { return NearPlane; }
	float GetFarPlane() const { return FarPlane; }

	void Rotate(const Vector3& DeltaRotation);
	void Translate(const Vector3& DeltaTranslation);

private:
	float FOVdeg;
	float AspectRatio;
	float NearPlane;
	float FarPlane;

	CameraRenderStates* RenderStates;
};