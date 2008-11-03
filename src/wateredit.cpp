#include <common.h>
#include "graphics.h"
#include "undo.h"

extern long mousestartx, mousestarty;
extern double mouse3dx, mouse3dy, mouse3dz;
extern cGraphics Graphics;
extern cUndoStack undostack;


int cProcessManagement::wateredit_process_events(SDL_Event &event)
{
	switch(event.type)
	{
		case SDL_MOUSEMOTION:
			break;
		case SDL_MOUSEBUTTONDOWN:
		{
			break;
		}
		case SDL_KEYDOWN:
		{
			switch (event.key.keysym.sym)
			{
			case SDLK_LEFTBRACKET:
				cGraphics::world->water.type = max(0, cGraphics::world->water.type - 1);
				break;
			case SDLK_RIGHTBRACKET:
				cGraphics::world->water.type = min(Graphics.waterCount-1, cGraphics::world->water.type + 1);
				break;
			case SDLK_PAGEDOWN:
				undostack.push(new cUndoChangeWater(cGraphics::world->water));
				cGraphics::world->water.height++;
				break;
			case SDLK_PAGEUP:
				undostack.push(new cUndoChangeWater(cGraphics::world->water));
				cGraphics::world->water.height--;
				break;
			default:
				break;
			}

			break;
		}
		
			
	}



	return true;


}
