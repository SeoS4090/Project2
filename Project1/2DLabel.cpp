#include "2DLabel.h"
#include <d3dx9.h>


Label::Label()
{
	font = NULL;
	sprite = NULL;
	text = "";
}

void Label::create(LPDIRECT3DDEVICE9 device,LPCSTR _text, D3DXVECTOR2 _Pos)
{
	vec2_pos = _Pos;
	fontsize = 30;
	D3DXCreateFont(device, fontsize, 0, 0, 0, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "±Ã¼­Ã¼", &font);	
	D3DXCreateSprite(device, &sprite);	
	
	text =_text;

	color.a = 255;
	color.r = 255;
	color.g = 0;
	color.b = 0;


}

void Label::SetColor(int a, int r, int g, int b)
{
	color.a = a % 256;
	color.r = r % 256;
	color.g = g % 256;
	color.b = b % 256;
}
void Label::setText(LPCSTR str)
{
	text = str;
}

void Label::Draw()
{
	if (sprite != NULL)
	{
		sprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);
		RECT rec = { vec2_pos.x, vec2_pos.y , vec2_pos.x + fontsize * strlen(text), vec2_pos.y + fontsize };
		font->DrawTextA(sprite, text, strlen(text), &rec, NULL, color);
		sprite->End();
	}
	
}


Label::~Label()
{
	if(font != NULL)
		font->Release();
	if(sprite != NULL)
		sprite->Release();
}
