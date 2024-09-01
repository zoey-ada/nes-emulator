#include "SdlAlternateRenderTarget.hpp"

#include <SDL.h>

SdlAlternateRenderTarget::SdlAlternateRenderTarget(SDL_Renderer* renderer,
	SDL_Texture* render_target)
	: _renderer(renderer)
{
	SDL_SetRenderTarget(this->_renderer, render_target);
}

SdlAlternateRenderTarget::~SdlAlternateRenderTarget()
{
	SDL_SetRenderTarget(this->_renderer, nullptr);
}
