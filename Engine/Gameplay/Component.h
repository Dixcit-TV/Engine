#pragma once
#include <cassert>
#include "../Math/Vector.h"
#include "../Math/Matrix.h"
#include "../Math/MathHelpers.h"

class Actor;

struct Transform
{
	Transform(Vector3 inPosition = Vector3::Zero, Vector3 inRotation = Vector3::Zero, Vector3 inScale = Vector3::One) 
		: Position(inPosition), Rotation(inRotation), Scale(inScale) {}

	inline void SetPosition(const Vector3& inPosition) { Position = inPosition; }
	inline void SetRotation(const Vector3& inRotation) { Rotation = inRotation; }
	inline void SetScale(const Vector3& inScale) { Scale = inScale; }

	inline const Vector3& GetPosition() const { return Position; }
	inline const Vector3& GetRotation() const { return Rotation; }
	inline const Vector3& GetScale() const { return Scale; }

	inline void Translate(const Vector3& inTranslation) { Position += inTranslation; }
	inline void Rotate(const Vector3& inRotation) { Rotation += inRotation; }
	inline void ScaleBy(const Vector3& inScale) { Scale *= inScale; }

	inline Matrix44 ToMatrix() const
	{
		Matrix44 World = Matrix44::Identity;
		MatrixHelpers::RotateDegrees(World, Rotation);
		World.V0 *= Scale.X;
		World.V1 *= Scale.Y;
		World.V2 *= Scale.Z;
		World.V3 = Vector4(Position, 1.f);
		return World;
	}

	inline Matrix44 ToMatrixNoScale() const
	{
		Matrix44 World = Matrix44::Identity;
		MatrixHelpers::RotateDegrees(World, Rotation);
		World.V3 = Vector4(Position, 1.f);
		return World;
	}

private:
	Vector3 Position;
	Vector3 Rotation;
	Vector3 Scale;
};

class Tickable
{
public:
	virtual ~Tickable();

	virtual void Tick(double inDeltaTime) = 0;

	void SetTickEnabled(bool inEnabled);

protected:
	explicit Tickable() : bIsTickEnabled(false) {}

	bool bIsTickEnabled;
};

class Transformable
{
public:
	virtual ~Transformable() = default;

	void SetTransform(const Transform& inTransform) { TransformData = inTransform; }
	void SetTransform(Vector3 inPosition = Vector3::Zero, Vector3 inRotation = Vector3::Zero, Vector3 inScale = Vector3::One) { TransformData = Transform(inPosition, inRotation, inScale); }
	const Transform& GetTransform() const { return TransformData; }

protected:
	explicit Transformable() : TransformData() {}

	Transform TransformData;
};

class Renderable
{
public:
	virtual ~Renderable();

	virtual void CreateRenderState() = 0;
	virtual void DestroyRenderState() = 0;

	void MarkRenderStateDirty();
	void MarkRenderStateValid() { bIsDirty = false; }

protected:
	explicit Renderable() : bIsDirty(true) {}

private:
	bool bIsDirty;
};

class Component
{
public:
	virtual ~Component() = default;
	virtual void Initialize() {}
	virtual void Deinitialize() {}

	inline void SetOwner(Actor* inOwner) 
	{ 
		assert(!Owner && inOwner && "Cannot set owner for component has it already has one.");
		Owner = inOwner;
	}

protected:
	explicit Component() : Owner(nullptr) {}

	Actor* Owner;
};

class TickComponent : public Component, public Tickable {};
class TransformComponent : public Component, public Transformable {};
class RenderComponent : public Component, public Renderable {};
class TickTransformComponent : public Component, public Tickable, public Transformable {};
class RenderTickComponent : public Component, public Tickable, public Renderable {};
class RenderTransformComponent : public Component, public Transformable, public Renderable {};
class RenderTickTransComponent : public Component, public Tickable, public Transformable, public Renderable {};

