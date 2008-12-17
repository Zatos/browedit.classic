#ifndef __WORLD_H__
#define __WORLD_H__

#include <list>
#include "texture.h"
#ifdef WIN32
#include <gd/gd.h>
#else
#include <gd.h>
#endif
#include <tinyxml/tinyxml.h>
#include "RSMModel.h"
#include "texturecache.h"
#include "texturemodel.h"
#include "sprite.h"
#include "plugins/base/interface.h"


class cLightmap : public cBrowInterface::cPluginLightmap
{
public:
	cLightmap()
	{
		generated = false;
		generated2 = false;
	}
	~cLightmap()
	{
		if (generated)
			glDeleteTextures(1, &tid);
		generated = false;
		if (generated2)
			glDeleteTextures(1, &tid2);
		generated2 = false;
	}
	void regenerate()
	{
		del();
		del2();
	}
	int texId();
	int texId2();
	void del();
	void del2();

	bool generated;
	bool generated2;
	GLuint tid;
	GLuint tid2;
};

class cRealLightMap
{
public:
	cRealLightMap();
	int texId();
	int texId2();

	bool generated;
	bool generated2;
	GLuint tid;
	GLuint tid2;
	int x;
	int y;

	void reset();
};

class cTile : public cBrowInterface::cPluginTile
{
public:
	std::vector<std::vector<int> > lightsWithShadow;
	cTile() {};
	cTile(cBrowInterface::cPluginTile t);
};


class cCube : public cBrowInterface::cPluginCube
{
public:
	float maxHeight;
	float minHeight;
	void calcNormal();
	cVector3 normal;
	cVector3 vNormal1;
	cVector3 vNormal2;
	cVector3 vNormal3;
	cVector3 vNormal4;

};

class cTextureContainer : public cBrowInterface::cPluginTexture
{
public:
	cTexture* texture;
	std::string RoFilename;
	std::string RoFilename2;
	GLuint texId() { return texture->texId(); }

};

class cLight : public cBrowInterface::cPluginLight
{
public:
	cLight() {};
	cLight(cBrowInterface::cPluginLight other);
	bool operator == (cLight other);
};


class cSound : public cBrowInterface::cPluginSound
{
public:
	cSound(){};
	cSound(cBrowInterface::cPluginSound);
	bool operator == (cSound other);

};

class cEffect : public cBrowInterface::cPluginEffect
{
public:
	cEffect() {};
	cEffect(cBrowInterface::cPluginEffect e);
};


class cQuadTreeNode
{
public:
	int load(std::vector<cVector3>&, int index, int level);
	void save(std::vector<cVector3>&);
	void generate(float w, float h, float, float,int);
	void draw(int level);

	void recalculate();
	cVector3 box1;
	cVector3 box2;


	cVector3 range1;
	cVector3 range2;

	cQuadTreeNode* child1;
	cQuadTreeNode* child2;
	cQuadTreeNode* child3;
	cQuadTreeNode* child4;
	
	~cQuadTreeNode();
};


class cGatTile : public cBrowInterface::cPluginGatCube
{
public:

};

class cWater
{
public:
	float	height;
	int		type;
	float	amplitude;
	float	phase;
	float	surfaceCurve;
//	int		texcycle; // unknown
};

class cAmbientLight
{
public:
	int	ambientr;
	int	ambientg;
	int	ambientb;
	cVector3	diffuse;
	cVector3	shadow;
	float		alpha;
};

class cWorld
{

public:
	cAmbientLight						ambientLight;
	cWater								water;
	bool								loaded;

	char								fileName[128];
	int									height;
	int									width;
	int									texSize;
	bool								quickSave;
	std::vector<cTextureContainer*>		textures;
	std::vector<cLightmap*>				lightmaps;
	std::vector<cTile>					tiles;
	std::vector<std::vector<cCube> >	cubes;
	std::vector<std::vector<cGatTile> >		gattiles;
	std::vector<std::vector<cRealLightMap*> >		realLightmaps;

	std::vector<cRSMModel*>				models;
	std::vector<cLight>					lights;
	std::vector<cSound>					sounds;
	std::vector<cEffect>				effects;
	std::vector<cSprite*>				sprites;

	
	long								size;
	float								tileScale;
	
	int									lightmapWidth;
	int									lightmapHeight;
	int									gridSizeCell;
	
	cTextureModel*						light;
	cTextureModel*						light2;
	cTextureModel*						sound;
	cTextureModel*						effect;
	
	std::vector<cVector3>				quadTreeFloats;
	cQuadTreeNode* root;
	
	
	cWorld();
	~cWorld();
	void newEmpty(int,int);
	void								draw();
	void								load();
	void								save();
	void								exportheight();
	void								unload();
	void								newWorld();
	void								clean();

	void								saveLightmap();
	void								loadLightmap();
	bool								checkSanity();

	void								fixGridding();
	bool								makeLightmapsUnique();
	bool								blackLightmaps();
	void								calcVertexNormals(int = 0, int = 0, int = -1, int=-1);
	std::vector<cCube*>					getWall(int,int,bool,bool);

	

	char								iniFile[40];
	char								gndFile[40];
	char								gatFile[40];


	int									unknown1;
	int									unknown2;
	int									unknown3;
	int									unknown4;



};

#endif
