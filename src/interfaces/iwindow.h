#pragma once

class IWindow
{
public:
    virtual ~IWindow() = default;
    virtual void* GetMonitorFromPoint() = 0;
};