#ifndef __INTERFACEIMPLEMENTATION_H__
#define __INTERFACEIMPLEMENTATION_H__

#include "plugins/base/interface.h"

class cBrowInterfaceImplementation : public cBrowInterface
{
public:
	void fixNormals();
	cWindow* addXmlWindow(std::string);
	int getSelectedTextureIndex();
	void render();
	std::string inputWindow(std::string, std::string);
	void messageWindow(std::string);
	cRSMModel* loadModel(std::string);
	void removeModel(int);
};


#endif
