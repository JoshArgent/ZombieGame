#include "Button.h"
#include "Psyja2Engine.h"

Button::Button(Psyja2Engine *engine, char *text, int x, int y) : DisplayableObject(engine), engine(engine), text(text)
{
	m_iDrawWidth = 200;
	m_iDrawHeight = 50;
	m_iCurrentScreenX = x;
	m_iCurrentScreenY = y;
	font = engine->GetFont("Millimetre-Bold.otf", 20);

	// calculate an estimation of the offset required to centre the text
	int i = 0;
	for (; text[i] != NULL; i++);
	xOffset = 100 - (10 * i) / 2;
}

Button::~Button()
{

}

void Button::Draw()
{
	// Do not draw if it should not be visible
	if (!IsVisible())
		return;
	unsigned int color = 0x293d29;

	// Draw button
	if (mouseOver)
	{
		engine->DrawScreenRectangle(m_iCurrentScreenX + 3, m_iCurrentScreenY + 3, m_iCurrentScreenX + 200, m_iCurrentScreenY + 50, color);
	}
	else
	{
		engine->DrawScreenRectangle(m_iCurrentScreenX + 3, m_iCurrentScreenY + 3, m_iCurrentScreenX + 200, m_iCurrentScreenY + 50, 0x000000);
		engine->DrawScreenRectangle(m_iCurrentScreenX + 1, m_iCurrentScreenY + 1, m_iCurrentScreenX + 198, m_iCurrentScreenY + 48, color);
	}

	// Draw text
	engine->DrawScreenString(m_iCurrentScreenX + xOffset, m_iCurrentScreenY + 15, text, 0xFFFFFF, font);
	
	StoreLastScreenPositionForUndraw();
}

void Button::DoUpdate(int iCurrentTime)
{
	// calculate if mouse is over:
	int iX = engine->GetCurrentMouseX();
	int iY = engine->GetCurrentMouseY();
	mouseOver = iX > m_iCurrentScreenX && iX < m_iCurrentScreenX + 200 && iY > m_iCurrentScreenY && iY < m_iCurrentScreenY + 50;
	if (mouseOver && engine->GetMouseXClickedDown() == iX && engine->GetMouseYClickedDown() == iY)
		pressed = true;
	else
		pressed = false;

	RedrawObjects();
}

bool Button::IsPressed()
{
	return pressed;
}

