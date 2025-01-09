#include "math/utils.h"

namespace Everest {
    /*transforms point to given data, transform.rotation is in radians*/
    vec3 Math::transformOrtho(vec3 point, vec3 position, vec2 scale, f32 rotation){
        const f32 _sn = glm::sin(rotation);
        const f32 _cs = glm::cos(rotation);

        f32 px, py;
        px = point.x * scale.x;
        py = point.y * scale.y;
        point.x = px * _cs - py * _sn;
        point.y = px * _sn + py * _cs;

        return point + position;
    }

    bool Math::decomposeTransform(transform_c& transform, mat4& transformMatrix){
        using namespace glm;

        mat4 locmat(transformMatrix);

		// Normalize the matrix.
		if (epsilonEqual(locmat[3][3], static_cast<float>(0), epsilon<f32>()))
			return false;

		// First, isolate perspective.  This is the messiest.
		if (
			epsilonNotEqual(locmat[0][3], static_cast<f32>(0), epsilon<f32>()) ||
			epsilonNotEqual(locmat[1][3], static_cast<f32>(0), epsilon<f32>()) ||
			epsilonNotEqual(locmat[2][3], static_cast<f32>(0), epsilon<f32>())
            ) {
			// Clear the perspective partition
			locmat[0][3] = locmat[1][3] = locmat[2][3] = static_cast<f32>(0);
			locmat[3][3] = static_cast<f32>(1);
		}

		// Next take care of transform.position (easy).
		transform.position = vec3(locmat[3]);
		locmat[3] = vec4(0, 0, 0, locmat[3].w);

		vec3 Row[3], Pdum3;

		// Now get transform.scale and shear.
		for (length_t i = 0; i < 3; ++i)
			for (length_t j = 0; j < 3; ++j)
				Row[i][j] = locmat[i][j];

		// Compute X transform.scale factor and normalize first row.
		transform.scale.x = length(Row[0]);
		Row[0] = detail::scale(Row[0], static_cast<f32>(1));
		transform.scale.y = length(Row[1]);
		Row[1] = detail::scale(Row[1], static_cast<f32>(1));
		transform.scale.z = length(Row[2]);
		Row[2] = detail::scale(Row[2], static_cast<f32>(1));

		// At this point, the matrix (in rows[]) is orthonormal.
		// Check for a coordinate system flip.  If the determinant
		// is -1, then negate the matrix and the scaling factors.

		transform.rotation.y = asin(-Row[0][2]);
		if (cos(transform.rotation.y) != 0) {
			transform.rotation.x = atan2(Row[1][2], Row[2][2]);
			transform.rotation.z = atan2(Row[0][1], Row[0][0]);
		}
		else {
			transform.rotation.x = atan2(-Row[2][0], Row[1][1]);
			transform.rotation.z = 0;
		}
        transform.rotation = glm::degrees(transform.rotation);


		return true;
    }
    vec3 Math::getCameraForward(transform_c& transform){
        f32 alpha = glm::radians(transform.rotation.x);
        f32 beta   = glm::radians(transform.rotation.y);
        f32 sinb = glm::sin(beta), cosb = glm::cos(beta),
            sina = glm::sin(alpha), cosa = glm::cos(alpha);
        vec3 forward = vec3(-cosa * sinb, sina, -cosa * cosb);
        return glm::normalize(forward);
    }
}
