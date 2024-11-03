#ifndef SHADOW_RAY_GLSL
#define SHADOW_RAY_GLSL
#include "random.glsl"

float ShadowRay(vec3 origin, vec3 direction, float dist) {
  rayQueryEXT rq;
  rayQueryInitializeEXT(rq, scene, gl_RayFlagsTerminateOnFirstHitEXT, 0xFF,
                        origin, length(origin) * 1e-3, direction, dist * 0.999);
  rayQueryProceedEXT(rq);

  if (rayQueryGetIntersectionTypeEXT(rq, true) !=
      gl_RayQueryCommittedIntersectionNoneEXT) {
    return 0.0f;
  }
  return 1.0;
}

#endif
