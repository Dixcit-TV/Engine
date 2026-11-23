#include "RHIInterface.h"
#include "../App/Viewport.h"
#include "../App/WindowInterface.h"
#include <cassert>

#include "DX12RHI/DX12RHI.h"

std::unique_ptr<RHIInterface> RHIInterface::Create(RHIType inType, const Viewport& inViewport, const std::unique_ptr<WindowInterface>& inWindow)
{
    assert(!bHasInstance && "Cannot create more than one RHIInterface.");
    switch (inType)
    {
    case RHIType::DX12:
        return std::make_unique<DX12RHI>(inViewport, inWindow);
    default:
        assert(false);
        return nullptr;
    }
}