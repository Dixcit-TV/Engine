#pragma once

class RHIInterface
{
public:
	virtual ~RHIInterface() = default;

	virtual void FlushCommandQueue() = 0;
	virtual void ExecuteCommandList() = 0;
	virtual void StartFrame() = 0;
	virtual void EndFrame() = 0;
};