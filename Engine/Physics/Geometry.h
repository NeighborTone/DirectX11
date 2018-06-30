#pragma once
#include "Physics.h"
#include "../Utilitys/Vec.hpp"
#include "../Graphics/Mesh.h"
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
	virtual dGeomID GetGeomID() const final;
	/*!
	* @brief 座標を取得します
	* @return Vec3
	*/
	virtual Vec3 GetPosition() const final;
	/*!
	* @brief 座標を設定します
	* @param (pos) 座標
	*/
	virtual void SetPosition(const Vec3& pos) final;
	/*!
	* @brief 形状の姿勢を設定します
	* @param (angle) 度数(degree)での回転値
	*/
	virtual void SetRotation(const Vec3& angle) final;
	/*!
	* @brief 形状の姿勢を回転行列で得ます
	* @return DirectX::XMMATRIX
	*/
	virtual DirectX::XMMATRIX GetRotation() const final;
	
};


class StaticBox final : public Geometry
{
private:
	void Create(const Vec3& pos, const Vec3& scale);
public:
	StaticBox(const Vec3& pos, const Vec3& scale);
	StaticBox();
	~StaticBox();
};

class StaticSphere final : public Geometry
{
private:
	void Create(const Vec3& pos, const dReal& radius);
public:
	StaticSphere(const Vec3& pos, const dReal& radius);
	StaticSphere();
	~StaticSphere();	
};

class StaticCylinder final : public Geometry
{
private:
	void Create(const Vec3& pos, const dReal radius, const dReal length);
public:
	StaticCylinder(const Vec3& pos, const dReal radius, const dReal length);
	StaticCylinder();
	~StaticCylinder();
};

class StaticCapsule final : public Geometry
{
private:
	void Create(const Vec3& pos, const dReal radius, const dReal length);
public:
	StaticCapsule(const Vec3& pos, const dReal radius, const dReal length);
	StaticCapsule();
	~StaticCapsule();
};