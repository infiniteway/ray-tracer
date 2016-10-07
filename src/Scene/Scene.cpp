#include "Scene.h"

#include <memory>
#include <algorithm>
#include <cassert>
#include <iostream>

#include "../../includes/glm/gtx/norm.hpp"
#include "../../includes/glm/gtx/rotate_vector.hpp"

#include "../Rendering/Materials/LambertianMaterial.h"
#include "../Geometry/Sphere.h"

using namespace std;

Scene::Scene() {}

Scene::~Scene() {
	for (auto& rg : renderGroups) {
		for (auto& prim : rg.primitives) {
			delete prim;
		}
	}
	for (auto m : materials) {
		delete m;
	}
}

void Scene::Initialize() {
	for (unsigned int i = 0; i < renderGroups.size(); ++i) {
		if (renderGroups[i].material->IsEmissive()) {
			emissiveRenderGroups.push_back(i);
		}
	}
}

glm::vec3 Scene::TraceRay(const Ray & ray, const unsigned int bouncesPerHit, const unsigned int depth) const {
	if (depth == 0) { return glm::vec3(0, 0, 0); }

	assert(depth > 0);
	assert(bouncesPerHit > 0);

	unsigned int intersectionPrimitiveIndex, intersectionRenderGroupIndex;
	float intersectionDistance;
	bool intersectionFound = RayCast(ray, intersectionRenderGroupIndex, intersectionPrimitiveIndex, intersectionDistance);

	/* If the ray doesn't intersect, simply return (0, 0, 0). */
	if (!intersectionFound) { return glm::vec3(0, 0, 0); }

	/* Retrieve primitive and material information for the intersected object. */
	const auto & intersectionRenderGroup = renderGroups[intersectionRenderGroupIndex];
	Material* hitMaterial = intersectionRenderGroup.material;
	if (hitMaterial->IsEmissive()) {
		return intersectionRenderGroup.material->GetEmissionColor();
	}
	const auto & intersectionPrimitive = intersectionRenderGroup.primitives[intersectionPrimitiveIndex];

	/* Calculate intersection point and normal. */
	glm::vec3 intersectionPoint = ray.from + ray.dir * intersectionDistance;
	assert(glm::distance(ray.from, intersectionPoint) > FLT_EPSILON);
	glm::vec3 hitNormal = intersectionPrimitive->GetNormal(intersectionPoint);

	/* Shoot rays and integrate based on BRDF sampling. */
	glm::vec3 colorAccumulator = { 0,0,0 };
	std::vector<Ray> bouncingRays = GenerateBouncingRays(ray.dir, hitNormal, intersectionPoint, hitMaterial, bouncesPerHit, ray.length);
	for (const auto & bouncedRay : bouncingRays) {
		const auto incomingRadiance = TraceRay(bouncedRay, bouncesPerHit, depth - 1);
		colorAccumulator += hitMaterial->CalculateBRDF(bouncedRay.dir, ray.dir, hitNormal, incomingRadiance);
	}

	return (1.0f / (float)bouncesPerHit) * colorAccumulator;
}

bool Scene::RayCast(const Ray & ray, unsigned int & intersectionRenderGroupIndex,
					unsigned int & intersectionPrimitiveIndex, float & intersectionDistance) const {
	float closestInterectionDistance = FLT_MAX;
	for (unsigned int i = 0; i < renderGroups.size(); ++i) {
		for (unsigned int j = 0; j < renderGroups[i].primitives.size(); ++j) {
			/* Check if the ray intersects with anything. */
			bool intersects = renderGroups[i].primitives[j]->RayIntersection(ray, intersectionDistance);
			if (intersects) {
				if (intersectionDistance < closestInterectionDistance) {
					intersectionRenderGroupIndex = i;
					intersectionPrimitiveIndex = j;
					closestInterectionDistance = intersectionDistance;
				}
			}
		}
	}
	return closestInterectionDistance < FLT_MAX - FLT_EPSILON;
}

std::vector<Ray> Scene::GenerateBouncingRays(const glm::vec3 & incomingDirection,
											 const glm::vec3 & hitNormal,
											 const glm::vec3 & intersectionPoint,
											 Material * material,
											 const unsigned int numberOfRays,
											 const float rayLength) const {
	std::vector<Ray> result;
	const glm::vec3 reflection = reflect(incomingDirection, hitNormal);
	for (unsigned int i = 0; i < numberOfRays; ++i) {
		glm::vec3 reflectionDirection = Math::CosineWeightedHemisphereSampleDirection(hitNormal);
		result.push_back(Ray(intersectionPoint, intersectionPoint + reflectionDirection * rayLength));
	}
	return result;

	// const glm::vec3 orthogonalToReflection = normalize(glm::cross(reflection, Math::NonParallellVector(reflection)));

	// Always shoot towards the light => less rays needed for a neat image.
	/*
	for (unsigned int i = 0; i < emissiveRenderGroups.size(); ++i) {
		const auto & grp = renderGroups[emissiveRenderGroups[i]];
		for (unsigned int j = 0; j < grp.primitives.size(); ++j) {
			const auto & prim = grp.primitives[j];
			glm::vec3 newRayDirection = glm::normalize(prim->GetCenter() - intersectionPoint);
			if (dot(newRayDirection, hitNormal) > 0) {
				result.push_back(Ray(intersectionPoint, intersectionPoint + newRayDirection * rayLength));
			}
		}
	}*/

	// The technique we use is that we rotate the reflection using an inclination angle
	// and then an azimuth angle. The azimuth and inclination angles are given by the 
	// material and it's distribution function. Thus we can have different light bounce
	// behaviour for different materials.
	// for (unsigned int i = 0; i < numberOfRays; ++i) {

		// glm::vec3 newRayDirection = Math::CosineWeightedHemisphereSampleDirection(hitNormal);
		// result.push_back(Ray(intersectionPoint, intersectionPoint + newRayDirection * rayLength));

		/*
		// Fetch angles from the material (using it's distribution functions).
		const float azimuthAngle = material->AzimuthDistributionFunction();
		const float inclinationAngle = material->InclinationDistributionFunction();

		// Calculate a new ray direction using the azimuth and inclination angles.
		glm::vec3 newRayDirection = glm::rotate(reflection,
												inclinationAngle,
												orthogonalToReflection);
		newRayDirection = glm::rotate(newRayDirection, azimuthAngle, reflection);

		// Check if the new ray is going through the material.
		// (if it is, then we could check the refraction index of the material).
		if (glm::dot(newRayDirection, hitNormal) > 0) {

			break;
		}
		*/
		// }


}