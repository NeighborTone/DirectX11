#pragma once
#include "Physics.h"
#include "Utility.hpp"
#include "Mesh.h"
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
	dGeomID GetGeomID() const;
	/*!
	* @brief dBodyIDを取得します
	* @return dBodyID
	*/
	dBodyID GetBodyID() const;
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
	* @brief ボディの重心に作用する力を設定します。これは主にボディを有効化するときに使用します。
	* @detail このメソッドを用いることで、剛体ごとに重力をかけるようなことができます
	* @param (force) 向きに対して与えたい力
	*/
	void AddForce(const Vec3& force);
	/*!
	* @brief 剛体に速度を与えます
	* @param (velocity) 向きに対して与えたい速度
	*/
	void AddVelocity(const Vec3& velocity);
	/*!
	* @brief 剛体の速度を得ます
	* @return Vec3
	*/
	Vec3 GetVelocity() const;
	/*!
	* @brief 未実装
	*/
	void SetQuaternion(Vec3& angle);
	/*!
	* @brief 未実装
	*/
	Vec3 GetQuaternion();
	/*!
	* @brief 任意の軸1つに対しての姿勢を設定します
	* @param (axis) 軸
	* @param (degree) 回転値
	*/
	void SetAxisAndAngle(Axis axis,float degree);
	/*!
	* @brief 剛体に対しての姿勢を設定します
	* @param (angle) 度数(degree)での回転値
	*/
	void SetRotation(const Vec3& angle);
	/*!
	* @brief 剛体の姿勢を回転行列で得ます
	* @return DirectX::XMMATRIX
	*/
	DirectX::XMMATRIX GetRotation() const;
	/*!
	* @brief 剛体の物理演算を有効にします
	*/
	void BodyEnable();
	/*!
	* @brief 剛体の物理演算を無効にします
	*/
	void BodyDisable();
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
class DynamicBox : public RigidBody
{
private:
	Mesh mesh;
public:
	DynamicBox(const Vec3& pos, const Vec3& scale, dReal totalMass);
	DynamicBox(const DynamicBox& box);
	DynamicBox();
	~DynamicBox();
	/*!
	* @brief 直方体を生成します
	* @param (pos)       剛体の中心座標
	* @param (scale)	 剛体のそれぞれの軸の1辺の大きさ
	* @param (totalMass) 質量
	*/
	void Create(const Vec3& pos, const Vec3& scale, dReal totalMass);

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
class DynamicSphere : public RigidBody
{
public:
	DynamicSphere(const Vec3& pos, const dReal& radius,dReal totalMass);
	DynamicSphere(const DynamicSphere& sphere);
	DynamicSphere();
	~DynamicSphere();
	/*!
	* @brief 球を生成します
	* @param (pos)       剛体の中心座標
	* @param (radius)    半径
	* @param (totalMass) 質量
	*/
	void Create(const Vec3& pos, const dReal& radius, const dReal totalMass);
	void Draw(Texture& tex) override;
};

/*! 
*	@class DynamicCylinder
*   @brief 円筒を生成します
*/
class DynamicCylinder : public RigidBody
{
public:
	DynamicCylinder(const Vec3& pos, const dReal totalMass, const int direction, const dReal radius, const dReal length);
	DynamicCylinder(const DynamicCylinder& cylinder);
	DynamicCylinder();
	~DynamicCylinder();
	/*!
	* @brief 円筒を生成します
	* @param (pos)       剛体の中心座標
	* @param (totalMass) 質量
	* @param (direction) 円筒の向き
	* @param (radius)    半径
	* @param (length)    長さ
	*/
	void Create(const Vec3& pos , const dReal totalmass, const int direction, const dReal radius, const dReal length);
	void Draw(Texture& tex) override;
};
 
/*!
*	@class DynamicCapsule
*   @brief カプセルを生成します
*/
class DynamicCapsule : public RigidBody
{
public:
	DynamicCapsule(const Vec3& pos, const dReal totalMass, const int direction, const dReal radius, const dReal length);
	DynamicCapsule(const DynamicCapsule& capsule);
	DynamicCapsule();
	~DynamicCapsule();
	/*!
	* @brief カプセルを生成します
	* @param (pos)       剛体の中心座標
	* @param (totalMass) 質量
	* @param (direction) カプセルの向き
	* @param (radius)    半径
	* @param (length)    長さ
	*/
	void Create(const Vec3& pos, const dReal totalMass, const int direction, const dReal radius, const dReal length);
	void Draw(Texture& tex) override;
};
