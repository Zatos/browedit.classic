#ifndef __WINDOWLINE_H__
#define __WINDOWLINE_H__

#include "windowobject.h"
//using namespace std;

class cWindowLine : public cWindowObject
{
protected:
	int r,g,b;
public:
	cWindowLine(cWindow* parent) : cWindowObject(parent)
	{
		w = 100;
		h = 100;
		x = 5;
		y = 20;
		alignment = ALIGN_BOTTOMLEFT;
		type = OBJECT_LINE;
		r = (g = (b = 0));
	}
	void draw(int cutoffleft, int cutoffright, int cutofftop, int cutoffbottom)
	{
		GLfloat colors[4];
		glGetFloatv(GL_CURRENT_COLOR, colors);
		glColor3i(r,g,b);
		glDisable(GL_TEXTURE_2D);
		glBegin(GL_LINES);
			glVertex2f(x, parent->getHeight() - y);
			glVertex2f(w, parent->getHeight() - h);
		glEnd();
		glEnable(GL_TEXTURE_2D);
		glColor4fv(colors);
	}
	void setInt(int index, int val)
	{
		if (index == 0)
			x = val;
		else if (index == 1)
			y = val;
		else if (index == 2)
			w = val;
		else if (index == 3)
			h = val;
		else if (index == 4)
			r = val;
		else if (index == 5)
			g = val;
		else if (index == 6)
			b = val;
	}

	cWindowObject* inObject()
	{
		int xx=mouseX-parent->getX();
		int yy=mouseY-parent->getY();
		if (xx > realX() && xx < w &&
			yy > realY() && yy < h)
			return this;
		return NULL;
	}
};




#endif
