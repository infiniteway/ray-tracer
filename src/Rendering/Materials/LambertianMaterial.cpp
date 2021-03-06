#include "LambertianMaterial.h"

LambertianMaterial::LambertianMaterial(glm::vec3 color, float _emissivity, float _reflectivity,
									   float _transparency, float _refractiveIndex, float _specularity, float _specularExponent) :
	surfaceColor(color), Material(_emissivity, _reflectivity, _transparency, _refractiveIndex, _specularity, _specularExponent) {}

glm::vec3 LambertianMaterial::GetSurfaceColor() const { return surfaceColor; }

glm::vec3 LambertianMaterial::CalculateDiffuseLighting(const glm::vec3 & inDirection,
													   const glm::vec3 & outDirection,
													   const glm::vec3 & normal,
													   const glm::vec3 & incomingRadiance) const {
	assert(glm::length(inDirection) < 1.0f + 10.0f * FLT_EPSILON && glm::length(inDirection) > 1.0f - 10.0f * FLT_EPSILON);
	assert(glm::length(outDirection) < 1.0f + 10.0f * FLT_EPSILON && glm::length(outDirection) > 1.0f - 10.0f * FLT_EPSILON);
	return glm::max(0.0f, glm::dot(-inDirection, normal)) * (incomingRadiance * surfaceColor);
}


