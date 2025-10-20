#pragma once

class RenderingBackend
{

public:
  virtual ~RenderingBackend() = default;
  virtual void HelloTriangle() = 0;
};