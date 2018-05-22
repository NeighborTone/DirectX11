#include "Text.h"

Text::Text():
	bitmap(nullptr),
	brush(nullptr),
	textFormat(nullptr)
{
	__super::Initialize();
	color = Float4(0.0f, 0.0f, 0.0f, 1.0f);
}

Text::Text(const std::string& text, float fontSize, const char* const fontFace) :
	bitmap(nullptr),
	brush(nullptr),
	textFormat(nullptr)
{
	__super::Initialize();
	color = Float4(0.0f, 0.0f, 0.0f, 1.0f);
	Create(text, fontSize, fontFace);
}


Text::~Text()
{

}

void Text::Create(const std::string& text, float fontSize, const char* const fontFace)
{

}

void Text::Draw()
{

}
