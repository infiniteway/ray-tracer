#pragma once
#include "../Scene/Scene.h"
#include "Octree.h"

class PhotonMap
{
public:

	/// <summary> 
	/// Shoots all photons in the scene and saves them in a Octree.
	/// </summary>
	/// <param name='scene'> The scene. </param>
	/// <param name='PHOTONS_PER_LIGHT_SOURCE'> The amount of photons used per light source. </param>
	/// <param name='MAX_PHOTONS_PER_NODE'> The maximum amount of photons per node. </param>
	/// <param name='MINIMUM_NODE_BOX_DIMENSION'> The minimum width, height and depth of a nodes box size. </param>
	/// <param name='MAX_DEPTH'> The amount of times each photon will bounce at most. </param>
	void CreatePhotonMap(const Scene & scene,
						 const unsigned int PHOTONS_PER_LIGHT_SOURCE,
						 const unsigned int MAX_PHOTONS_PER_NODE,
						 const float MINIMUM_NODE_BOX_DIMENSION,
						 const unsigned int MAX_DEPTH);

	/// <summary> 
	/// Returns all photons located at the given world position.
	/// </summary>
	/// <param name='pos'> The position to search around. </param>
	std::vector<Photon*> & GetPhotonsAtPosition(const glm::vec3 & pos);

private:

	// The data structure photons are saved in.
	Octree octree;
};




