#ifndef DEFAULTS_HPP_BF9C692C_BF14_459C_B203_B0ED7DD3CB94
#define DEFAULTS_HPP_BF9C692C_BF14_459C_B203_B0ED7DD3CB94

#include <chrono>
#include <random>

/* Select default random number generator
 *
 * We don't need very high quality random numbers in this case, so we can
 * opt for a fast (but fairly low quality) LCG-based generator.
 */
using RNG = std::minstd_rand;

/* Select default clock
 *
 * The steady clock is a reasonable choice. It is monotonic, meaning that time
 * will never seem to go backwards (this can happen, e.g., during daylight
 * savings switches, or due to adjustments from something like NTP).
 *
 * std::chrono::high_resolution_clock is a reasonable alternative.
 */
using Clock = std::chrono::steady_clock;

/* Alias: time duration in seconds, as a float.
 *
 * Don't use for long durations (e.g., time since application start or
 * similar), as float accuracy decreases quickly.
 */
using Secondsf = std::chrono::duration<float, std::ratio<1>>;

#endif // DEFAULTS_HPP_BF9C692C_BF14_459C_B203_B0ED7DD3CB94
