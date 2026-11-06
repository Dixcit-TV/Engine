#pragma once

struct Viewport
{
	unsigned int Top = 0;
	unsigned int Left = 0;
	unsigned int Width = 0;
	unsigned int Height = 0;
	float MinDepth = 0.f;
	float MaxDepth = 0.f;

	bool IsValid() const
	{
		return Width > 0 && Height > 0
			&& MaxDepth > MinDepth;
	}
};