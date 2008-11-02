#include "inputwindow.h"

cInputWindow::cWindowOkButton::cWindowOkButton( cWindow* parent, TiXmlDocument &skin ) : cWindowButton(parent, skin)
{
	alignment = ALIGN_BOTTOMLEFT;
	moveTo(20, 20);
	resizeTo(100, 20);
	text = "Ok";
}

void cInputWindow::cWindowOkButton::click()
{
	parent->userfunc((void*)1);
}

cInputWindow::cWindowCancelButton::cWindowCancelButton( cWindow* parent, TiXmlDocument &skin ) : cWindowButton(parent, skin)
{
	alignment = ALIGN_BOTTOMRIGHT;
	moveTo(20, 20);
	resizeTo(100, 20);
	text = "Cancel";
}

void cInputWindow::cWindowCancelButton::click()
{
	parent->userfunc((void*)0);
}

cInputWindow::cInputWindow( cInputWindowCaller* c, cTexture* t, cFont* f, TiXmlDocument &skin ) : cWindow(t,f,skin)
{
	caller = c;
	
	windowType = WT_INPUT;
	resizable = false;
	visible = true;
	modal = true;
	
	h = 200;
	w = 350;
	title = "Please input...";
	center();
	
	defaultObject = "OkButton";
	
	addLabel("text", 15,40,"UserId")->resizeTo(w-30, 12);
	
	cWindowObject* o = new cWindowInputBox(this,skin);
	o->alignment = ALIGN_TOPLEFT;
	o->moveTo(15,60);
	o->resizeTo(w-30, 20);
	objects["input"] = o;
	selectedObject = o;
	
	objects["rollup"] = new cWindowRollupButton(this,skin);
	objects["close"] = new cWindowCloseButton(this,skin);;
	objects["OkButton"] = new cWindowOkButton(this,skin);
	objects["CancelButton"] = new cWindowCancelButton(this,skin);
}

void* cInputWindow::userfunc( void* param )
{
	caller->data = objects["input"]->getText(0);
	if ((int)param == 1)
		caller->Ok();
	else
		caller->Cancel();
	
	delete caller;
	close();
	return NULL;
}