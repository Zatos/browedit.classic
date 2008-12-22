#include "interfaceimplementation.h"
#include <graphics.h>
#include <windows/xmlwindow.h>
#include <SDL/SDL.h>
#include <settings.h>
#include <plugins/base/base.h>

extern std::vector<cPluginBase*> plugins;

void cBrowInterfaceImplementation::fixNormals()
{
	cGraphics::world->calcVertexNormals();
}

cWindow* cBrowInterfaceImplementation::addXmlWindow(const char* fileName)
{
	TiXmlDocument windowLayout = cFileSystem::getXml(fileName);
	cXmlWindow* w = new cXmlWindow(windowLayout);
	cWM::addWindow(w);
	return w;
}

int cBrowInterfaceImplementation::getSelectedTextureIndex()
{
	return cGraphics::worldContainer->settings.texturestart + ((int)cGraphics::worldContainer->settings.selectionstart.y - 32) / 288;	
}


void cBrowInterfaceImplementation::render()
{
	cGraphics::draw();
	SDL_GL_SwapBuffers();
}

const char* cBrowInterfaceImplementation::inputWindow( const char* msg, const char* defaultText)
{
	std::string tmp = cWM::inputWindow(msg, defaultText);
	char* input = new char[1024];
	strcpy(input, tmp.c_str());
	return input;
}

void cBrowInterfaceImplementation::messageWindow( const char* msg)
{
	cWM::showMessage(msg);
}

cBrowInterface::cPluginRSMModel* cBrowInterfaceImplementation::loadModel( const char* )
{
	cRSMModel* model = new cRSMModel();
	model->load(cSettings::roDir +  "data\\model\\내부소품\\철다리.rsm");
	return model;	
}

void cBrowInterfaceImplementation::removeModel(int id)
{
	delete cGraphics::world->models[id];
	cGraphics::world->models.erase(cGraphics::world->models.begin() + id);
}

void cBrowInterfaceImplementation::removeSprite(int id)
{
	delete cGraphics::world->sprites[id];
	cGraphics::world->sprites.erase(cGraphics::world->sprites.begin() + id);
}

void cBrowInterfaceImplementation::addModel(cPluginRSMModel* model)
{
	cGraphics::world->models.push_back((cRSMModel*)model);
}

cBrowInterface::cPluginLightmap* cBrowInterfaceImplementation::getLightmap(int i)
{
	return cGraphics::world->lightmaps[i];
}

cBrowInterface::cPluginCube* cBrowInterfaceImplementation::getCube(int x, int y)
{
	return &cGraphics::world->cubes[y][x];
}

cBrowInterface::cPluginTexture* cBrowInterfaceImplementation::getTexture(int i)
{
	return cGraphics::world->textures[i];
}

cBrowInterface::cPluginTile* cBrowInterfaceImplementation::getTile(int i)
{
	return &cGraphics::world->tiles[i];
}


int cBrowInterfaceImplementation::tileCount()
{
	return cGraphics::world->tiles.size();
}
int cBrowInterfaceImplementation::effectCount()
{
	return cGraphics::world->effects.size();
}
int cBrowInterfaceImplementation::lightmapCount()
{
	return cGraphics::world->lightmaps.size();
}
int cBrowInterfaceImplementation::soundCount()
{
	return cGraphics::world->sounds.size();
}
int cBrowInterfaceImplementation::textureCount()
{
	return cGraphics::world->textures.size();
}

void cBrowInterfaceImplementation::addTile(cBrowInterface::cPluginTile tile)
{
	cGraphics::world->tiles.push_back(tile);
}
void cBrowInterfaceImplementation::addEffect(cBrowInterface::cPluginEffect effect)
{
	cGraphics::world->effects.push_back(effect);
}
void cBrowInterfaceImplementation::addTexture(cBrowInterface::cPluginTexture* pTexture)
{
	cGraphics::world->textures.push_back((cTextureContainer*)pTexture);
}
void cBrowInterfaceImplementation::addSound(cBrowInterface::cPluginSound sound)
{
	cGraphics::world->sounds.push_back(sound);
}
void cBrowInterfaceImplementation::addLight(cBrowInterface::cPluginLight light)
{
	cGraphics::world->lights.push_back(light);
}

void cBrowInterfaceImplementation::removeTile(int i)
{
	cGraphics::world->tiles.erase(cGraphics::world->tiles.begin()+i);
}
void cBrowInterfaceImplementation::removeEffect(int i)
{
	cGraphics::world->effects.erase(cGraphics::world->effects.begin()+i);
}
void cBrowInterfaceImplementation::removeTexture(int i)
{
	cTextureCache::unload(cGraphics::world->textures[i]->texture);
	cGraphics::world->textures.erase(cGraphics::world->textures.begin() + i);
}
void cBrowInterfaceImplementation::removeSound(int i)
{
	cGraphics::world->sounds.erase(cGraphics::world->sounds.begin()+i);
}
void cBrowInterfaceImplementation::removeLight(int i)
{
	cGraphics::world->lights.erase(cGraphics::world->lights.begin()+i);
}
void cBrowInterfaceImplementation::removeLightmap(int i)
{
	delete cGraphics::world->lightmaps[i];
	cGraphics::world->lightmaps.erase(cGraphics::world->lightmaps.begin()+i);
}

cBrowInterface::cPluginSound* cBrowInterfaceImplementation::getSound(int i)
{
	return &cGraphics::world->sounds[i];
}

cBrowInterfaceImplementation::cPluginEffect* cBrowInterfaceImplementation::getEffect(int i)
{
	return &cGraphics::world->effects[i];
}

void cBrowInterfaceImplementation::addLightmap(cPluginLightmap* p)
{
	cGraphics::world->lightmaps.push_back((cLightmap*)p);
}

cBrowInterface::cPluginLightmap* cBrowInterfaceImplementation::newLightmap()
{
	return new cLightmap();
}

void cBrowInterfaceImplementation::setWaterHeight(float f)
{
	cGraphics::world->water.height = f;
}

int cBrowInterfaceImplementation::getWorldHeight()
{
	return cGraphics::world->height;
}

int cBrowInterfaceImplementation::getWorldWidth()
{
	return cGraphics::world->width;
}

void cBrowInterfaceImplementation::makeTilesUnique()
{
	cGraphics::world->makeLightmapsUnique();
}

void cBrowInterfaceImplementation::fixGridding()
{
	cGraphics::world->fixGridding();
}

int cBrowInterfaceImplementation::spriteCount()
{
	return cGraphics::world->sprites.size();
}

int cBrowInterfaceImplementation::modelCount()
{
	return cGraphics::world->models.size();
}

int cBrowInterfaceImplementation::lightCount()
{
	return cGraphics::world->lights.size();
}

cBrowInterface::cPluginLight* cBrowInterfaceImplementation::getLight(int index)
{
	return &cGraphics::world->lights[index];
}

cBrowInterface::cPluginGatCube* cBrowInterfaceImplementation::getGatCube(int x, int y)
{
	return &cGraphics::world->gattiles[y][x];
}

bool cBrowInterfaceImplementation::confirmWindow(const char* message)
{
	return cWM::confirmWindow(message);
}


cPluginBase* cBrowInterfaceImplementation::getPlugin(const char* name)
{
	unsigned int i = 0;
	for(i = 0; i < plugins.size(); i++)
	{
		if(lcase(plugins[i]->name) == lcase(name))
			return plugins[i];
	}
	return NULL;
}


void cBrowInterfaceImplementation::checkModelsOnCubes()
{
	if(!cGraphics::world)
		return;
	int x, y;
	unsigned int i;
	int ww = cGraphics::w();
	ww -= 256;
	int hh = cGraphics::h()-20;
	
	glEnable(GL_DEPTH_TEST);
	glViewport(0,0,ww,hh);						// Reset The Current Viewport
	
	float camrad = 10;
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);				// Black Background
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix
	gluPerspective(45.0f,(GLfloat)ww/(GLfloat)hh,10.0f,10000.0f);
	gluLookAt(  -cGraphics::worldContainer->camera.pointer.x + cGraphics::worldContainer->camera.height*sin(cGraphics::worldContainer->camera.rot),
		camrad+cGraphics::worldContainer->camera.height,
		-cGraphics::worldContainer->camera.pointer.y + cGraphics::worldContainer->camera.height*cos(cGraphics::worldContainer->camera.rot),
		-cGraphics::worldContainer->camera.pointer.x,camrad + cGraphics::worldContainer->camera.height * (cGraphics::worldContainer->camera.angle/10.0f),-cGraphics::worldContainer->camera.pointer.y,
		0,1,0);
	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();									// Reset The Modelview Matrix
	//	glTranslatef(0,0,cGraphics::world->height*10);
	//	glScalef(1,1,-1);
	
	for(i = 0; i < cGraphics::world->models.size(); i++)
		cGraphics::world->models[i]->precollides();
	
	for(x = 0; x < cGraphics::world->width; x++)
	{
		for(y = 0; y < cGraphics::world->height; y++)
		{
			cGraphics::world->cubes[y][x].maxHeight = -99999;
			cGraphics::world->cubes[y][x].minHeight = 99999;
		}
	}
		
	for(i = 0; i < cGraphics::world->models.size(); i++)
	{
		Log(3,0,GetMsg("CALCMODEL"), i, cGraphics::world->models.size(), (i/(float)cGraphics::world->models.size())*100);
		cGraphics::world->models[i]->draw(false,false,true);
	}
	
	
	for(x = 0; x < cGraphics::world->width; x++)
	{
		for(y = 0; y < cGraphics::world->height; y++)
		{
			if(cGraphics::world->cubes[y][x].maxHeight == -99999 || cGraphics::world->cubes[y][x].minHeight == 99999)
				cGraphics::world->cubes[y][x].hasModelOnTop = true;
			else
				cGraphics::world->cubes[y][x].hasModelOnTop = false;
			
			cGraphics::world->cubes[y][x].maxHeight = 0;
			cGraphics::world->cubes[y][x].minHeight = 0;
		}
	}
			
	return;	
}



const char*	cBrowInterfaceImplementation::getMapFile()
{
	return cGraphics::world->fileName;
}

void cBrowInterfaceImplementation::makeLightmapsUnique()
{
	cGraphics::world->makeLightmapsUnique();	
}

void cBrowInterfaceImplementation::makeLightmapsBlack()
{
	cGraphics::world->blackLightmaps();
}