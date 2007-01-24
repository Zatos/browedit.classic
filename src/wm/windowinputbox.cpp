#include "windowinputbox.h"
#include "window.h"

#include <GL/gl.h>												// Header File For The OpenGL32 Library
#include <GL/glu.h>												// Header File For The GLu32 Library
extern float mousex;
void cWindowInputBox::draw()
{
	GLfloat colors[4];
	glGetFloatv(GL_CURRENT_COLOR, colors);
	int xx, yy;
	xx = realx();
	yy = realy();

	glBindTexture(GL_TEXTURE_2D, parent->texture->texid());
	glBegin(GL_QUADS);


		glTexCoord2f((371.0f)/512.0f,	(337.0f)/512.0f);				glVertex2d(xx+2, yy+2);
		glTexCoord2f((371.0f)/512.0f,	(337.0f)/512.0f);				glVertex2d(xx+w-2, yy+2);
		glTexCoord2f((371.0f)/512.0f,	(337.0f)/512.0f);				glVertex2d(xx+w-2, yy+h-2);
		glTexCoord2f((371.0f)/512.0f,	(337.0f)/512.0f);				glVertex2d(xx+2, yy+h-2);

		glTexCoord2f((258.0f+2.0f)/512.0f,	(340.0f+2.0f)/512.0f);		glVertex2d(xx+w-2, yy+2);
		glTexCoord2f((258.0f)/512.0f,		(340.0f+2.0f)/512.0f);		glVertex2d(xx+w, yy+2);
		glTexCoord2f((258.0f)/512.0f,		(469.0f-2.0f)/512.0f);		glVertex2d(xx+w, yy+h-2);
		glTexCoord2f((258.0f+2.0f)/512.0f,	(469.0f-2.0f)/512.0f);		glVertex2d(xx+w-2, yy+h-2);

		glTexCoord2f((387.0f-2.0f)/512.0f,	(340.0f+2.0f)/512.0f);		glVertex2d(xx, yy+2);
		glTexCoord2f((387.0f)/512.0f,		(340.0f+2.0f)/512.0f);		glVertex2d(xx+2, yy+2);
		glTexCoord2f((387.0f)/512.0f,		(469.0f-2.0f)/512.0f);		glVertex2d(xx+2, yy+h-2);
		glTexCoord2f((387.0f-2.0f)/512.0f,	(469.0f-2.0f)/512.0f);		glVertex2d(xx, yy+h-2);

		glTexCoord2f((258.0f+2.0f)/512.0f,	(340.0f+2.0f)/512.0f);		glVertex2d(xx, yy+h);
		glTexCoord2f((258.0f+2.0f)/512.0f,	(340.0f)/512.0f);			glVertex2d(xx, yy+h-2);
		glTexCoord2f((387.0f)/512.0f,		(340.0f)/512.0f);			glVertex2d(xx+w-2, yy+h-2);
		glTexCoord2f((387.0f)/512.0f,		(340.0f+2.0f)/512.0f);		glVertex2d(xx+w-2, yy+h);
		
		glTexCoord2f((258.0f)/512.0f,		(469.0f)/512.0f);			glVertex2d(xx+2, yy+2);
		glTexCoord2f((258.0f)/512.0f,		(469.0f-2.0f)/512.0f);		glVertex2d(xx+2, yy);
		glTexCoord2f((387.0f-2.0f)/512.0f,	(469.0f-2.0f)/512.0f);		glVertex2d(xx+w, yy);
		glTexCoord2f((387.0f-2.0f)/512.0f,	(469.0f)/512.0f);			glVertex2d(xx+w, yy+2);

		glTexCoord2f((258.0f+2.0f)/512.0f,	(340.0f+2.0f)/512.0f);		glVertex2d(xx, yy);
		glTexCoord2f((258.0f)/512.0f,		(340.0f+2.0f)/512.0f);		glVertex2d(xx+2, yy);
		glTexCoord2f((258.0f)/512.0f,		(340.0f)/512.0f);			glVertex2d(xx+2, yy+2);
		glTexCoord2f((258.0f+2.0f)/512.0f,	(340.0f)/512.0f);			glVertex2d(xx, yy+2);

		glTexCoord2f((387.0f)/512.0f,		(469.0f)/512.0f);			glVertex2d(xx+w-2, yy+h-2);
		glTexCoord2f((387.0f-2.0f)/512.0f,	(469.0f)/512.0f);			glVertex2d(xx+w, yy+h-2);
		glTexCoord2f((387.0f-2.0f)/512.0f,	(469.0f-2.0f)/512.0f);		glVertex2d(xx+w, yy+h);
		glTexCoord2f((387.0f)/512.0f,		(469.0f-2.0f)/512.0f);		glVertex2d(xx+w-2, yy+h);

	glEnd();
	glDisable(GL_TEXTURE_2D);

	string temptext = text;
	if (mask != "")
	{
		char buf[255];
		ZeroMemory(buf, 255);
		memset(buf, (char)mask[0], text.length());
		text = buf;
	}

	string t = text;
	t = t.substr(startindex, 1000000);
	int l = t.size();
	while(l > 0 && parent->font->textlen(t.substr(0, l)) > w-5)
		l--;
	t.resize(l);


	float len = 0;
	float xoff = 0;
	if (cursor > selectionstart)
	{
		len = parent->font->textlen(text.substr(max(selectionstart, startindex), cursor-selectionstart-(startindex > selectionstart ? startindex : 0)));
		xoff = parent->font->textlen(text.substr(startindex, selectionstart-startindex))+5;
		if (selectionstart < startindex)
			xoff = 5;
	}
	if (cursor < selectionstart)
	{
 		len = parent->font->textlen(text.substr(cursor, min(selectionstart-cursor, (int)t.length()-(cursor-startindex))));
		xoff = parent->font->textlen(text.substr(startindex, cursor-startindex))+5;
	}

	if (focussed)
	{
		glColor3f(0.7f,0.7f,1.0f);
		glBegin(GL_QUADS);
			glVertex2d(xx+xoff, yy+3);
			glVertex2d(xx+xoff+len, yy+3);
			glVertex2d(xx+xoff+len,yy+h-3);
			glVertex2d(xx+xoff, yy+h-3);
		glEnd();
	}
	glColor3f(0,0,0);

	if (mask == "")
		parent->font->print(0,0,0, parent->px()+xx+5, parent->py()+yy+2, "%s", t.c_str());
	else
	{
		parent->font->print(0,0,0, parent->px()+xx+5, parent->py()+yy+2, t.c_str());
	}
	if (editable && focussed && (SDL_GetTicks() % 1000) > 500)
		parent->font->print(0,0,0,parent->px()+2+xx+parent->font->textlen(t.substr(0, cursor-startindex)), parent->py()+yy+2, "|");

	text = temptext;

	glColor4fv(colors);
}


void cWindowInputBox::click()
{
	if (parent->selectedobject != this)
	{
		selectionstart = 0;
		cursor = text.length();
		return;
	}
	int xx = mousex;
	xx -= realx();
	xx -= parent->px();
	int i = startindex;
	while(xx > 0 && i <= (int)text.length())
	{
		xx -= parent->font->width[(int)text[i]-32];
		i++;
	}

	selectionstart = (cursor = min(max(i-1, 0), (int)text.length()));
}


bool cWindowInputBox::onkeydown(int keyid)
{
	if (editable)
	{
		bool ret = false;
		if (keyid == SDLK_LEFT)
		{
			if (cursor > 0)
				cursor--;
			if (!(SDL_GetModState() & KMOD_SHIFT))
				selectionstart = cursor;
			ret = true;
		}
		if (keyid == SDLK_RIGHT)
		{
			if (cursor < (int)text.length())
				cursor++;
			if (!(SDL_GetModState() & KMOD_SHIFT))
				selectionstart = cursor;
			ret = true;
		}
		if (keyid == SDLK_END)
		{
			cursor = text.length();
			if (!(SDL_GetModState() & KMOD_SHIFT))
				selectionstart = cursor;
			ret = true;
		}
		if (keyid == SDLK_HOME)
		{
			cursor = 0;
			if (!(SDL_GetModState() & KMOD_SHIFT))
				selectionstart = cursor;
			ret = true;
		}		

		if (keyid == SDLK_BACKSPACE)//backspace
		{
			if (cursor != selectionstart)
			{
				text = text.substr(0, min(cursor, selectionstart)) + text.substr(max(cursor, selectionstart), 10000);
				cursor = min(selectionstart, cursor);
				selectionstart = cursor;
			}
			else if (cursor != 0)
			{
				text = text.substr(0, cursor-1) + text.substr(cursor);
				cursor--;
			}
			selectionstart = cursor;
			ret = true;
		}
		if (keyid == SDLK_DELETE)//delete
		{
			if (cursor != selectionstart)
			{
				text = text.substr(0, min(cursor, selectionstart)) + text.substr(max(cursor, selectionstart), 10000);
				cursor = min(selectionstart, cursor);
			}
			if (cursor != (int)text.length())
			{
				text = text.substr(0, cursor) + text.substr(cursor+1);
			}
			ret = true;
		}
		while(cursor < startindex)
			startindex = max(0, cursor-4);
		while(parent->font->textlen(text.substr(startindex, cursor-startindex)) > w-10)
			startindex = min(startindex + 4, (int)text.length());
		if (selectionstart > (int)text.length())
			selectionstart = cursor;
		if (ret)
			return true;

	}		
	return false;
}

bool cWindowInputBox::onchar(char keyid)
{
	if (keyid > 27 && editable)
	{
		text = text.substr(0, min(cursor, selectionstart)) + text.substr(max(cursor, selectionstart), 10000);
		cursor = min(selectionstart, cursor);
		text = text.substr(0, cursor) + (char)keyid + text.substr(cursor);
		cursor++;
		while(cursor < startindex)
			startindex = max(0, cursor-4);
		while(parent->font->textlen(text.substr(startindex, cursor-startindex)) > w-10)
			startindex = min(startindex + 4, (int)text.length());

		while(parent->font->textlen(text.substr(startindex, cursor-startindex)) > w-10)
			startindex = min(startindex + 4, (int)text.length());

		if (!(SDL_GetModState() & KMOD_SHIFT))
			selectionstart = cursor;
		selectionstart = cursor;
		return true;
	}
	selectionstart = cursor;
	return false;
}


void cWindowInputBox::SetText(int i, string t)
{
	if (i == 0)
		text = t;
	if (i == 1)
		mask = t;
}

void cWindowInputBox::SetInt(int id, int val)
{
	if (id == 0)
		editable = (val == 1);
	if (id == 1)
		cursor = val;
	if (id == 2)
		selectionstart = val;
}

string cWindowInputBox::GetText(int id)
{
	return text;
}

int cWindowInputBox::GetInt(int id)
{
	if (id == 0)
		return editable ? 1 : 0;
	if (id == 1)
		return cursor;
	if (id == 2)
		return selectionstart;
	return cursor;
}