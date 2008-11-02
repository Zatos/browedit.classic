#ifndef __XMLWINDOW_H__
#define __XMLWINDOW_H__

#include <wm/window.h>
#include <wm/windowbutton.h>



class cXmlWindow : public cWindow
{
public:
	class cXmlButton : public cWindowButton
	{
	public:
		cXmlButton(cWindow*, TiXmlDocument&);
		void click();
	};
	cXmlWindow(cTexture*, cFont*, TiXmlDocument&, TiXmlDocument&);

	void event(cWindowObject*, std::string);

	void (*eventhandler)(cWindow*, std::string, std::string);

};





#endif
