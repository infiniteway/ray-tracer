#pragma once

namespace Math {
	/// <summary> Returns an interpolated value between 4 corner values. </summary>
	/// <param name='dy'> 
	/// The amount interpolated in the y-direction (on the "width axis"). 
	/// Must be normalized to [0,1]. 
	/// </param>
	/// <param name='dz'> 
	/// The amount interpolated in the z-direction (on the "height axis"). 
	/// Must be normalized to [0,1]. 
	/// </param>
	/// <param name='x1'> The value of the "lower left" point. </param>
	/// <param name='x2'> The value of the "upper left" point. </param>
	/// <param name='x3'> The value of the "upper right" point. </param>
	/// <param name='x4'> The value of the "lower right" point. </param>
	float InterpolationQuad4f(float dy, float dz, float x1, float x2, float x3, float x4);
}