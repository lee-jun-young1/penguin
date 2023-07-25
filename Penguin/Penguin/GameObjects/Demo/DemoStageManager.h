#pragma once
#include "StageManager.h"
class DemoStageManager :
    public StageManager
{
public:
    DemoStageManager(const std::string& name = "DemoStageManager");
    virtual void Init() override;
    virtual void Update(float dt) override;
    virtual void CreateObj() override;
};

