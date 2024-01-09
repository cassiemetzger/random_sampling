#include "random.h"
#include <iostream>
#include <vector>

int main() {
  // Initialize the random number generator. If no seed is provided, the
  // generator will be given a random seed. To provide a seed, you can either do
  // rng.init(SOME_SEED) or specify it at construction time:
  // random_number_generator rng(SOME_SEED);
  random_number_generator rng;

  int N = 100'000;
  double mu = 1.0;
  double sigma = 0.5;

  // Generate N random numbers from a normal distribution
  std::vector<double> x(N);
  for (int i = 0; i < N; i++) {
    x[i] = rng.normal(mu, sigma);
  }

  // Compute the histogram
  std::vector<double> hist(10, 0);
  for (int i = 0; i < N; i++) {
    int bin =
        std::max(0.0, std::min((x[i] - mu) / sigma + hist.size() / 2.0,
                               hist.size() - 1.0));
    hist[bin] += 1.0;
  }

  // Print the histogram
  for (int i = -(int)hist.size() / 2; i < (int)hist.size() / 2; i++) {
    int bin = i + hist.size() / 2;
    if (i < -1) {
      std::cout << i + 1 << " sigma: ";
    } else if (i == -1) {
      std::cout << "-0 sigma: ";
    } else {
      std::cout << "+" << i << " sigma: ";
    }
    for (int j = 0; j < hist[bin] / (N / hist.size() / 10); j++) {
      if (hist[bin] > 0.0)
        std::cout << "#";
    }
    std::cout << hist[bin];
    std::cout << std::endl;
  }

  return 0;
}
