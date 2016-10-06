#pragma once

#include <glm.hpp>

class Material {
public:
	//The intensity of the light of this material (should generally be 0 if not a lightsource).
	glm::vec3 lightIntensity;

	/// <summary> 
	/// Calculates the BRDF value of this material. 
	/// In other words calculates the outgoing radiance. 
	/// </summary>
	/// <param name='inDirection'> The incoming direction of the light.</param>
	/// <param name='outDirection'> The outgoing direction of the light.</param>
	/// <param name='normal'> The normal of the surface.</param>
	/// <param name='incomingIntensity'> The intensity of the incoming light.</param>
	/// <param name='surfaceColor'> The color of the surface.</param>
	/// <returns> The BRDF value of this material. </returns>
	virtual glm::vec3 CalculateBRDF(glm::vec3& inDirection, glm::vec3& outDirection,
									glm::vec3& normal, glm::vec3 & incomingIntensity,
									glm::vec3 & surfaceColor) const = 0;
};