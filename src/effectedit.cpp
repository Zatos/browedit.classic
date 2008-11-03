#include <common.h>
#include "graphics.h"
#include "undo.h"
#include "menu.h"
#include "windows/effectwindow.h"

extern long mousestartx, mousestarty;
extern double mouse3dx, mouse3dy, mouse3dz;
extern cGraphics Graphics;
extern cUndoStack undostack;
extern float oldmousex, oldmousey;
extern int movement;
extern cMenuItem* selectedeffect;
extern cMenu* effectsmenu;
extern bool lbuttondown;

int cProcessManagement::effectedit_process_events(SDL_Event &event)
{
	switch(event.type)
	{
		case SDL_MOUSEMOTION:
			if(lbuttondown)
			{
				if (cGraphics::world->effects.size() == 0)
					break;
				if(Graphics.objectStartDrag)
				{
					bool ctrl = (SDL_GetModState() & KMOD_CTRL) != 0;
					bool alt = (SDL_GetModState() & KMOD_ALT) != 0;
					if (!ctrl && !alt)
					{
						cGraphics::world->effects[Graphics.selectedObject].pos.x = mouse3dx / 5.0f;
						cGraphics::world->effects[Graphics.selectedObject].pos.z = mouse3dz / 5.0f;
						if (SDL_GetModState() & KMOD_SHIFT)
						{
							cGraphics::world->effects[Graphics.selectedObject].pos.x = floor(cGraphics::world->effects[Graphics.selectedObject].pos.x * (Graphics.gridsize/2.0f) + 0.5-Graphics.gridoffsetx) / (Graphics.gridsize/2.0f) + Graphics.gridoffsetx/(Graphics.gridsize/2.0f);
							cGraphics::world->effects[Graphics.selectedObject].pos.z = floor(cGraphics::world->effects[Graphics.selectedObject].pos.z * (Graphics.gridsize/2.0f) + 0.5-Graphics.gridoffsety) / (Graphics.gridsize/2.0f) + Graphics.gridoffsety/(Graphics.gridsize/2.0f);
						}
					}
					if(ctrl && !alt)
					{
						cGraphics::world->effects[Graphics.selectedObject].pos.y += (mouseY-oldmousey);
						if (SDL_GetModState() & KMOD_SHIFT)
						{
							cGraphics::world->effects[Graphics.selectedObject].pos.y = floor(cGraphics::world->effects[Graphics.selectedObject].pos.y * (Graphics.gridsize/2.0f) + 0.5-Graphics.gridoffsetx) / (Graphics.gridsize/2.0f) + Graphics.gridoffsetx/(Graphics.gridsize/2.0f);
						}
					}
					if(!ctrl && alt)
					{
					}
				}
			}
			break;
		case SDL_MOUSEBUTTONDOWN:
			if(event.button.button == SDL_BUTTON_LEFT)
			{
				if(cGraphics::world->effects.size() == 0)
					break;
				int minobj = 0;
				float mindist = 999999;
				for(unsigned int i = 0; i < cGraphics::world->effects.size(); i++)
				{
					cVector3 d = cGraphics::world->effects[i].pos;
					d.x = d.x;
					
					d.x -= mouse3dx/5;
					d.z -= mouse3dz/5;
					d.y = 0;

					if(mindist > d.Magnitude())
					{
						mindist = d.Magnitude();
						minobj = i;
					}
				}
				Graphics.objectStartDrag = Graphics.selectedObject == minobj;
			}
			break;
		case SDL_MOUSEBUTTONUP:
			if(event.button.button == SDL_BUTTON_LEFT && movement < 3)
			{
				if (SDL_GetModState() & KMOD_CTRL)
				{
					if (selectedeffect == NULL)
						break;
					cEffect e;
					char buf[100];
					sprintf(buf, "obj%i", rand());
					e.category = "\0\0\0\0";
					e.loop = 40;
					e.pos = cVector3(mouse3dx/5, -mouse3dy-10, mouse3dz/5);
					e.name = buf;
					e.readablename = selectedeffect->title;
					e.type = atoi(selectedeffect->data.c_str());
					e.todo1 = 1;
					e.todo2 = 1;
					e.todo3 = 1;
					e.rotation = cVector3(0,0,0);
					e.scale = cVector3(1,1,1);
					e.todo10 = 1; // seems to be linked to 11
					e.todo11 = 1; // seems to be linked to 10
					e.todo12 = 0; // seems to be always 0
					e.todo13 = 0; // seems to be always 0

					cGraphics::world->effects.push_back(e);
				}
				else
				{
					if (cGraphics::world->effects.size() == 0)
						break;
					int minobj = 0;
					float mindist = 999999;
					for(unsigned int i = 0; i < cGraphics::world->effects.size(); i++)
					{
						cVector3 d = cGraphics::world->effects[i].pos;
						d.x = d.x;
						
						d.x -= mouse3dx/5.0f;
						d.z -= mouse3dz/5.0f;
						d.y = 0;

						if(mindist > d.Magnitude())
						{
							mindist = d.Magnitude();
							minobj = i;
						}
					}
					Graphics.selectedObject = minobj;
					char buf[100];
					sprintf(buf, "%i", cGraphics::world->effects[Graphics.selectedObject].type);
				}
			}
			break;
		case SDL_KEYDOWN:
		{
			switch (event.key.keysym.sym)
			{
			case SDLK_PAGEDOWN:
				if (Graphics.selectedObject != -1)
				{
					undostack.push(new cUndoChangeEffect(Graphics.selectedObject));
					cGraphics::world->effects[Graphics.selectedObject].loop--;
				}
				break;
			case SDLK_PAGEUP:
				if (Graphics.selectedObject != -1)
				{
					undostack.push(new cUndoChangeEffect(Graphics.selectedObject));
					cGraphics::world->effects[Graphics.selectedObject].loop++;
				}
				break;
			case SDLK_BACKSPACE:
				if (Graphics.selectedObject > -1 && Graphics.selectedObject < (int)cGraphics::world->effects.size())
				{
					undostack.push(new cUndoEffectDelete(Graphics.selectedObject));
					cGraphics::world->effects.erase(cGraphics::world->effects.begin() + Graphics.selectedObject);
					Graphics.selectedObject = -1;
				}
				break;
			case SDLK_RETURN:
				if (Graphics.selectedObject != -1)
				{
					cEffect* o = &cGraphics::world->effects[Graphics.selectedObject];

					cWindow* w = new cEffectWindow();
					((cWindowFloatInputBox*)w->objects["posx"])->floatje = &o->pos.x;
					((cWindowFloatInputBox*)w->objects["posy"])->floatje = &o->pos.y;
					((cWindowFloatInputBox*)w->objects["posz"])->floatje = &o->pos.z;
					((cWindowFloatInputBox*)w->objects["rotx"])->floatje = &o->rotation.x;
					((cWindowFloatInputBox*)w->objects["roty"])->floatje = &o->rotation.y;
					((cWindowFloatInputBox*)w->objects["rotz"])->floatje = &o->rotation.z;
					((cWindowFloatInputBox*)w->objects["scalex"])->floatje = &o->scale.x;
					((cWindowFloatInputBox*)w->objects["scaley"])->floatje = &o->scale.y;
					((cWindowFloatInputBox*)w->objects["scalez"])->floatje = &o->scale.z;
					((cWindowFloatInputBox*)w->objects["looptime"])->floatje = &o->loop;

					((cWindowStringInputBox*)w->objects["objectname"])->stringetje = &o->readablename;
					((cEffectWindow*)w)->undo = new cUndoChangeEffect(Graphics.selectedObject);
					cWM::addWindow(w);
				}
				break;

			default:
				break;
			}

			break;
		}
		
			
	}



	return true;


}
