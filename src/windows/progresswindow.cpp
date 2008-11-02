#include "progresswindow.h"
#include <wm/windowprogressbar.h>

cProgressWindow::cCancelButton::cCancelButton( cWindow* parent, bool* pCancel, TiXmlDocument &skin ) : cWindowButton(parent,skin)
{
	cancel = pCancel;
	text = "Cancel";
	alignment = ALIGN_BOTTOMLEFT;
	moveTo(0,0);
}

void cProgressWindow::cCancelButton::click()
{
	if(cancel != NULL)
		*cancel = false;
}

cProgressWindow::cProgressWindow( cTexture* t, cFont* f, bool* cancel, TiXmlDocument &skin ) : cWindow(t,f,skin)
{
	windowType = WT_PROGRESS;
	resizable = false;
	visible = true;
	
	h = 110;
	w = 350;
	title = GetMsg("wm/progress/TITLE");
	center();
	
	defaultObject = "OkButton";
	
	cWindowObject* o;
	
	o = new cWindowProgressBar(this,skin);
	o->alignment = ALIGN_TOPLEFT;
	o->moveTo(0,7);
	o->resizeTo(innerWidth(), 10);
	objects["progress"] = o;
	
	addLabel("lblStatus", 0,15,"")->resizeTo(innerWidth(), 20);
	
	objects["cancel"] = new cCancelButton(this, cancel,skin);
	
	
	//objects["ClButton"] = new cOkButton(this, pCallback);
}