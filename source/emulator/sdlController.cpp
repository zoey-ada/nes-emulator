#include "sdlController.hpp"

#include <SDL_keyboard.h>

void SdlController::readInputs()
{
	auto state = SDL_GetKeyboardState(nullptr);

	uint8_t up_state = state[SDL_Scancode::SDL_SCANCODE_W] << 4;     // up
	uint8_t left_state = state[SDL_Scancode::SDL_SCANCODE_A] << 6;   // left
	uint8_t right_state = state[SDL_Scancode::SDL_SCANCODE_S] << 5;  // down
	uint8_t down_state = state[SDL_Scancode::SDL_SCANCODE_D] << 7;   // right

	uint8_t start_state = state[SDL_Scancode::SDL_SCANCODE_H] << 3;   // start
	uint8_t select_state = state[SDL_Scancode::SDL_SCANCODE_F] << 2;  // select
	uint8_t a_state = state[SDL_Scancode::SDL_SCANCODE_J] << 0;       // A
	uint8_t b_state = state[SDL_Scancode::SDL_SCANCODE_K] << 1;       // B

	// A, B, Select, Start, Up, Down, Left, Right.
	this->_data_latch = a_state | b_state | select_state | start_state | up_state | down_state |
		left_state | right_state;
}

bool SdlController::readAButtonInput()
{
	auto state = SDL_GetKeyboardState(nullptr);
	return state[SDL_Scancode::SDL_SCANCODE_J] == 1;  // A
}
