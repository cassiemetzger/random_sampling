#pragma once

#include <cmath>
#include <cstdint>
#include <random>
#include <iostream>

// This random number generator is based off the xoshiro256++ algorithm:
// http://xoroshiro.di.unimi.it/
struct random_number_generator {
  uint64_t s[4] = {};
  uint64_t seed = 0;

  // Default constructor, initialize the rng with a random seed
  random_number_generator() {
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<uint64_t> dist;
    seed = dist(rng);
    std::cout << "Random Seed: " << std::hex << seed << std::dec << std::endl;
    init(seed);
  }

  // Constructor, initialize the rng with a specific seed
  random_number_generator(uint64_t seed) {
    this->seed = seed;
    init(seed);
  }

  // Initialize the rng with a given seed
  void init(uint64_t seed) {
    s[0] = split_mix_64(seed);
    s[1] = split_mix_64(s[0]);
    s[2] = split_mix_64(s[1]);
    s[3] = split_mix_64(s[2]);
  }

  double uniform() { return double(next()) / double(UINT64_MAX); }

  int uniform_int(int min, int max) { return min + (next() % (max - min));}

  double normal(double mu = 0.0, double sigma = 1.0) {
    double u1 = uniform();
    double u2 = uniform();
    double x = std::sqrt(-2.0 * std::log(u1)) * std::cos(2.0 * M_PI * u2); //normally distrubuted random number 
    x = x * sigma + mu; // shifted by mu and sigma so that depending on desired mean and std, the distribution will be changed 
    return x; 


    // TODO: Finish this implementation
  }

  /*  Written in 2018 by David Blackman and Sebastiano Vigna (vigna@acm.org)

  To the extent possible under law, the author has dedicated all copyright
  and related and neighboring rights to this software to the public domain
  worldwide. This software is distributed without any warranty.

  See <http://creativecommons.org/publicdomain/zero/1.0/>. */
  uint64_t next() {
    const uint64_t result = s[0] + s[3];

    const uint64_t t = s[1] << 17;

    s[2] ^= s[0];
    s[3] ^= s[1];
    s[1] ^= s[2];
    s[0] ^= s[3];

    s[2] ^= t;

    s[3] = rotl(s[3], 45);

    return result;
  }

  // Helper functions for the random number generator
  constexpr uint64_t rotl(const uint64_t x, int k) {
    return (x << k) | (x >> (64 - k));
  }

  constexpr uint64_t split_mix_64(uint64_t x) {
    uint64_t z = (x += 0x9e3779b97f4a7c15);
    z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9;
    z = (z ^ (z >> 27)) * 0x94d049bb133111eb;
    return z ^ (z >> 31);
  }

};
