#pragma once
#include "D2DGameEngine/Level.h"
#include "D2DGameEngine/SpriteResource.h"

class LoadingLevel : public Level
{
public:
    LoadingLevel(class World* _world, const std::wstring& _name);
    virtual ~LoadingLevel();

    virtual void Enter() override;
    virtual void Exit();
    virtual void Clear();

    virtual void FixedUpdate(float _fixedRate);
    virtual void PreUpdate(float _dt);
    virtual void Update(float _dt);
    virtual void PostUpdate(float _dt);
    virtual void Render(class D2DRenderer* _renderer) override;

    bool LoadImages(const wchar_t* szPath);
    bool LoadSound();

private:
    std::vector<std::shared_ptr<SpriteResource>> loadingResource;
};
