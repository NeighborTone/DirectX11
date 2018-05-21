#pragma once
#include "Utility.hpp"
#include "Mesh.h"
#include <DirectXMath.h>

/*! @class Sprite
*   @brief ‰æ‘œ‚ð•`‰æ‚·‚é‚½‚ß‚ÌƒNƒ‰ƒX‚Å‚·
*/
class Sprite
{
protected:
	Mesh mesh;
	Texture texture;
	void Initialize();
public:
	Vec3 pos;
	Vec3 angle;
	Vec3 scale;
	Float4 color;

	Sprite();
	Sprite(const char* const filePath);
	Sprite(const BYTE* const buffer, int width, int height);
	~Sprite();

	void Load(const char* const filePath);
	void Create(const BYTE* const buffer, int width, int height);
	DirectX::XMINT2 GetSize() const;
	void SetPivot(Vec2 pivot);
	Material& GetMaterial();
	void Draw();
};

