#ifndef LABEL_H
#define LABEL_H
#include <Windows.h>
#include "Defines.h"
class Label
{
private:
	LPCSTR			text;
	ID3DXFont	 *	font;
	ID3DXSprite	 *	sprite;
	D3DXVECTOR2		vec2_pos;
	D3DXCOLOR		color;
	int				fontsize;
public:
	Label();
	void create(LPDIRECT3DDEVICE9 device, LPCSTR text, D3DXVECTOR2 _Pos);
	void SetColor(int a ,int r, int g, int b);
	void setText(LPCSTR str);
	void Draw();
	~Label();
};


#endif