#pragma once
#include "../Math/Vector.h"
#include "Actor.h"
#include <cassert>

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

private:
	Vector3 Position;
	Vector3 Rotation;
	Vector3 Scale;
};

class Tickable
{
public:
	virtual void Tick(double inDeltaTime) {}

	void SetTickEnabled(bool inEnabled);

private:
	explicit Tickable() : bIsTickEnabled(false) {}

	bool bIsTickEnabled;
};

class Transformable
{
public:
	void SetTransform(const Transform& inTransform) { TransformData = inTransform; }
	const Transform& GetTransform() const { return TransformData; }

protected:
	explicit Transformable() : TransformData() {}

	Transform TransformData;
};

class Component
{
public:
	virtual ~Component() = default;
	virtual void Initialize() {}
	virtual void Deinitialize() {}

	inline void SetOwner(Actor* inOwner) 
	{ 
		assert(inOwner && "Cannot set owner for component has it already has one.");
		Owner = inOwner;
		Owner->AddComponent(this);
	}

protected:
	explicit Component() : Owner(nullptr) {}

	Actor* Owner;
};

class TickComponent : public Component, public Tickable {};
class TransformComponent : public Component, public Transformable {};
class TickTransformComponent : public Component, public Tickable, public Transformable {};

