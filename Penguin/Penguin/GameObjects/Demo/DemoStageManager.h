#pragma once
#include "StageManager.h"
class DemoStageManager :
    public StageManager
{
public:
    DemoStageManager(const std::string& name = "DemoStageManager");
    virtual void Init() override;
    virtual void IncreaseScore(const int& score) override;
    virtual void IncreaseScore(const ScoreItemType& type) override;
    virtual void ResetScore() override;
};

