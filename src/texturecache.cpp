#include "texturecache.h"



cTexture* cTextureCache::load(string filename)
{
	if (textures.find(filename) == textures.end())
	{
		cTexture* tex = new cTexture();
		tex->Load(filename);
		textures[filename] = tex;
		
		used[tex] = 1;
	}
	else
		used[textures[filename]]++;
	return textures[filename];

}

void cTextureCache::unload(cTexture* tex)
{
	if (used.find(tex) == used.end())
	{
		Log(3,0,GetMsg("graphics/texturecache/UNLOADFAIL"), tex->getfilename().c_str());
		return;
	}
	used[tex]--;
	if (used[tex] == 0)
	{
		//Log(3,0,"Texture %s seems to be unused", tex->getfilename().c_str());
		textures[tex->getfilename()]->unLoad();
		textures.erase(textures.find(tex->getfilename()));
		used.erase(used.find(tex));
	}
	if (used[tex] < 0)
	{
		Log(3,0,GetMsg("graphics/texturecache/UNLOADALOT"), tex->getfilename().c_str());
	}
}


void cTextureCache::status()
{
	for(map<string, cTexture*, less<string> >::iterator i = textures.begin(); i != textures.end(); i++)
	{
		if (used[i->second] > 0)
		{
			Log(3,0,GetMsg("graphics/texturecache/STILLLOADED"), i->first.c_str(), used[i->second]);
		}
	}

}
