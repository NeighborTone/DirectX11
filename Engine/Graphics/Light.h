//#pragma once 
//#include "../Engine.h"
//#include <vector>
//#include <algorithm>
//
//class Engine;
//class LightManager
//{
//public:
//	struct Constant
//	{
//		int type;
//		Vec3 pos;
//		Vec3 direction;
//		float range;
//		Float4 color;
//	};
//
//	static const int limit = 100;
//
//	static void AddLight(Constant* const constant)
//	{
//		Get().lights.emplace_back(constant);
//	}
//	static void RemoveLight(Constant* const constant)
//	{
//		std::vector<Constant*>& v = Get().lights;
//		v.erase(remove(v.begin(), v.end(), constant), v.end());
//	}
//	static void Update()
//	{
//		for (int i = 0; i < limit; i++)
//		{
//			if (Get().lights.size() > i)
//				Get().constant[i] = *Get().lights[i];
//			else
//				Get().constant[i].type = -1;
//		}
//
//		Engine::GetDXContext3D().UpdateSubresource(Get().constantBuffer.Get(), 0, nullptr, Get().constant, 0, 0);
//		Engine::GetDXContext3D().VSSetConstantBuffers(1, 1, Get().constantBuffer.GetAddressOf());
//		Engine::GetDXContext3D().HSSetConstantBuffers(1, 1, Get().constantBuffer.GetAddressOf());
//		Engine::GetDXContext3D().DSSetConstantBuffers(1, 1, Get().constantBuffer.GetAddressOf());
//		Engine::GetDXContext3D().GSSetConstantBuffers(1, 1, Get().constantBuffer.GetAddressOf());
//		Engine::GetDXContext3D().PSSetConstantBuffers(1, 1, Get().constantBuffer.GetAddressOf());
//	}
//
//private:
//	struct Property
//	{
//		std::vector<Constant*> lights;
//		Constant constant[limit];
//		Microsoft::WRL::ComPtr<ID3D11Buffer> constantBuffer = nullptr;
//	};
//
//	static Property& Get()
//	{
//		static std::unique_ptr<Property> prop;
//
//		if (prop == nullptr)
//		{
//			prop.reset(new Property());
//
//			D3D11_BUFFER_DESC constantBufferDesc = {};
//			constantBufferDesc.ByteWidth = sizeof(Constant) * limit;
//			constantBufferDesc.Usage = D3D11_USAGE_DEFAULT;
//			constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
//			Engine::GetDXDevice3D().CreateBuffer(&constantBufferDesc, nullptr, Get().constantBuffer.GetAddressOf());
//		}
//
//		return *prop;
//	}
//};
//
//
//class Light
//{
//public:
//	enum class Type : int
//	{
//		Directional,
//		Point,
//	};
//
//	Type type;
//	Vec3 position;
//	Vec3 angles;
//	Vec3 color;
//	float range;
//	float intensity;
//
//	Light()
//	{
//		type = Type::Directional;
//		color = Vec3(1.0f, 1.0f, 1.0f);
//		range = 5.0f;
//		intensity = 1.0f;
//
//		LightManager::AddLight(&_constant);
//	}
//	~Light()
//	{
//		LightManager::RemoveLight(&_constant);
//	}
//	void Update()
//	{
//		_constant.direction = Vec3(
//			cosf(DirectX::XMConvertToRadians(angles.x)) * cosf(DirectX::XMConvertToRadians(angles.y + 90.0f)),
//			sinf(DirectX::XMConvertToRadians(angles.x)),
//			cosf(DirectX::XMConvertToRadians(angles.x)) * sinf(DirectX::XMConvertToRadians(angles.y + 90.0f))
//		);
//
//		_constant.type = (int)type;
//		_constant.pos = position;
//		auto temp = color * intensity;
//		Float4 tcolor(temp.x, temp.y, temp.z, 0);
//		_constant.color = tcolor;
//		_constant.range = range;
//	}
//
//private:
//	LightManager::Constant _constant;
//};