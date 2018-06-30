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
	
	textLayout.Reset();
	Engine::GetTextFactory().CreateTextLayout(
		StringConverter::StringToWString(text).c_str(), 
		(UINT32)text.length(),
		textFormat.Get(), 
		FLT_MAX, FLT_MAX, 
		textLayout.GetAddressOf());
	
	DWRITE_TEXT_METRICS textMetrics;
	textLayout->GetMetrics(&textMetrics);
	
	textLayout.Reset();
	Engine::GetTextFactory().CreateTextLayout(
		StringConverter::StringToWString(text).c_str(),
		(UINT32)text.length(),
		textFormat.Get(),
		textMetrics.width, 
		textMetrics.height, 
		textLayout.GetAddressOf());
	
	std::unique_ptr<BYTE[]> buffer(new BYTE[(unsigned int)textMetrics.width * (unsigned int)textMetrics.height * 4]);
	texture.Create(buffer.get(), (int)textMetrics.width, (int)textMetrics.height);

	Microsoft::WRL::ComPtr<IDXGISurface> surface = nullptr;
	texture.GetInterface().QueryInterface(surface.GetAddressOf());

	D2D1_BITMAP_PROPERTIES1 bitmapProperties = {};
	bitmapProperties.pixelFormat.format = DXGI_FORMAT_B8G8R8A8_UNORM;
	bitmapProperties.pixelFormat.alphaMode = D2D1_ALPHA_MODE_PREMULTIPLIED;
	bitmapProperties.bitmapOptions = D2D1_BITMAP_OPTIONS_TARGET;

	bitmap.Release();
	Engine::GetDXContext2D().CreateBitmapFromDxgiSurface(surface.Get(), bitmapProperties, &bitmap);

	
	mesh.GetMaterial().SetTexture(0, &texture);

	SetPivot(Vec2(0,0));
}


void Text::Draw()
{
	Engine::GetDXContext2D().SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_DEFAULT);
	Engine::GetDXContext2D().SetTarget(bitmap);

	Engine::GetDXContext2D().BeginDraw();
	Engine::GetDXContext2D().Clear(D2D1::ColorF(1.0f, 1.0f, 1.0f, 0.0f));

	Engine::GetDXContext2D().DrawTextLayout(D2D1::Point2F(0.0f, 0.0f), textLayout.Get(), brush.Get());

	Engine::GetDXContext2D().EndDraw();

	__super::Draw();
}
