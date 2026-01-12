#pragma once

#include "core/drivers/vk/vk_core.h"
#include "gk/scene/scene.h"
#include "gk/immediate/immediate.h"

namespace juye{

class gkRenderer{
public:
  virtual int Init(void* pDisplayHandle) = 0;
  virtual void Destroy() = 0;
  virtual void DrawImmediateBuffer(const gkImmediateDrawList& buf) = 0;
  virtual void Dev() = 0;
  virtual void DevUpdate() = 0;

  virtual ~gkRenderer() = default;
};

class gkVulkanFrontend : public gkRenderer{
private:
  VK mBackend;
public:
  virtual int Init(void* pDisplayHandle)  override;
  virtual void Destroy() override;
  virtual void DrawImmediateBuffer(const gkImmediateDrawList& buf) override;
  virtual void Dev() override;
  virtual void DevUpdate() override;
};

}
