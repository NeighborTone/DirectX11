#pragma once
#include "Physics.h"
#include "../Utilitys/Vec.hpp"
#include "../Graphics/Mesh.h"
//とりあえずテンプレートメソッドパターンで実装


/*!
* @enum Axis
* SetAxisAndAngleメソッドの第一引数で使います
* これはどの軸に対して回転したいかを指定するために用います
*/
enum class Axis
{
	X,
	Y,
	Z
};

/*! 
*	@class RigidBody
*   @brief  OpenDynamicsEngineで物理演算を行いたい剛体を生成するクラスのインターフェースです
*   @detail このクラスはPhysicsWorldのAddRigidBodyメソッドの引数で生成します
*	このクラス単体では使用できません
*	このクラスが持つメソッドの処理内容は描画以外継承先でも変わりません
*/
class RigidBody
{
private:

protected:
	dBodyID body;		//剛体
	dGeomID geom;		//衝突検知
	
public:
	RigidBody();
	virtual ~RigidBody();
	/*!
	* @brief dGeomIDを取得します
	* @return dGeomID
	*/
	virtual dGeomID GetGeomID() const final;
	/*!
	* @brief dBodyIDを取得します
	* @return dBodyID
	*/
	virtual dBodyID GetBodyID() const final;
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
	* @brief ボディの重心に作用する力を設定します。これは主にボディを有効化するときに使用します。
	* @detail このメソッドを用いることで、剛体ごとに重力をかけるようなことができます
	* @param (force) 向きに対して与えたい力
	*/
	virtual void AddForce(const Vec3& force) final;
	/*!
	* @brief 剛体に速度を与えます
	* @param (velocity) 向きに対して与えたい速度
	*/
	virtual void AddVelocity(const Vec3& velocity) final;
	/*!
	* @brief 剛体の速度を得ます
	* @return Vec3
	*/
	virtual Vec3 GetVelocity() const final;
	/*!
	* @brief 未実装
	*/
	virtual void SetQuaternion(Vec3& angle) final;
	/*!
	* @brief 未実装
	*/
	virtual Vec3 GetQuaternion() final;
	/*!
	* @brief 任意の軸1つに対しての姿勢を設定します
	* @param (axis) 軸
	* @param (degree) 回転値
	*/
	virtual void SetAxisAndAngle(Axis axis,float degree) final;
	/*!
	* @brief 剛体に対しての姿勢を設定します
	* @param (angle) 度数(degree)での回転値
	*/
	virtual void SetRotation(const Vec3& angle) final;
	/*!
	* @brief 剛体の姿勢を回転行列で得ます
	* @return DirectX::XMMATRIX
	*/
	virtual DirectX::XMMATRIX GetRotation() const final;
	/*!
	* @brief 剛体の物理演算を有効にします
	*/
	virtual void BodyEnable() final;
	/*!
	* @brief 剛体の物理演算を無効にします
	*/
	virtual void BodyDisable() final;
	/*!
	* @brief 剛体の形状を描画します(現在Boxのみ)
	* @param (tex) 描画に使いたいテクスチャー
	*/
	virtual void Draw(Texture& tex) = 0;
};


/*! 
*	@class DynamicBox
*   @brief 直方体を生成します
*/
class DynamicBox final : public RigidBody
{
private:
	Mesh mesh;
	void Create(const Vec3& pos, const Vec3& scale, dReal totalMass);
public:
	/*!
	* @brief 直方体を生成します
	* @param (pos)       剛体の中心座標
	* @param (scale)	 剛体のそれぞれの軸の1辺の大きさ
	* @param (totalMass) 質量
	*/
	DynamicBox(const Vec3& pos, const Vec3& scale, dReal totalMass);
	DynamicBox(const DynamicBox& box);
	DynamicBox();
	~DynamicBox();
	void Draw(Texture& tex) override;

	//アライメント対策
	void* operator new(size_t i)
	{
		return _mm_malloc(i, 16);
	}

	void operator delete(void* p)
	{
		_mm_free(p);
	}
};

/*! 
*	@class DynamicSphere
*   @brief 球を生成します
*/
class DynamicSphere final : public RigidBody
{
private:
	void Create(const Vec3& pos, const dReal& radius, const dReal totalMass);
public:
	/*!
	* @brief 球を生成します
	* @param (pos)       剛体の中心座標
	* @param (radius)    半径
	* @param (totalMass) 質量
	*/
	DynamicSphere(const Vec3& pos, const dReal& radius,dReal totalMass);
	DynamicSphere(const DynamicSphere& sphere);
	DynamicSphere();
	~DynamicSphere();
	
	void Draw(Texture& tex) override;
};

/*! 
*	@class DynamicCylinder
*   @brief 円筒を生成します
*/
class DynamicCylinder final : public RigidBody
{
private:
	void Create(const Vec3& pos, const dReal totalmass, const int direction, const dReal radius, const dReal length);
public:
	/*!
	* @brief 円筒を生成します
	* @param (pos)       剛体の中心座標
	* @param (totalMass) 質量
	* @param (direction) 円筒の向き
	* @param (radius)    半径
	* @param (length)    長さ
	*/
	DynamicCylinder(const Vec3& pos, const dReal totalMass, const int direction, const dReal radius, const dReal length);
	DynamicCylinder(const DynamicCylinder& cylinder);
	DynamicCylinder();
	~DynamicCylinder();

	void Draw(Texture& tex) override;
};
 
/*!
*	@class DynamicCapsule
*   @brief カプセルを生成します
*/
class DynamicCapsule final : public RigidBody
{
private:
	void Create(const Vec3& pos, const dReal totalMass, const int direction, const dReal radius, const dReal length);
public:
	/*!
	* @brief カプセルを生成します
	* @param (pos)       剛体の中心座標
	* @param (totalMass) 質量
	* @param (direction) カプセルの向き
	* @param (radius)    半径
	* @param (length)    長さ
	*/
	DynamicCapsule(const Vec3& pos, const dReal totalMass, const int direction, const dReal radius, const dReal length);
	DynamicCapsule(const DynamicCapsule& capsule);
	DynamicCapsule();
	~DynamicCapsule();

	void Draw(Texture& tex) override;
};
