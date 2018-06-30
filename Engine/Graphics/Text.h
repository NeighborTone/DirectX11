#pragma once
#include "../Utilitys/StringConverter.hpp"
#include "Sprite.h"
#include "../Engine.h"

/*! @class Text
*   @brief テキストをスプライトとして描画するためのクラスです
*/
class Text : public Sprite
{
public:
	Text();
	Text(const std::string& text = "", float fontSize = 16.0f, const std::string& fontFace = "ＭＳ ゴシック");
	~Text();
	/*!
	* @brief テキストを作成します
	* @param (text) 表示したい文字列
	* @param (fontSize) フォントサイズ
	* @param (fontFace) 使用したいフォント
	*/
	void Create(const std::string& text = "", float fontSize = 16.0f, const std::string& fontFace = "ＭＳ ゴシック");
	/*!
	* @brief テキストをスプライトとして描画します
	*/
	void Draw();

	void Load(const char* const filePath) = delete;
	void Create(const BYTE* const buffer, int width, int height) = delete;
private:
	ATL::CComPtr<ID2D1Bitmap1> bitmap;
	Microsoft::WRL::ComPtr<IDWriteTextLayout> textLayout;
	Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> brush;
	Microsoft::WRL::ComPtr<IDWriteTextFormat> textFormat;
};
