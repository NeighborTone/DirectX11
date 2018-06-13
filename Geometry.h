#pragma once
#include "Physics.h"
#include "Utility.hpp"
#include "Mesh.h"

/*!
*	@class Geometry
*   @brief  OpenDynamicsEngineで衝突判定を行いたい形状を生成するクラスのインターフェースです
*   @detail このクラスはPhysicsWorldのAddGeometryメソッドの引数で生成します
*	このクラス単体では使用できません
*	このクラスが持つメソッドの処理内容は継承先でも変わりません
*/
class Geometry
{
protected:
	dGeomID geom;
	
public:
	Geometry();
	virtual ~Geometry();
	/*!
	* @brief dGeomIDを取得します
	* @return dGeomID
	*/
	dGeomID GetGeomID() const;
	/*!
	* @brief 座標を取得します
	* @return Vec3
	*/
	Vec3 GetPosition() const;
	/*!
	* @brief 座標を設定します
	* @param (pos) 座標
	*/
	void SetPosition(const Vec3& pos);
	/*!
	* @brief 形状の姿勢を設定します
	* @param (angle) 度数(degree)での回転値
	*/
	void SetRotation(const Vec3& angle);
	/*!
	* @brief 形状の姿勢を回転行列で得ます
	* @return DirectX::XMMATRIX
	*/
	DirectX::XMMATRIX GetRotation() const;
	
};


class StaticBox : public Geometry
{
public:
	StaticBox(const Vec3& pos, const Vec3& scale);
	StaticBox();
	void Create(const Vec3& pos, const Vec3& scale);
	~StaticBox();
	
};

class StaticSphere : public Geometry
{
public:
	StaticSphere(const Vec3& pos, const dReal& radius);
	StaticSphere();
	~StaticSphere();
	void Create(const Vec3& pos, const dReal& radius);
};

class StaticCylinder : public Geometry
{
public:
	StaticCylinder(const Vec3& pos, const int direction, const dReal radius, const dReal length);
	StaticCylinder();
	~StaticCylinder();
	void Create(const Vec3& pos, const int direction, const dReal radius, const dReal length);
};

class StaticCapsule : public Geometry
{
public:
	StaticCapsule(const Vec3& pos, const int direction, const dReal radius, const dReal length);
	StaticCapsule();
	~StaticCapsule();
	void Create(const Vec3& pos, const int direction, const dReal radius, const dReal length);
};