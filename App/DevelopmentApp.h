#pragma once
#include "App/Application.h"

class DevelopmentApp final : public Application
{
public:
	explicit DevelopmentApp();

protected:
	void Initialize() override;
	void Tick(double inDeltaTime) override;
	void Shutdown() override;
};

