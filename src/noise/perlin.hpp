/* Perlin's noise algorithm
 * 
 * usage :
 *  return the noise value for the given coordinates
 *  pass non int values or it won't work.
 *  divide at least by 5 or it won't look nice.
 *
 * example :
 *  int x = 100;
 *  int y = 50;
 *
 *  float p[2] = {x+0.5, y+0.5}
 *
 *  const float resolution = 15.0;
 *
 *  float noise = noise2(p[0]/ resolution);
 */
#pragma once
#ifndef _PERLIN_HPP
#define	_PERLIN_HPP

/// Set or Reset the seed (you probably don't need to call that function at all!)
void reseed(int seed);

float noise1(float arg);
float noise2(float vec[2]);
float noise3(float vec[3]); /* The simplex noise algorithm is much more efficient in 3D */

#endif	/* _PERLIN_HPP */
