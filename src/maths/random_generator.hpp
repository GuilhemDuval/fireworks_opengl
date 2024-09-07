#pragma once

#include <vector>

unsigned long long binomial_coefficient(int n, int k);

bool   bernoulli_distribution(double p);
double uniform_distribution(double lower_bound, double upper_bound);
int    discrete_uniform_distribution(int lower_bound, int upper_bound);

int binomial_distribution(int n, double p);
int binomial_distribution_cdf(int n, double p);

double exponential_distribution(double lambda);
double laplace_distribution(double mu, double b);

void normal_distribution(std::vector<double>& results, double average, double quarterType);
void normal_distribution_polar(std::vector<double>& results, double average, double quarterType);

double beta_distribution(double alpha, double beta);
