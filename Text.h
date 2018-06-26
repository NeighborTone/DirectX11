#pragma once
#include "StringConverter.hpp"
#include "Sprite.h"
#include "Engine.h"

/*! @class Text
*   @brief �e�L�X�g���X�v���C�g�Ƃ��ĕ`�悷�邽�߂̃N���X�ł�
*/
class Text : public Sprite
{
public:
	Text();
	Text(const std::string& text = "", float fontSize = 16.0f, const std::string& fontFace = "�l�r �S�V�b�N");
	~Text();
	/*!
	* @brief �e�L�X�g���쐬���܂�
	* @param (text) �\��������������
	* @param (fontSize) �t�H���g�T�C�Y
	* @param (fontFace) �g�p�������t�H���g
	*/
	void Create(const std::string& text = "", float fontSize = 16.0f, const std::string& fontFace = "�l�r �S�V�b�N");
	/*!
	* @brief �e�L�X�g���X�v���C�g�Ƃ��ĕ`�悵�܂�
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
