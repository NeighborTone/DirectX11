#pragma once
#include "../Utilitys/Vec.hpp"
#include "Mesh.h"
#include <DirectXMath.h>

/*! @class Sprite
*   @brief 画像を描画するためのクラスです
*/
class Sprite
{
protected:
	Mesh mesh;
	Texture texture;
	void Initialize();
	void Create(const BYTE* const buffer, int width, int height);
public:
	Vec3 pos;
	Vec3 angle;
	Vec3 scale;
	Float4 color;

	Sprite();
	Sprite(const char* const filePath);
	Sprite(const BYTE* const buffer, int width, int height);
	~Sprite();

	/*!
	* @brief スプライトをロードします
	* @param (filePath) 画像のパス
	*/
	void Load(const char* const filePath);

	/*!
	* @brief スプライトのサイズを得ます
	* @return XINT2
	*/
	DirectX::XMINT2 GetSize() const;
	/*!
	* @brief スプライトの軸を設定します
	* @param (pivot) 回転の中心軸
	*/
	void SetPivot(Vec2 pivot);
	/*!
	* @brief Materialを取得します
	* @return Material&
	*/
	Material& GetMaterial();
	/*!
	* @brief スプライトを描画します
	*/
	void Draw();

	/*!
	* @brief 2次元限定のスプライトの矩形当たり判定
	*	画像サイズと同じでないと機能しない、また、スケールにも対応していない
	* @param (Sprite) 判定をしたい相手
	*/
	bool HitSprite(Sprite& s2);

	/*!
	* @brief 2次元限定のスプライトの円の当たり判定
	* @param (Sprite) 判定をしたい相手
	*/
	bool HitCircle(Sprite& c2);

};

