#pragma once

namespace Burst {
	namespace Physics {
		bool AABB_AABB(Vector3 pos1, Vector3 scale1, Vector3 pos2, Vector3 scale2) {
			float x1min = pos1.x - (scale1.x / 2.0f);
			float y1min = pos1.y - (scale1.y / 2.0f);
			float z1min = pos1.z - (scale1.z / 2.0f);

			float x1max = pos1.x + (scale1.x / 2.0f);
			float y1max = pos1.y + (scale1.y / 2.0f);
			float z1max = pos1.z + (scale1.z / 2.0f);

			float x2min = pos2.x - (scale2.x / 2.0f);
			float y2min = pos2.y - (scale2.y / 2.0f);
			float z2min = pos2.z - (scale2.z / 2.0f);

			float x2max = pos2.x + (scale2.x / 2.0f);
			float y2max = pos2.y + (scale2.y / 2.0f);
			float z2max = pos2.z + (scale2.z / 2.0f);
			return (x1min < x2max&& x1max > x2min && y1min < y2max&& y1max > y2min && z1min < z2max&& z1max > z2min);
		}

		bool AABB_Sphere(Vector3 pos1, Vector3 scale1, Vector3 pos2, float radius) {
			float x1min = pos1.x - (scale1.x / 2.0f);
			float y1min = pos1.y - (scale1.y / 2.0f);
			float z1min = pos1.z - (scale1.z / 2.0f);

			float x1max = pos1.x + (scale1.x / 2.0f);
			float y1max = pos1.y + (scale1.y / 2.0f);
			float z1max = pos1.z + (scale1.z / 2.0f);

			Vector3 q;

			float v = pos2.x;
			if (v < x1min) v = x1min;
			if (v > x1max) v = x1max;
			q.x = v;

			v = pos2.y;
			if (v < y1min) v = y1min;
			if (v > y1max) v = y1max;
			q.y = v;

			v = pos2.z;
			if (v < z1min) v = z1min;
			if (v > z1max) v = z1max;
			q.z = v;

			radius /= 2.0f;

			return Vector3::Distance(q, pos2) < radius * radius;
		}

		bool Sphere_Sphere(Vector3 pos1, float radius1, Vector3 pos2, float radius2) {
			float distance = Vector3::Distance(pos1, pos2);
			float r = radius1 / 2.0f + radius2 / 2.0f;
			return distance < r* r;
		}

		bool Cube_Sphere(Vector3 pos1, Vector3 rot1, Vector3 scale1, Vector3 pos2, float radius) {
			Vector3 translate = pos1 * -1;
			pos1 = pos1 + translate;
			pos2 = pos2 + translate;
			pos2.Rotate(rot1);
			return AABB_Sphere(pos1, scale1, pos2, radius);
		}

		bool Cube_Cube(Vector3 pos1, Vector3 rot1, Vector3 scale1, Vector3 pos2, Vector3 rot2, Vector3 scale2) {
			Vector3 translate = pos1 * -1;
			pos1 = pos1 + translate;
			pos2 = pos2 + translate;
			pos2.Rotate(rot1);
			rot2 = rot2 - rot1;
			return AABB_AABB(pos1, scale1, pos2, scale2);
		}

		bool Line_AABB(Vector3 line1, Vector3 line2, Vector3 pos, Vector3 scale) {
			return false;
		}

		bool Line_Sphere(Vector3 line1, Vector3 line2, Vector3 pos, float radius) {
			return false;
		}

		bool Line_Cube(Vector3 line1, Vector3 line2, Vector3 pos, Vector3 rot, Vector3 scale) {
			Vector3 translate = pos * -1.0f;
			pos = pos + translate;
			line1 = line1 + translate;
			line2 = line2 + translate;
			line1.Rotate(rot);
			line2.Rotate(rot);
			return Line_AABB(line1, line2, pos, scale);
		}
	}
}