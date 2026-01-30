#include "misc/Collision.hpp"

#include <math.h>

#include "misc/NumberUtils.hpp"

bool Collision_BoxIntersect(Box a, Box b, int ignore_faces,
                            mc::Vector3f* ncoll,  // normal of collision.
			    float* dcoll,   // depth of intersection.
			    int* fcoll)     // face intersected.

{
	// the normal of each face.
	static const mc::Vector3f faces[6] = {
        mc::Vector3f(-1, 0, 0),  // 'left' face normal (-x direction)
        mc::Vector3f(1, 0, 0),   // 'right' face normal (+x direction)
        mc::Vector3f(0, -1, 0),  // 'bottom' face normal (-y direction)
        mc::Vector3f(0, 1, 0),   // 'top' face normal (+y direction)
        mc::Vector3f(0, 0, -1),  // 'far' face normal (-z direction)
        mc::Vector3f(0, 0, 1),   // 'near' face normal (+z direction)
    };

	// distance of collided box to the face.
	float distances[6] = {
	    (b.max.x - a.min.x),  // distance of box 'b' to face on 'left' side of 'a'.
	    (a.max.x - b.min.x),  // distance of box 'b' to face on 'right' side of 'a'.
	    (b.max.y - a.min.y),  // distance of box 'b' to face on 'bottom' side of 'a'.
	    (a.max.y - b.min.y),  // distance of box 'b' to face on 'top' side of 'a'.
	    (b.max.z - a.min.z),  // distance of box 'b' to face on 'far' side of 'a'.
	    (a.max.z - b.min.z),  // distance of box 'b' to face on 'near' side of 'a'.
	};

	// scan each face, make sure the box intersects,

	// and take the face with least amount of intersection

	// as the collided face.

	for (int i = 0; i < 6; i++) {
		// box does not intersect face. So boxes don't intersect at all.

		if (distances[i] < 0.0f) return false;

		if (ignore_faces & (1 << i)) continue;

		// face of least intersection depth. That's our candidate.

		if ((i == 0) || (distances[i] < *dcoll)) {
			*fcoll = i;
			*ncoll = faces[i];
			*dcoll = distances[i];
		}
	}

	return true;
}

Box Box_Create(float x, float y, float z, float w, float h, float d) {
    return (Box){mc::Vector3f(x, y, z), mc::Vector3f(x + w, y + h, z + d)};
}

bool Box_Contains(Box box, float x, float y, float z) {
    return box.min.x <= x && box.min.y <= y && box.min.z <= z && box.max.x > x && box.max.y > y && box.max.z > z;
}
