#ifndef __WINDOW_H__
#define __WINDOW_H__
#include "../common.h"

#include <map>
using namespace std;

#include "../texture.h"
#include "../font.h"
#include "windowobject.h"

typedef map<string, cWindowObject*, less<string> > objectlist;

extern cWindowObject* draggingobject;

enum WINDOW_TYPE
{
	WT_MESSAGE,
	WT_CONFIRM,
	WT_OBJECT,
};

enum CLOSE_TYPE
{
	CLOSE,
	HIDE
};
class cWindow
{
protected:
	bool modal;
	bool visible;
	bool rolledup;
	bool topwindow;
	int  x, y;
	int  h, w;
	bool resizable;
	bool movable;
	bool resizingx;
	bool resizingxy;
	bool resizingy;
	bool resizingyx;
	int minh, minw;
	WINDOW_TYPE wtype;
	string title;
	CLOSE_TYPE closetype;
	bool enabled;
public:
	bool savewindow;
	string defaultobject;
	cWindow()
	{
		visible = false;
		rolledup = false;
		resizable = true;
		movable = true;
		resizingx = false;
		resizingy = false;
		resizingxy = false;
		resizingyx = false;
		minh = 100;
		minw = 100;
		selectedobject = NULL;
		closetype = CLOSE;
		modal = false;
		enabled = true;
	}
	virtual ~cWindow()
	{
		for(map<string, cWindowObject*, less<string> >::iterator i = objects.begin(); i != objects.end(); i++)
		{
			if(draggingobject == i->second)
				draggingobject = NULL;
			delete i->second;
		}

		objects.clear();
		// delete objects
	}
	int  px()						{ return x; }
	int  py()						{ return rolledup ? y+h-16 : y; }
	int  py2()						{ return y; }
	int  pw()						{ return w; }
	int  ph()						{ return rolledup ? 16 : h; }
	int  ph2()						{ return h; }
	bool modality()					{ return modal; }
	void istopwindow(bool b)		{ topwindow = b; }
	bool istopwindow()				{ return topwindow; }

	void init(cTexture* t, cFont* f){ texture = t; font = f; }
		
	WINDOW_TYPE windowtype()		{ return wtype; }
	bool isvisible()				{ return visible; }
	void hide()						{ visible = false; }
	void show()						{ visible = true; }
	void togglevis()				{ visible = !visible; }
	void rollup()					{ rolledup  = true; }
	void rollback()					{ rolledup  = false; }
	void toggleroll()				{ rolledup  = !rolledup; }
	void stopresizing()				{ resizingxy = (resizingyx = (resizingy = (resizingx = false))); }
	void startresisingx()			{ resizingx = true; }
	void startresisingxy()			{ resizingxy = true; }
	void startresizingy()			{ resizingy = true; }
	void startresizingyx()			{ resizingyx = true; }
	void disable()					{ enabled = false; }
	void enable()					{ enabled = true; }
	bool isenabled()				{ return enabled; }
	bool resizing()					{ return resizingxy | resizingyx | resizingx | resizingy; }
	void moveto(int xx, int yy)		{ x = xx; y = yy; }
	virtual void resizeto(int ww, int hh)	{ w = ww; h = hh; }

	void SetTitle(string t)			{ title = t; }
	string gettitle()				{ return title; }

	int getcursor();
	void close();

	virtual bool inwindow();
	cWindowObject* inobject();
	bool drag();
	virtual void click();
	virtual void draw();
	virtual void save() { };
	virtual void doubleclick();
	virtual void rightclick();
	virtual void stopdrag() {}
	virtual void dragover() {}
	virtual void holddragover() {}

	virtual bool onkeyup(int);
	virtual bool onkeydown(int);
	virtual bool onchar(char);


	void center();

	virtual void* userfunc(void*) { return NULL; };

	cTexture* texture;
	cFont* font;
	objectlist objects;
	cWindowObject*	selectedobject;
};

extern float startmousex, startmousey, dragoffsetx, dragoffsety;

#endif