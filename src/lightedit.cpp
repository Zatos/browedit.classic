#include <common.h>
#include "graphics.h"
#include "undo.h"
#include "menu.h"
#include "menucommands.h"
#include "wm/lightwindow.h"

extern float mousestartx, mousestarty;
extern double mouse3dx, mouse3dy, mouse3dz;
extern cGraphics Graphics;
extern cUndoStack undostack;
extern bool	doneaction;
extern float oldmousex, oldmousey;
extern int movement;
extern bool lbuttondown;

extern cMenu* popupmenu;

double mouseclickx, mouseclicky, mouseclickz;

cProcessManagement::lightedit_process_events(SDL_Event &event)
{
	switch(event.type)
	{
		case SDL_MOUSEMOTION:
			if(lbuttondown)
			{
				if (Graphics.world.lights.size() == 0)
					break;
				int minobj = 0;
				float mindist = 999999;
				if(Graphics.objectstartdrag)
				{
					if(doneaction)
					{
						undostack.push(new cUndoChangeLight(Graphics.selectedobject));
						doneaction = false;
					}
					bool ctrl = (SDL_GetModState() & KMOD_CTRL) != 0;
					bool alt = (SDL_GetModState() & KMOD_ALT) != 0;
					if (!ctrl && !alt)
					{
						Graphics.world.lights[Graphics.selectedobject].pos.x = mouse3dx / 5;
						Graphics.world.lights[Graphics.selectedobject].pos.z = mouse3dz / 5;
						if (SDL_GetModState() & KMOD_SHIFT)
						{
							Graphics.world.lights[Graphics.selectedobject].pos.x = floor(Graphics.world.lights[Graphics.selectedobject].pos.x * (Graphics.gridsize/2.0f) + 0.5-Graphics.gridoffsetx) / (Graphics.gridsize/2.0f) + Graphics.gridoffsetx/(Graphics.gridsize/2.0f);
							Graphics.world.lights[Graphics.selectedobject].pos.z = floor(Graphics.world.lights[Graphics.selectedobject].pos.z * (Graphics.gridsize/2.0f) + 0.5-Graphics.gridoffsety) / (Graphics.gridsize/2.0f) + Graphics.gridoffsety/(Graphics.gridsize/2.0f);
						}
					}
					if(ctrl && !alt)
					{
						Graphics.world.lights[Graphics.selectedobject].pos.y += (mousey-oldmousey);
						if (SDL_GetModState() & KMOD_SHIFT)
						{
							Graphics.world.lights[Graphics.selectedobject].pos.y = floor(Graphics.world.lights[Graphics.selectedobject].pos.y * (Graphics.gridsize/2.0f) + 0.5-Graphics.gridoffsetx) / (Graphics.gridsize/2.0f) + Graphics.gridoffsetx/(Graphics.gridsize/2.0f);
						}
					}
					if(!ctrl && alt)
					{
						Graphics.world.lights[Graphics.selectedobject].range += (mousey-oldmousey);
					}
				}
			}
			break;
		case SDL_MOUSEBUTTONDOWN:
		{
			if(event.button.button == SDL_BUTTON_LEFT)
			{
				if(Graphics.world.lights.size() == 0)
					break;
				int minobj = 0;
				float mindist = 999999;
				for(int i = 0; i < Graphics.world.lights.size(); i++)
				{
					cVector3 d = Graphics.world.lights[i].pos;
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
				Graphics.objectstartdrag = Graphics.selectedobject == minobj;
			}
			break;
		}
		case SDL_MOUSEBUTTONUP:
			if(event.button.button == SDL_BUTTON_LEFT && movement < 3)
			{

				if (SDL_GetModState() & KMOD_CTRL)
				{
					cLight l;
					char buf[100];
					sprintf(buf, "obj%i", rand());
					l.name = buf;
					l.color.x = 0;
					l.color.y = 0;
					l.color.z = 0;
					l.pos = cVector3(mouse3dx/5, mouse3dy+10, mouse3dz/5);
					l.todo = string(buf, 40);
					l.todo2 = 192;
					l.maxlightincrement = 256;
					l.range = 200;
					l.lightfalloff = 1;

					Graphics.world.lights.push_back(l);
					undostack.push(new cUndoNewLight());
				}
				else
				{
					if (Graphics.world.lights.size() == 0)
						break;
					int minobj = 0;
					float mindist = 999999;
					for(int i = 0; i < Graphics.world.lights.size(); i++)
					{
						cVector3 d = Graphics.world.lights[i].pos;
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
					Graphics.selectedobject = minobj;
				}
			}
			else
			{
				if(movement < 3)
				{
					mouseclickx = mouse3dx;
					mouseclicky = mouse3dy;
					mouseclickz = mouse3dz;

					popupmenu = new cMenu();
					popupmenu->parent = NULL;
					popupmenu->drawstyle = 1;
					popupmenu->x = mousex;
					popupmenu->y = mousey;
					popupmenu->w = 150;
					popupmenu->opened = true;
					cMenuItem* mm;
					cMenu* favs;
					ADDMENUITEM(mm,popupmenu,"Deselect light",		&MenuCommand_new); //new
					ADDMENU(favs,		popupmenu, "Favorites",		popupmenu->x + 150,200); // File
					favs->y = popupmenu->y;
					favs->x = popupmenu->x + popupmenu->w;
					favs->w = 200;

					ADDMENUITEM(mm,popupmenu,"Disable Shadows",		&MenuCommand_new); //new
					ADDMENUITEM(mm,popupmenu,"Snap to floor",		&MenuCommand_new); //new
					ADDMENUITEM(mm,popupmenu,"Set to 50 over floor",		&MenuCommand_new); //new
					ADDMENUITEM(mm,popupmenu,"Set as sunlight",		&MenuCommand_new); //new

					ADDMENUITEM(mm,favs,"Torch",		&MenuCommand_addfavorite); //new
					ADDMENUITEM(mm,favs,"Spotlight",		&MenuCommand_addfavorite); //new
					ADDMENUITEM(mm,favs,"Disco Red",		&MenuCommand_addfavorite); //new
					ADDMENUITEM(mm,favs,"Disco Green",		&MenuCommand_addfavorite); //new
					ADDMENUITEM(mm,favs,"Disco Blue",		&MenuCommand_addfavorite); //new
					ADDMENUITEM(mm,favs,"Faint stuff",		&MenuCommand_addfavorite); //new
					ADDMENUITEM(mm,favs,"Random Colors",		&MenuCommand_addfavorite); //new
				}
			}
			break;
		case SDL_KEYDOWN:
		{
			switch (event.key.keysym.sym)
			{
			case SDLK_BACKSPACE:
				if (Graphics.selectedobject > -1 && Graphics.selectedobject < Graphics.world.lights.size())
				{
					undostack.push(new cUndoLightDelete(Graphics.selectedobject));
					Graphics.world.lights.erase(Graphics.world.lights.begin() + Graphics.selectedobject);
					Graphics.selectedobject = -1;
				}
				break;
			case SDLK_RETURN:
				if (Graphics.selectedobject != -1)
				{
					cLight* l = &Graphics.world.lights[Graphics.selectedobject];

					cWindow* w = new cLightWindow(&Graphics.WM.texture, &Graphics.WM.font);
					w->objects["posx"]->SetInt(3,(int)&l->pos.x);
					w->objects["posy"]->SetInt(3,(int)&l->pos.y);
					w->objects["posz"]->SetInt(3,(int)&l->pos.z);
					w->objects["colorr"]->SetInt(3,(int)&l->color.x);
					w->objects["colorg"]->SetInt(3,(int)&l->color.y);
					w->objects["colorb"]->SetInt(3,(int)&l->color.z);
					w->objects["intensity"]->SetInt(3,(int)&l->todo2);
					w->objects["range"]->SetInt(3,(int)&l->range);
					w->objects["maxlightincrement"]->SetInt(3,(int)&l->maxlightincrement);
					w->objects["lightfalloff"]->SetInt(3,(int)&l->lightfalloff);
					w->objects["castshadow"]->SetInt(3,(int)&l->givesshadow);
					//((cEffectWindow*)w)->undo = new cUndoChangeEffect(Graphics.selectedobject);
					Graphics.WM.addwindow(w);
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