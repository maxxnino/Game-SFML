#pragma once
#include "Box2D/Box2D.h"
#include "PhysicObject.h"
#include <vector>
class QuerySelectorBox2D
{
private:
	//query for select by point
	class PointQuerySelector : public b2QueryCallback
	{
	public:
		bool ReportFixture(b2Fixture* fixture) override
		{
			if (fixture->TestPoint(mousePos))
			{
				foundBodies.push_back(fixture->GetBody());
			}
			return true;//keep going to find all fixtures in the query area
		}
		std::vector<b2Body*> foundBodies;
		b2Vec2 mousePos = { 0.0f,0.0f };
	};

	//query for select by rect
	class RectQuerySelector : public b2QueryCallback
	{
	public:
		bool ReportFixture(b2Fixture* fixture) override
		{
			foundBodies.push_back(fixture->GetBody());
			return true;//keep going to find all fixtures in the query area
		}
		std::vector<b2Body*> foundBodies;
	};
public:
	QuerySelectorBox2D()
	{
		aabb.lowerBound = { 0.0f, 0.0f };
		aabb.upperBound = { 0.0f, 0.0f };
	}
	template <class T>
	std::vector<T*> GetBodyList(b2World& box2DEngine, const b2Vec2& upperBound, const b2Vec2& lowerBound)
	{
		rectQueryCallback.foundBodies.clear();
		aabb.lowerBound = lowerBound;
		aabb.upperBound = upperBound;
		box2DEngine.QueryAABB(&rectQueryCallback, aabb);

		std::vector<T*> bodyList;
		
		for (size_t i = 0; i < rectQueryCallback.foundBodies.size(); i++)
		{
			auto p = static_cast<PhysicObject*>(rectQueryCallback.foundBodies[i]->GetUserData());
			if (auto b = dynamic_cast<T*>(p))
			{
				bodyList.emplace_back(b);
			}
		}
		return std::move(bodyList);
	}
	template <class T>
	T* SelectOneBody(b2World& box2DEngine, const b2Vec2& pos)
	{
		pointQueryCallback.foundBodies.clear();
		pointQueryCallback.mousePos = pos;
		aabb.lowerBound = { pos.x - 0.01f, pos.y - 0.01f };
		aabb.upperBound = { pos.x + 0.01f, pos.y + 0.01f };
		box2DEngine.QueryAABB(&pointQueryCallback, aabb);

		float lengSQ = std::numeric_limits<float>::max();
		T* body = nullptr;

		for (size_t i = 0; i < pointQueryCallback.foundBodies.size(); i++)
		{
			auto p = static_cast<PhysicObject*>(pointQueryCallback.foundBodies[i]->GetUserData());
			if (auto b = dynamic_cast<T*>(p))
			{
				float curLengSQ = (pos - pointQueryCallback.foundBodies[i]->GetPosition()).LengthSquared();
				if (curLengSQ < lengSQ)
				{
					lengSQ = curLengSQ;
					body = b;
				}
			}
		}
		return body;
	}
private:
	PointQuerySelector pointQueryCallback;
	RectQuerySelector rectQueryCallback;
	b2AABB aabb;
};
