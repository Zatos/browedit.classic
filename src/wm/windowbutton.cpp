#include "windowbutton.h"
#include "window.h"

#include <GL/gl.h>												// Header File For The OpenGL32 Library
#include <GL/glu.h>												// Header File For The GLu32 Library

void cWindowButton::draw()
{
	int xx, yy;
	xx = realx();
	yy = realy();
	glEnable(GL_TEXTURE_2D);

	GLfloat colors[4];
	glGetFloatv(GL_CURRENT_COLOR, colors);
	
	glColor4f(1,1,1, colors[3]);
	if (this->inobject() && parent->istopwindow())
		glColor4f(0.75f, 0.75f, 1.0f, colors[3]);

	glBindTexture(GL_TEXTURE_2D, parent->texture->texid());
	glBegin(GL_QUADS);


		glTexCoord2f((258.0f+2.0f)/512.0f,	(340.0f+2.0f)/512.0f);		glVertex2d(xx+2, yy+2);
		glTexCoord2f((387.0f-2.0f)/512.0f,	(340.0f+2.0f)/512.0f);		glVertex2d(xx+w-2, yy+2);
		glTexCoord2f((387.0f-2.0f)/512.0f,	(469.0f-2.0f)/512.0f);		glVertex2d(xx+w-2, yy+h-2);
		glTexCoord2f((258.0f+2.0f)/512.0f,	(469.0f-2.0f)/512.0f);		glVertex2d(xx+2, yy+h-2);

		glTexCoord2f((258.0f+2.0f)/512.0f,	(340.0f+2.0f)/512.0f);		glVertex2d(xx, yy+2);
		glTexCoord2f((258.0f)/512.0f,		(340.0f+2.0f)/512.0f);		glVertex2d(xx+2, yy+2);
		glTexCoord2f((258.0f)/512.0f,		(469.0f-2.0f)/512.0f);		glVertex2d(xx+2, yy+h-2);
		glTexCoord2f((258.0f+2.0f)/512.0f,	(469.0f-2.0f)/512.0f);		glVertex2d(xx, yy+h-2);

		glTexCoord2f((387.0f-2.0f)/512.0f,	(340.0f+2.0f)/512.0f);		glVertex2d(xx+w-2, yy+2);
		glTexCoord2f((387.0f)/512.0f,		(340.0f+2.0f)/512.0f);		glVertex2d(xx+w, yy+2);
		glTexCoord2f((387.0f)/512.0f,		(469.0f-2.0f)/512.0f);		glVertex2d(xx+w, yy+h-2);
		glTexCoord2f((387.0f-2.0f)/512.0f,	(469.0f-2.0f)/512.0f);		glVertex2d(xx+w-2, yy+h-2);

		glTexCoord2f((258.0f+2.0f)/512.0f,	(340.0f+2.0f)/512.0f);		glVertex2d(xx+2, yy+2);
		glTexCoord2f((258.0f+2.0f)/512.0f,	(340.0f)/512.0f);			glVertex2d(xx+2, yy);
		glTexCoord2f((387.0f)/512.0f,		(340.0f)/512.0f);			glVertex2d(xx+w, yy);
		glTexCoord2f((387.0f)/512.0f,		(340.0f+2.0f)/512.0f);		glVertex2d(xx+w, yy+2);
		
		glTexCoord2f((258.0f)/512.0f,		(469.0f)/512.0f);			glVertex2d(xx, yy+h);
		glTexCoord2f((258.0f)/512.0f,		(469.0f-2.0f)/512.0f);		glVertex2d(xx, yy+h-2);
		glTexCoord2f((387.0f-2.0f)/512.0f,	(469.0f-2.0f)/512.0f);		glVertex2d(xx+w-2, yy+h-2);
		glTexCoord2f((387.0f-2.0f)/512.0f,	(469.0f)/512.0f);			glVertex2d(xx+w-2, yy+h);

		glTexCoord2f((258.0f)/512.0f,		(340.0f)/512.0f);			glVertex2d(xx, yy);
		glTexCoord2f((258.0f+2.0f)/512.0f,	(340.0f)/512.0f);			glVertex2d(xx+2, yy);
		glTexCoord2f((258.0f+2.0f)/512.0f,	(340.0f+2.0f)/512.0f);		glVertex2d(xx+2, yy+2);
		glTexCoord2f((258.0f)/512.0f,		(340.0f+2.0f)/512.0f);		glVertex2d(xx, yy+2);

		glTexCoord2f((387.0f-2.0f)/512.0f,	(469.0f-2.0f)/512.0f);		glVertex2d(xx+w-2, yy+h-2);
		glTexCoord2f((387.0f)/512.0f,		(469.0f-2.0f)/512.0f);		glVertex2d(xx+w, yy+h-2);
		glTexCoord2f((387.0f)/512.0f,		(469.0f)/512.0f);			glVertex2d(xx+w, yy+h);
		glTexCoord2f((387.0f-2.0f)/512.0f,	(469.0f)/512.0f);			glVertex2d(xx+w-2, yy+h);


	glEnd();
	parent->font->print(0,0,0, parent->px() + xx+5, parent->py() + yy+3, text.c_str());
	glColor4fv(colors);
}


void cWindowButton::click()
{
	Log(1,0, "You clicked the button!");
}

bool cWindowButton::onkeyup(int c)
{
	if (c == SDLK_RETURN)
	{
		click();
		return true;
	}
	return false;
}