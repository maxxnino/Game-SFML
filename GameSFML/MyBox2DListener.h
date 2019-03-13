#pragma once
#include "Box2D/Box2D.h"
#include <functional>
#include <unordered_map>
#include <typeinfo>
#include <type_traits>
#include "PhysicObject.h"
using TypePair = std::pair<
	const std::type_info*,
	const std::type_info*
>;

namespace std
{
	template <>
	struct hash<TypePair>
	{
		size_t operator()(const TypePair& tp) const
		{
			const auto hash0 = tp.first->hash_code();
			return hash0 ^ (tp.second->hash_code() + 0x9e3779b9 + (hash0 << 6) + (hash0 >> 2));
		}
	};
	template <>
	struct equal_to<TypePair>
	{
		size_t operator()(const TypePair& lhs, const TypePair& rhs) const
		{
			return *lhs.first == *rhs.first && *lhs.second == *rhs.second;
		}
	};
}

class MyBox2DListener : public b2ContactListener
{
public:
	template<class T, class U, class F>
	void CaseContact(F f)
	{
		static_assert(std::is_base_of<PhysicObject, T>::value, "Template param type T must be derived from Box::ColorTrait!");
		static_assert(std::is_base_of<PhysicObject, U>::value, "Template param type U must be derived from Box::ColorTrait!");
		ContactHandlers[{&typeid(T), &typeid(U)}] = f;
		ContactHandlers[{&typeid(U), &typeid(T)}] = std::bind(
			f, std::placeholders::_2, std::placeholders::_1
		);
	}
	template<class T, class U, class F>
	void CaseLeave(F f)
	{
		static_assert(std::is_base_of<PhysicObject, T>::value, "Template param type T must be derived from Box::ColorTrait!");
		static_assert(std::is_base_of<PhysicObject, U>::value, "Template param type U must be derived from Box::ColorTrait!");
		LeaveHandlers[{&typeid(T), &typeid(U)}] = f;
		LeaveHandlers[{&typeid(U), &typeid(T)}] = std::bind(
			f, std::placeholders::_2, std::placeholders::_1
		);
	}
	template<class T, class U>
	bool HasCase() const
	{
		static_assert(std::is_base_of<PhysicObject, T>::value, "Template param type T must be derived from Box::ColorTrait!");
		static_assert(std::is_base_of<PhysicObject, U>::value, "Template param type U must be derived from Box::ColorTrait!");
		return ContactHandlers.count({ &typeid(T),&typeid(U) }) > 0;
	}
	template<class T, class U>
	void ClearCase()
	{
		static_assert(std::is_base_of<PhysicObject, T>::value, "Template param type T must be derived from Box::ColorTrait!");
		static_assert(std::is_base_of<PhysicObject, U>::value, "Template param type U must be derived from Box::ColorTrait!");
		ContactHandlers.erase({ &typeid(T),&typeid(U) });
		ContactHandlers.erase({ &typeid(U),&typeid(T) });
	}
	template<class F>
	void Default(F f)
	{
		def = f;
	}
	void BeginContact(b2Contact* contact) override
	{
		SwitchContact(
			*reinterpret_cast<PhysicObject*>(contact->GetFixtureA()->GetBody()->GetUserData()),
			*reinterpret_cast<PhysicObject*>(contact->GetFixtureB()->GetBody()->GetUserData())
		);
	}
	void EndContact(b2Contact* contact)
	{
		SwitchLeave(
			*reinterpret_cast<PhysicObject*>(contact->GetFixtureA()->GetBody()->GetUserData()),
			*reinterpret_cast<PhysicObject*>(contact->GetFixtureB()->GetBody()->GetUserData())
		);
	}
private:
	void SwitchContact(PhysicObject& a, PhysicObject& b)
	{
		auto i = ContactHandlers.find({
			&typeid(a),
			&typeid(b)
			});
		if (i != ContactHandlers.end())
		{
			i->second(&a, &b);
		}
		/*else
		{
			def(a, b);
		}*/
	}
	void SwitchLeave(PhysicObject& a, PhysicObject& b)
	{
		auto i = LeaveHandlers.find({
			&typeid(a),
			&typeid(b)
			});
		if (i != LeaveHandlers.end())
		{
			i->second(&a, &b);
		}
		/*else
		{
			def(a, b);
		}*/
	}

private:
	std::unordered_map<TypePair, std::function<void(PhysicObject*, PhysicObject*)>> ContactHandlers;
	std::unordered_map<TypePair, std::function<void(PhysicObject*, PhysicObject*)>> LeaveHandlers;
	std::function<void(PhysicObject&, PhysicObject&)> def = [](PhysicObject&, PhysicObject&) {};
};