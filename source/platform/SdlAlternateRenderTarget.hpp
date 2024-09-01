#pragma once

#include "iAlternateRenderTarget.hpp"

struct SDL_Renderer;
struct SDL_Texture;

class SdlAlternateRenderTarget: public IAlternateRenderTarget
{
public:
	SdlAlternateRenderTarget(SDL_Renderer* renderer, SDL_Texture* render_target);
	virtual ~SdlAlternateRenderTarget();

private:
	SDL_Renderer* _renderer {nullptr};
};
