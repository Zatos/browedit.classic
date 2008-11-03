#include <common.h>
#include "graphics.h"
#include "undo.h"
#include "menu.h"
#include <wm/windowinputbox.h>
#include "windows/objectwindow.h"
#include "windows/modeloverviewwindow.h"

#define MENUCOMMAND(x) bool MenuCommand_ ## x (cMenuItem* src)
MENUCOMMAND(model);

extern long mousestartx, mousestarty;
extern double mouse3dx, mouse3dy, mouse3dz;
extern cGraphics Graphics;
extern cUndoStack undostack;
extern bool lbuttondown,rbuttondown;
extern bool	doneaction;
extern cMenu* snaptofloor;
extern float oldmousex, oldmousey;
extern int movement;
extern cMenu* currentobject;
extern cMenu* models;

int cProcessManagement::objectedit_process_events(SDL_Event &event)
{
	switch(event.type)
	{
		case SDL_MOUSEMOTION:
			if (lbuttondown && !rbuttondown)
			{
				if(Graphics.objectStartDrag && Graphics.selectedObject != -1)
				{
					if (doneaction)
					{
						undostack.push(new cUndoChangeObject(Graphics.selectedObject));
						doneaction = false;
					}
					bool ctrl = (SDL_GetModState() & KMOD_CTRL) != 0;
					bool alt = (SDL_GetModState() & KMOD_ALT) != 0;
					if (!ctrl && !alt)
					{
						cGraphics::world->models[Graphics.selectedObject]->pos.x = mouse3dx / 5;
						if(snaptofloor->ticked)
							cGraphics::world->models[Graphics.selectedObject]->pos.y = -mouse3dy;
						cGraphics::world->models[Graphics.selectedObject]->pos.z = mouse3dz / 5;
						if (SDL_GetModState() & KMOD_SHIFT)
						{
							cGraphics::world->models[Graphics.selectedObject]->pos.x = floor(cGraphics::world->models[Graphics.selectedObject]->pos.x * (Graphics.gridsize/2.0f) + 0.5-Graphics.gridoffsetx) / (Graphics.gridsize/2.0f) + Graphics.gridoffsetx/(Graphics.gridsize/2.0f);
							cGraphics::world->models[Graphics.selectedObject]->pos.z = floor(cGraphics::world->models[Graphics.selectedObject]->pos.z * (Graphics.gridsize/2.0f) + 0.5-Graphics.gridoffsety) / (Graphics.gridsize/2.0f) + Graphics.gridoffsety/(Graphics.gridsize/2.0f);
						}
					}
					if(ctrl && !alt)
					{
						cGraphics::world->models[Graphics.selectedObject]->rot.x += mouseY - oldmousey;
						cGraphics::world->models[Graphics.selectedObject]->rot.y += mouseX - oldmousex;
						if (SDL_GetModState() & KMOD_SHIFT)
						{

							cGraphics::world->models[Graphics.selectedObject]->rot.x = floor((cGraphics::world->models[Graphics.selectedObject]->rot.x+22.5) / 45) * 45;
							cGraphics::world->models[Graphics.selectedObject]->rot.y = floor((cGraphics::world->models[Graphics.selectedObject]->rot.y+22.5) / 45) * 45;
						}
					}
					if(!ctrl && alt)
					{
 						cGraphics::world->models[Graphics.selectedObject]->scale.x += (mouseX - oldmousex)/10.0;
						cGraphics::world->models[Graphics.selectedObject]->scale.y += (mouseX - oldmousex)/10.0;
						cGraphics::world->models[Graphics.selectedObject]->scale.z += (mouseX - oldmousex)/10.0;
					}
				}
			}
			break;
		case SDL_MOUSEBUTTONDOWN:
			if(event.button.button == SDL_BUTTON_LEFT)
			{
				int minobj = 0;
				float mindist = 999999;
				for(unsigned int i = 0; i < cGraphics::world->models.size(); i++)
				{
					cVector3 d = cGraphics::world->models[i]->pos;
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
				if (SDL_GetModState() & KMOD_CTRL && Graphics.previewModel != NULL)
				{
					cRSMModel* model = new cRSMModel();
					model->load(Graphics.previewModel->filename);
					model->pos = cVector3(mouse3dx/5, -mouse3dy, mouse3dz/5);
					model->scale = cVector3(1,1,1);
					model->rot = cVector3(0,0,0);
					model->name = "Object" + inttostring(rand()%1000);
					model->lightopacity = 1;
					char buf[100];
					sprintf(buf, "%s-%i", Graphics.previewModel->rofilename.c_str(), rand()%100);
					cGraphics::world->models.push_back(model);
					Graphics.selectedObject = cGraphics::world->models.size()-1;
					undostack.push(new cUndoNewObject());
				}
				else
				{
					int minobj = 0;
					float mindist = 999999;
					for(unsigned int i = 0; i < cGraphics::world->models.size(); i++)
					{
						cVector3 d = cGraphics::world->models[i]->pos;
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
					Graphics.selectedObject = minobj;
				}
				cWindow* w = cWM::getWindow(WT_MODELOVERVIEW);
				if(w != NULL)
				{
					w->userfunc(NULL);
					if(Graphics.selectedObject >= 0 && Graphics.selectedObject < (int)cGraphics::world->models.size()-1)
					{
						cModelOverViewWindow::cModelOverViewTree* tree = (cModelOverViewWindow::cModelOverViewTree*)w->objects["list"];
						tree->getObject(cGraphics::world->models[Graphics.selectedObject]);
					}
				}
			}
		case SDL_KEYDOWN:
		{
			switch (event.key.keysym.sym)
			{
			case SDLK_UP:
				if (Graphics.selectedObject > -1 && Graphics.selectedObject < (int)cGraphics::world->models.size())
				{
					undostack.push(new cUndoChangeObject(Graphics.selectedObject));
					bool ctrl = (SDL_GetModState() & KMOD_CTRL) != 0;
					bool alt = (SDL_GetModState() & KMOD_ALT) != 0;
					bool shift = (SDL_GetModState() & KMOD_SHIFT) != 0;
					if (!ctrl && !alt)
						cGraphics::world->models[Graphics.selectedObject]->pos.z+=0.1 + (shift ? 0.4 : 0);
					if (ctrl && !alt)
						cGraphics::world->models[Graphics.selectedObject]->rot.z+=1 + (shift ? 44 : 0);
					if (!ctrl && alt)
						cGraphics::world->models[Graphics.selectedObject]->scale.z+=0.1 + shift ? 0.1 : 0;
				}						
				break;
			case SDLK_DOWN:
				if (Graphics.selectedObject > -1 && Graphics.selectedObject < (int)cGraphics::world->models.size())
				{
					undostack.push(new cUndoChangeObject(Graphics.selectedObject));
					bool ctrl = (SDL_GetModState() & KMOD_CTRL) != 0;
					bool alt = (SDL_GetModState() & KMOD_ALT) != 0;
					bool shift = (SDL_GetModState() & KMOD_SHIFT) != 0;
					if (!ctrl && !alt)
						cGraphics::world->models[Graphics.selectedObject]->pos.z-=0.1 + (shift ? 0.4 : 0);
					if (ctrl && !alt)
						cGraphics::world->models[Graphics.selectedObject]->rot.z-=1 + (shift ? 44 : 0);
					if (!ctrl && alt)
						cGraphics::world->models[Graphics.selectedObject]->scale.z-=0.1 + shift ? 0.1 : 0;
				}
				break;
			case SDLK_RIGHT:
				if (Graphics.selectedObject > -1 && Graphics.selectedObject < (int)cGraphics::world->models.size())
				{
					undostack.push(new cUndoChangeObject(Graphics.selectedObject));
					bool ctrl = (SDL_GetModState() & KMOD_CTRL) != 0;
					bool alt = (SDL_GetModState() & KMOD_ALT) != 0;
					bool shift = (SDL_GetModState() & KMOD_SHIFT) != 0;
					if (!ctrl && !alt)
						cGraphics::world->models[Graphics.selectedObject]->pos.x+=0.1 + (shift ? 0.4 : 0);
					if (ctrl && !alt)
						cGraphics::world->models[Graphics.selectedObject]->rot.x+=1 + (shift ? 44 : 0);
					if (!ctrl && alt)
						cGraphics::world->models[Graphics.selectedObject]->scale.x+=0.1 + shift ? 0.1 : 0;
				}						
				break;
			case SDLK_LEFT:
				if (Graphics.selectedObject > -1 && Graphics.selectedObject < (int)cGraphics::world->models.size())
				{
					undostack.push(new cUndoChangeObject(Graphics.selectedObject));
					bool ctrl = (SDL_GetModState() & KMOD_CTRL) != 0;
					bool alt = (SDL_GetModState() & KMOD_ALT) != 0;
					bool shift = (SDL_GetModState() & KMOD_SHIFT) != 0;
					if (!ctrl && !alt)
						cGraphics::world->models[Graphics.selectedObject]->pos.x-=0.1 + (shift ? 0.4 : 0);
					if (ctrl && !alt)
						cGraphics::world->models[Graphics.selectedObject]->rot.x-=1 + (shift ? 44 : 0);
					if (!ctrl && alt)
						cGraphics::world->models[Graphics.selectedObject]->scale.x-=0.1 + shift ? 0.1 : 0;
				}
				break;
			case SDLK_MINUS:
				if (Graphics.previewModel != NULL)
					Graphics.previewModel->scale = Graphics.previewModel->scale * 0.9f;
				break;
			case SDLK_EQUALS:
				if (Graphics.previewModel != NULL)
					Graphics.previewModel->scale = Graphics.previewModel->scale * 1.1f;
				break;
			case SDLK_LEFTBRACKET:
				if (currentobject != NULL)
				{
					currentobject = currentobject->parent->getPrev(currentobject);
					MenuCommand_model((cMenuItem*)currentobject);
				}
				break;
			case SDLK_RIGHTBRACKET:
				if (currentobject != NULL)
				{
					currentobject = currentobject->parent->getNext(currentobject);
					MenuCommand_model((cMenuItem*)currentobject);
				}
				break;
			case SDLK_h:
				if (Graphics.selectedObject != -1)
				{
					undostack.push(new cUndoChangeObject(Graphics.selectedObject));
					if(SDL_GetModState() & KMOD_ALT)
						cGraphics::world->models[Graphics.selectedObject]->scale.y = -	cGraphics::world->models[Graphics.selectedObject]->scale.y;
					else
						cGraphics::world->models[Graphics.selectedObject]->scale.x = -	cGraphics::world->models[Graphics.selectedObject]->scale.x;
				}
				break;
			case SDLK_v:
				if (Graphics.selectedObject != -1)
				{
					undostack.push(new cUndoChangeObject(Graphics.selectedObject));
					cGraphics::world->models[Graphics.selectedObject]->scale.z = -	cGraphics::world->models[Graphics.selectedObject]->scale.z;
				}
				break;
			case SDLK_PAGEDOWN:
				if (Graphics.selectedObject > -1 && Graphics.selectedObject < (int)cGraphics::world->models.size())
				{
					undostack.push(new cUndoChangeObject(Graphics.selectedObject));
					bool ctrl = (SDL_GetModState() & KMOD_CTRL) != 0;
					bool alt = (SDL_GetModState() & KMOD_ALT) != 0;
					bool shift = (SDL_GetModState() & KMOD_SHIFT) != 0;
					if (!ctrl && !alt)
						cGraphics::world->models[Graphics.selectedObject]->pos.y+=0.1 + (shift ? 0.4 : 0);
					if (ctrl && !alt)
						cGraphics::world->models[Graphics.selectedObject]->rot.y+=1 + (shift ? 44 : 0);
					if (!ctrl && alt)
						cGraphics::world->models[Graphics.selectedObject]->scale.y+=0.1 + shift ? 0.1 : 0;
				}						
				break;
			case SDLK_PAGEUP:
				if (Graphics.selectedObject > -1 && Graphics.selectedObject < (int)cGraphics::world->models.size())
				{
					undostack.push(new cUndoChangeObject(Graphics.selectedObject));
					bool ctrl = (SDL_GetModState() & KMOD_CTRL) != 0;
					bool alt = (SDL_GetModState() & KMOD_ALT) != 0;
					bool shift = (SDL_GetModState() & KMOD_SHIFT) != 0;
					if (!ctrl && !alt)
						cGraphics::world->models[Graphics.selectedObject]->pos.y-=0.1 + (shift ? 0.4 : 0);
					if (ctrl && !alt)
						cGraphics::world->models[Graphics.selectedObject]->rot.y-=1 + (shift ? 44 : 0);
					if (!ctrl && alt)
						cGraphics::world->models[Graphics.selectedObject]->scale.y-=0.1 + shift ? 0.1 : 0;
				}
				break;
			case SDLK_c:
				if (Graphics.selectedObject != -1)
				{
					Graphics.clipboardRot = cGraphics::world->models[Graphics.selectedObject]->rot;
					Graphics.clipboardScale = cGraphics::world->models[Graphics.selectedObject]->scale;
					Graphics.clipboardFile = cGraphics::world->models[Graphics.selectedObject]->filename;
					Graphics.clipboardY = cGraphics::world->models[Graphics.selectedObject]->pos.y;
					Graphics.clipboardName = cGraphics::world->models[Graphics.selectedObject]->name;
					Graphics.clipboardFloat = cGraphics::world->models[Graphics.selectedObject]->lightopacity;
					currentobject = models->findData("data\\model\\" + cGraphics::world->models[Graphics.selectedObject]->rofilename);
					if(currentobject != NULL)
						MenuCommand_model((cMenuItem*)currentobject);
				}
				break;
			case SDLK_p:
				if (Graphics.clipboardFile != "")
				{
					if (SDL_GetModState() & KMOD_CTRL)
					{
						cGraphics::world->models[Graphics.selectedObject]->pos.y = Graphics.clipboardY;
					}
					else
					{
						cRSMModel* model = new cRSMModel();
						model->load(Graphics.clipboardFile);
						model->pos = cVector3(mouse3dx/5.0f, -mouse3dy, mouse3dz/5.0f);
						if (SDL_GetModState() & KMOD_SHIFT)
							model->pos.y = Graphics.clipboardY;
						model->scale = Graphics.clipboardScale;
						model->rot = Graphics.clipboardRot;
						model->lightopacity = Graphics.clipboardFloat;

						model->name = Graphics.clipboardName;
						if(model->name != "")
						{
							int i = model->name.length()-1;
							while((atoi(model->name.substr(i).c_str()) != 0 || model->name.substr(i,1) == "0") && i > 0)
								i--;

							char buf[100];
							int newid = atoi(model->name.substr(i+1).c_str());

							bool found = true;

							while(found)
							{
								newid++;
								found = false;
								sprintf(buf, "%s%i", model->name.substr(0,i+1).c_str(), newid);

								for(int ii = 0; ii < cGraphics::world->models.size() && !found; ii++)
								{
									if(cGraphics::world->models[ii]->name == buf)
										found = true;
								}
							}
							model->name = buf;
						}

						cGraphics::world->models.push_back(model);
						Graphics.selectedObject = cGraphics::world->models.size()-1;
						undostack.push(new cUndoNewObject());
					}
				}
				break;
			case SDLK_BACKSPACE:
				if (Graphics.selectedObject > -1 && Graphics.selectedObject < (int)cGraphics::world->models.size())
				{
					undostack.push(new cUndoObjectDelete(Graphics.selectedObject));
					delete cGraphics::world->models[Graphics.selectedObject];
					cGraphics::world->models.erase(cGraphics::world->models.begin() + Graphics.selectedObject);
					Graphics.selectedObject = -1;
					cWindow* w = cWM::getWindow(WT_MODELOVERVIEW);
					if(w != NULL)
						w->userfunc(NULL);
				}
				break;
			case SDLK_i:
				if (Graphics.selectedObject != -1)
				{
					cRSMModel* m = cGraphics::world->models[Graphics.selectedObject];
					Log(3,0,"Objects: %i", Graphics.selectedObject);
					Log(3,0,"Pos: %f,%f,%f", m->pos.x, m->pos.y, m->pos.z);
					Log(3,0,"scale: %f,%f,%f", m->scale.x, m->scale.y, m->scale.z);
					Log(3,0,"rot: %f,%f,%f", m->rot.x, m->rot.y, m->rot.z);
					Log(3,0,"nr of submeshes: %i", m->meshes.size());
				}
				break;
			case SDLK_RETURN:
				if (Graphics.selectedObject != -1)
				{
					cRSMModel* o = cGraphics::world->models[Graphics.selectedObject];
					cMenuItem* menuitem = (cMenuItem*)models->findData("data\\model\\" + o->rofilename);

					cWindow* w = new cObjectWindow();
					if (menuitem != NULL)
						w->objects["objectmenu"]->setText(0,menuitem->data2);
					((cWindowFloatInputBox*)w->objects["posx"])->floatje = &o->pos.x;
					((cWindowFloatInputBox*)w->objects["posy"])->floatje = &o->pos.y;
					((cWindowFloatInputBox*)w->objects["posz"])->floatje = &o->pos.z;
					((cWindowFloatInputBox*)w->objects["rotx"])->floatje = &o->rot.x;
					((cWindowFloatInputBox*)w->objects["roty"])->floatje = &o->rot.y;
					((cWindowFloatInputBox*)w->objects["rotz"])->floatje = &o->rot.z;
					((cWindowFloatInputBox*)w->objects["scalex"])->floatje = &o->scale.x;
					((cWindowFloatInputBox*)w->objects["scaley"])->floatje = &o->scale.y;
					((cWindowFloatInputBox*)w->objects["scalez"])->floatje = &o->scale.z;
					((cWindowStringInputBox*)w->objects["objectname"])->stringetje = &o->name;
					((cWindowFloatInputBox*)w->objects["lightopacity"])->floatje = &o->lightopacity;
					
					((cObjectWindow*)w)->undo = new cUndoChangeObject(Graphics.selectedObject);

					cWM::addWindow(w);
				}
				break;
			case SDLK_r:
				if (Graphics.selectedObject != -1)
				{
					undostack.push(new cUndoChangeObject(Graphics.selectedObject));
					cGraphics::world->models[Graphics.selectedObject]->rot = cVector3(0,0,0);
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
