#include "wbpch.h"
#include "CollisionDetector.h"

#include "Logger/Logger.h"

namespace Workbench {
	std::vector<Contact> CollisionDetector::DetectContacts(const std::vector<RigidBodyComponent*>& bodies) {
		std::vector<Contact> contacts;

		for (int i = 0; i < bodies.size(); i++) {
			for (int j = i + 1; j < bodies.size(); j++) {
				auto body1 = bodies[i];
				auto body2 = bodies[j];

				auto body_contacts = CheckForCollisions(body1, body2);
				for (auto& contact : body_contacts) {
					contacts.push_back(contact);
				}
			}
		}

		return contacts;
	}

	std::vector<Contact> CollisionDetector::CheckForCollisions(RigidBodyComponent* body1, RigidBodyComponent* body2) {
		if (body1->collider->GetType() == ColliderType::BoxCollider && body2->collider->GetType() == ColliderType::BoxCollider) {
			auto contacts = _contactBoxAndBox((BoxCollider*)body1->collider, (BoxCollider*)body2->collider);
			for (auto& contact : contacts) {
				if(body1->physicsEnabled)
					contact.bodies[0] = body1;
				if(body2->physicsEnabled)
					contact.bodies[1] = body2;
			}
			return contacts;
		}

		else if (body1->collider->GetType() == ColliderType::BoxCollider && body2->collider->GetType() == ColliderType::HalfSpaceCollider) {
			auto contacts = _contactBoxAndHalfSpace((BoxCollider*)body1->collider, (HalfSpaceCollider*)body2->collider);
			for (auto& contact : contacts) {
				if(body1->physicsEnabled)
					contact.bodies[0] = body1;
				contact.bodies[1] = nullptr;
			}
			return contacts;
		}
		else if (body1->collider->GetType() == ColliderType::HalfSpaceCollider && body2->collider->GetType() == ColliderType::BoxCollider) {
			auto contacts = _contactBoxAndHalfSpace((BoxCollider*)body2->collider, (HalfSpaceCollider*)body1->collider);
			for (auto& contact : contacts) {
				if (body2->physicsEnabled)
					contact.bodies[0] = body2;
				contact.bodies[1] = nullptr;
			}
			return contacts;
		}
	}

	std::vector<Contact> CollisionDetector::_contactBoxAndHalfSpace(
		BoxCollider* box,
		HalfSpaceCollider* plane
	)
	{

		// We have an intersection, so find the intersection points. We can make
		// do with only checking vertices. If the box is resting on a plane
		// or on an edge, it will be reported as four or two contact points.

		// Go through each combination of + and - for each half-size
		static float mults[8][3] = { {1,1,1},{-1,1,1},{1,-1,1},{-1,-1,1},
								   {1,1,-1},{-1,1,-1},{1,-1,-1},{-1,-1,-1} };

		std::vector<Contact> contacts;
		for (unsigned i = 0; i < 8; i++) {

			// Calculate the position of each vertex
			mathfu::vec3 vertexPos(mults[i][0], mults[i][1], mults[i][2]);
			vertexPos = vertexPos * box->half_size;

			//auto test = vertexPos * box->transform.ToRotationMatrix(box->transform) + box->transform.TranslationVector3D();

			auto test = box->transform * vertexPos;

			vertexPos = test;

			// Calculate the distance from the plane
			float vertexDistance = mathfu::vec3::DotProduct(vertexPos, plane->direction);

			// Compare this to the plane's distance
			if (vertexDistance <= plane->offset)
			{
				// Create the contact data.

				// The contact point is halfway between the vertex and the
				// plane - we multiply the direction by half the separation
				// distance and add the vertex location.
				Contact contact;
				contact.contact_point = plane->direction;
				contact.contact_point *= (vertexDistance - plane->offset);
				contact.contact_point += vertexPos;
				contact.contact_normal = plane->direction;
				contact.penetration_depth = plane->offset - vertexDistance;

				// Write the appropriate data
				contacts.push_back(contact);
				// Move onto the next contact
			}
		}

		return contacts;
	};



	mathfu::vec3 contactPoint(
		mathfu::vec3 pOne,
		mathfu::vec3 dOne,
		float oneSize,
		mathfu::vec3 pTwo,
		mathfu::vec3 dTwo,
		float twoSize,
		bool useOne
		) 
	{
		mathfu::vec3 toSt, cOne, cTwo;
		float dpStaOne, dpStaTwo, dpOneTwo, smOne, smTwo;
		float denom, mua, mub;

		smOne = dOne.LengthSquared();
		smTwo = dTwo.LengthSquared();
		dpOneTwo = mathfu::vec3::DotProduct(dTwo, dOne);

		toSt = pOne - pTwo;
		dpStaOne = mathfu::vec3::DotProduct(dOne, toSt);
		dpStaTwo = mathfu::vec3::DotProduct(dTwo, toSt);

		denom = smOne * smTwo - dpOneTwo * dpOneTwo;

		// Zero denominator indicates parrallel lines
		if (abs(denom) < 0.0001f) {
			return useOne ? pOne : pTwo;
		}

		mua = (dpOneTwo * dpStaTwo - smTwo * dpStaOne) / denom;
		mub = (smOne * dpStaTwo - dpOneTwo * dpStaOne) / denom;

		// If either of the edges has the nearest point out
		// of bounds, then the edges aren't crossed, we have
		// an edge-face contact. Our point is on the edge, which
		// we know from the useOne parameter.
		if (mua > oneSize ||
			mua < -oneSize ||
			mub > twoSize ||
			mub < -twoSize)
		{
			return useOne ? pOne : pTwo;
		}
		else
		{
			cOne = pOne + dOne * mua;
			cTwo = pTwo + dTwo * mub;

			return cOne * 0.5f + cTwo * 0.5f;
		}
	}

	float transformToAxis(
		BoxCollider* box,
		mathfu::vec3 axis
	)
	{
		return
			box->half_size.x * abs(mathfu::vec3::DotProduct(axis, box->GetAxis(0))) +
			box->half_size.y * abs(mathfu::vec3::DotProduct(axis, box->GetAxis(1))) +
			box->half_size.z * abs(mathfu::vec3::DotProduct(axis, box->GetAxis(2)));
	}

	float penetrationOnAxis(
		BoxCollider* one,
		BoxCollider* two,
		mathfu::vec3 axis,
		mathfu::vec3 toCenter)
	{
		float oneProject = transformToAxis(one, axis);
		float twoProject = transformToAxis(two, axis);

		// Project this onto the axis
		float distance = abs(mathfu::vec3::DotProduct(toCenter, axis));

		// Return the overlap (i.e. positive indicates
		// overlap, negative indicates separation).
		return oneProject + twoProject - distance;
	}

	bool tryAxis(
		BoxCollider* one,
		BoxCollider* two,
		mathfu::vec3 axis,
		mathfu::vec3 toCenter,
		unsigned index,
		float& smallestPenetration,
		unsigned& smallestCase
		)
	{
		if (axis.LengthSquared() < 0.0001) return true;
		axis.Normalize();

		float penetration = penetrationOnAxis(one, two, axis, toCenter);

		if (penetration < 0) return false;
		if (penetration < smallestPenetration) {
			smallestPenetration = penetration;
			smallestCase = index;
		}
		return true;
	}

	void fillPointFaceBoxBox(
		BoxCollider* one,
		BoxCollider* two,
		mathfu::vec3 toCenter,
		std::vector<Contact>& data,
		unsigned best,
		float pen
	)
	{
		// This method is called when we know that a vertex from
	// box two is in contact with box one.

		Contact contact;

		// We know which axis the collision is on (i.e. best),
		// but we need to work out which of the two faces on
		// this axis.
		mathfu::vec3 normal = one->GetAxis(best);
		if (mathfu::vec3::DotProduct(one->GetAxis(best), toCenter) > 0)
		{
			normal = normal * -1.0f;
		}

		// Work out which vertex of box two we're colliding with.
		// Using toCentre doesn't work!
		mathfu::vec3 vertex = two->half_size;
		if (mathfu::vec3::DotProduct(two->GetAxis(0), normal) < 0) vertex.x = -vertex.x;
		if (mathfu::vec3::DotProduct(two->GetAxis(1), normal) < 0) vertex.y = -vertex.y;
		if (mathfu::vec3::DotProduct(two->GetAxis(2), normal) < 0) vertex.z = -vertex.z;

		// Create the contact data
		contact.contact_normal = normal;
		contact.penetration_depth = pen;
		contact.contact_point = (two->transform * vertex);
		data.push_back(contact);
	}

	#define CHECK_OVERLAP(axis, index) \
		if(!tryAxis(one, two, (axis), toCenter, (index), pen, best)) return contacts;

	std::vector<Contact> CollisionDetector::_contactBoxAndBox(BoxCollider* one, BoxCollider* two) {
		std::vector<Contact> contacts;

		mathfu::vec3 toCenter = two->GetAxis(3) - one->GetAxis(3);

		float pen = FLT_MAX;
		unsigned best = 0xffffff;

		CHECK_OVERLAP(one->GetAxis(0), 0);
		CHECK_OVERLAP(one->GetAxis(1), 1);
		CHECK_OVERLAP(one->GetAxis(2), 2);

		CHECK_OVERLAP(two->GetAxis(0), 3);
		CHECK_OVERLAP(two->GetAxis(1), 4);
		CHECK_OVERLAP(two->GetAxis(2), 5);

		unsigned bestSingleAxis = best;
		

		CHECK_OVERLAP(mathfu::vec3::CrossProduct(one->GetAxis(0), two->GetAxis(0)), 6);
		CHECK_OVERLAP(mathfu::vec3::CrossProduct(one->GetAxis(0), two->GetAxis(1)), 7);
		CHECK_OVERLAP(mathfu::vec3::CrossProduct(one->GetAxis(0), two->GetAxis(2)), 8);
		CHECK_OVERLAP(mathfu::vec3::CrossProduct(one->GetAxis(1), two->GetAxis(0)), 9);
		CHECK_OVERLAP(mathfu::vec3::CrossProduct(one->GetAxis(1), two->GetAxis(1)), 10);
		CHECK_OVERLAP(mathfu::vec3::CrossProduct(one->GetAxis(1), two->GetAxis(2)), 11);
		CHECK_OVERLAP(mathfu::vec3::CrossProduct(one->GetAxis(2), two->GetAxis(0)), 12);
		CHECK_OVERLAP(mathfu::vec3::CrossProduct(one->GetAxis(2), two->GetAxis(1)), 13);
		CHECK_OVERLAP(mathfu::vec3::CrossProduct(one->GetAxis(2), two->GetAxis(2)), 14);

		if (best < 3)
		{
			// We've got a vertex of box two on a face of box one.
			fillPointFaceBoxBox(one, two, toCenter, contacts, best, pen);
			return contacts;
		}
		else if (best < 6)
		{
			// We've got a vertex of box one on a face of box two.
			// We use the same algorithm as above, but swap around
			// one and two (and therefore also the vector between their
			// centres).
			fillPointFaceBoxBox(two, one, toCenter * -1.0f, contacts, best - 3, pen);
			return contacts;
		}
		else
		{
			// We've got an edge-edge contact. Find out which axes
			best -= 6;
			unsigned oneAxisIndex = best / 3;
			unsigned twoAxisIndex = best % 3;

			mathfu::vec3 oneAxis = one->GetAxis(oneAxisIndex);
			mathfu::vec3 twoAxis = two->GetAxis(twoAxisIndex);
			mathfu::vec3 axis = mathfu::vec3::CrossProduct(oneAxis, twoAxis);
			axis.Normalize();

			// The axis should point from box one to box two.
			if (mathfu::vec3::DotProduct(axis, toCenter) > 0) axis = axis * -1.0f;

			// We have the axes, but not the edges: each axis has 4 edges parallel
			// to it, we need to find which of the 4 for each object. We do
			// that by finding the point in the centre of the edge. We know
			// its component in the direction of the box's collision axis is zero
			// (its a mid-point) and we determine which of the extremes in each
			// of the other axes is closest.
			mathfu::vec3 ptOnOneEdge = one->half_size;
			mathfu::vec3 ptOnTwoEdge = two->half_size;
			for (unsigned i = 0; i < 3; i++)
			{
				if (i == oneAxisIndex) ptOnOneEdge[i] = 0;
				else if (mathfu::vec3::DotProduct(one->GetAxis(i), axis) > 0) ptOnOneEdge[i] = -ptOnOneEdge[i];

				if (i == twoAxisIndex) ptOnTwoEdge[i] = 0;
				else if (mathfu::vec3::DotProduct(two->GetAxis(i), axis) < 0) ptOnTwoEdge[i] = -ptOnTwoEdge[i];
			}

			// Move them into world coordinates (they are already oriented
			// correctly, since they have been derived from the axes).
			ptOnOneEdge = one->transform * ptOnOneEdge;
			ptOnTwoEdge = two->transform * ptOnTwoEdge;

			// So we have a point and a direction for the colliding edges.
			// We need to find out point of closest approach of the two
			// line-segments.
			mathfu::vec3 vertex = contactPoint(
				ptOnOneEdge, oneAxis, one->half_size[oneAxisIndex],
				ptOnTwoEdge, twoAxis, two->half_size[twoAxisIndex],
				bestSingleAxis > 2
			);

			// We can fill the contact.
			Contact contact;

			contact.penetration_depth = pen;
			contact.contact_normal = axis;
			contact.contact_point = vertex;
			contacts.push_back(contact);
		}
		return contacts;
	}
}
