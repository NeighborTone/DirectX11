#include "Text.h"
#include <memory>

Text::Text():
	bitmap(nullptr),
	brush(nullptr),
	textFormat(nullptr),
	textLayout(nullptr)
{
	__super::Initialize();
	color = Float4(0.0f, 0.0f, 0.0f, 1.0f);
}

Text::Text(const std::string& text, float fontSize, const std::string& fontFace) :
	bitmap(nullptr),
	brush(nullptr),
	textFormat(nullptr),
	textLayout(nullptr)
{
	__super::Initialize();
	color = Float4(0.0f, 0.0f, 0.0f, 1.0f);
	Create(text, fontSize, fontFace);
}


Text::~Text()
{
	
}

void Text::Create(const std::string& text, float fontSize, const std::string&  fontFace)
{
	if (text == "")
	{
		return;
	}
		
	this->text = text;

	length = DirectX::XMINT2(0, 1);
	int maxLength = 0;
	for (UINT i = 0; i < text.length(); ++i)
	{
		if (text[i] == '\n')
		{
			maxLength = 0;
			++length.y;
			continue;
		}

		++maxLength;

		if (length.x < maxLength)
		{
			length.x = maxLength;
		}
	}

	DirectX::XMINT2 textureSize(static_cast<int>(length.x * fontSize), static_cast<int>(length.y * fontSize * 2.0f));
	std::unique_ptr<BYTE[]> buffer(new BYTE[textureSize.x * textureSize.y * 4]);
	texture.Create(buffer.get(), textureSize.x, textureSize.y);

	ATL::CComPtr<IDXGISurface> surface = nullptr;
	texture.GetInterface().QueryInterface(&surface);

	D2D1_BITMAP_PROPERTIES1 bitmapProperties = {};
	bitmapProperties.pixelFormat.format = DXGI_FORMAT_B8G8R8A8_UNORM;
	bitmapProperties.pixelFormat.alphaMode = D2D1_ALPHA_MODE_PREMULTIPLIED;
	bitmapProperties.bitmapOptions = D2D1_BITMAP_OPTIONS_TARGET;

	bitmap.Release();
	Engine::GetDXContext2D().CreateBitmapFromDxgiSurface(surface, bitmapProperties, &bitmap);

	brush.Reset();
	Engine::GetDXContext2D().CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), brush.GetAddressOf());
	Engine::GetDXContext2D().SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_ALIASED);

	textFormat.Reset();
	Engine::GetTextFactory().CreateTextFormat(
		StringConverter::StringToWString(fontFace).c_str(),
		nullptr, 
		DWRITE_FONT_WEIGHT_NORMAL, 
		DWRITE_FONT_STYLE_NORMAL, 
		DWRITE_FONT_STRETCH_NORMAL, 
		fontSize, 
		L"ja-jp", 
		textFormat.GetAddressOf());

	textFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
	textFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

	mesh.GetMaterial().SetTexture(0, &texture);

	SetPivot(Vec2(0,0));
}


void Text::Draw()
{
	Engine::GetDXContext2D().SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_DEFAULT);
	Engine::GetDXContext2D().SetTarget(bitmap);

	Engine::GetDXContext2D().BeginDraw();
	Engine::GetDXContext2D().Clear(D2D1::ColorF(1.0f, 1.0f, 1.0f, 0.0f));

	Engine::GetTextFactory().CreateTextLayout(StringConverter::StringToWString(text).c_str(), (UINT32)text.length(), textFormat.Get(), static_cast<float>(texture.GetSize().x), static_cast<float>(texture.GetSize().y), textLayout.GetAddressOf());

	Engine::GetDXContext2D().DrawTextLayout(D2D1::Point2F(0.0f, 0.0f), textLayout.Get(), brush.Get());

	Engine::GetDXContext2D().EndDraw();

	__super::Draw();
}
