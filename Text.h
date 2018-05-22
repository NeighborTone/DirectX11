#pragma once
#include "StringConverter.hpp"
#include "Sprite.h"
#include "Engine.h"

class Text : public Sprite
{
public:
	Text();
	Text(const std::string& text = "", float fontSize = 16.0f, const std::string& fontFace = "ÇlÇr ÉSÉVÉbÉN");
	~Text();
	void Create(const std::string& text = "", float fontSize = 16.0f, const std::string& fontFace = "ÇlÇr ÉSÉVÉbÉN");
	void Draw();
	void Load(const char* const filePath) = delete;
	void Create(const BYTE* const buffer, int width, int height) = delete;
private:
	std::string text;
	DirectX::XMINT2 length;
	ATL::CComPtr<ID2D1Bitmap1> bitmap;
	Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> brush;
	Microsoft::WRL::ComPtr<IDWriteTextFormat> textFormat;
};
