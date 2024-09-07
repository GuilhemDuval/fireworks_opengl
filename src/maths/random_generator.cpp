/*
    - unsigned long long binomial_coefficient(int n, int k)

    - bool bernoulli_distribution(double p)
    - double uniform_distribution(double lower_bound, double upper_bound)
    - int discrete_uniform_distribution(int lower_bound, int upper_bound)

    - int binomial_distribution(int n, double p)
    - int binomial_distribution_cdf(int n, double p)

    - int exponential_distribution(double lambda)
    - double laplace_distribution(double mu, double b)

    - void normal_distribution(std::vector<double>& results, double average, double quarterType)
    - void normal_distribution_polar(std::vector<double>& results, double average, double quarterType)

    - double beta_distribution(double alpha, double beta)
*/

#include "random_generator.hpp"
#include <cmath>
#include <cstdlib>
#define M_PI       3.14159265358979323846

// Calculate the binomial coefficient, (n k), representing the number of ways to choose k items from a set of n distinct items
unsigned long long binomial_coefficient(int n, int k)
{
    if (k < 0 || k > n)
    {
        return 0;
    }

    if (k == 0 || k == n)
    {
        return 1;
    }

    if (k == 1 || k == n - 1)
    {
        return n;
    }

    // Formula n!/((n-k)!k!) as a loop
    unsigned long long coeff = n;
    for (unsigned int i = 2; i <= k; i++)
    {
        coeff *= (n - i + 1);
        coeff /= i;
    }

    return coeff;
}

// Generate a random number between 0 and 1 uniformly
double generate_random()
{
    return static_cast<double>(rand()) / RAND_MAX;
}

// Simulate bernoulli distribution with p, probability of success
bool bernoulli_distribution(double p)
{
    double random_num = generate_random(); // Random number between 0 and 1

    return random_num < p; // Return 0 or 1 depending on the success
}

// Simulate uniform distribution with parameters lower_bound, the smallest result possible and upper_bound, the largest result possible
double uniform_distribution(double lower_bound, double upper_bound)
{
    double random_num = generate_random(); // Random number between 0 and 1

    // Scale and shift the random number to fit within the specified range
    return lower_bound + (upper_bound - lower_bound) * random_num;
}

// Simulate discrete uniform distribution
int discrete_uniform_distribution(int lower_bound, int upper_bound)
{
    int n          = upper_bound - lower_bound + 1; // Number of states
    int random_num = RAND_MAX;

    // Random number between 0 and a maximum divisible by n
    while (random_num > RAND_MAX - (RAND_MAX % n))
    {
        random_num = rand();
    };

    random_num %= n; // Scale the number to be between 0 and n-1

    // Shift the random number to fit within the specified range
    return lower_bound + random_num;
}

// Simulate binomial distribution by generating a number of success with n, number of trials, and p, probability of success, using multiples Bernoulli disribution
int binomial_distribution(int n, double p)
{
    int successes = 0;
    for (int i = 0; i < n; i++)
    {
        bernoulli_distribution(p) && successes++;
    }
    return successes;
}

// Simulate binomial distribution by generating a number of success with n, number of trials, and p, probability of success, using the cumulative distribution function method
int binomial_distribution_cdf(int n, double p)
{
    double success_probability = 0;
    double random_threshold    = (double)rand() / RAND_MAX; // Random number between 0 and 1

    for (int i = 0; i <= n; i++)
    {
        // Cumulative probability of obtaining i success in the sequence of trials
        success_probability += static_cast<double>(binomial_coefficient(n, i)) * pow(p, i) * pow(1 - p, n - i);

        // Check if the generated threshold is lower than the cumulative probability
        if (random_threshold < success_probability)
        {
            return i;
        }
    }
    return -1;
}

// Simulate exponential distribution by generating a time quotient, using inverse transform sampling
double exponential_distribution(double lambda)
{
    double u = generate_random(); // Random number between 0 and 1
    return -log(1 - u) / lambda;  // Application of the inverse distribution function
}

// Simulate Laplace distribution with parameters mu, location and b, scale, using inverse transform sampling
double laplace_distribution(double mu, double b)
{
    double u = generate_random() - 0.5; // Random number between -0.5 and 0.5

    if (u < 0)
    {
        // If u is less than 0, use the left side of the Laplace distribution
        return mu + b * log(1 - 2 * u);
    }
    // If u is greater than or equal to 0, use the right side of the Laplace distribution
    return mu - b * log(1 - 2 * u);
}

// Simulate normal distribution with parameters average and quarterType, using simple Box-Muller method
void normal_distribution(std::vector<double>& results, double average, double quarterType)
{
    double u = 0.0;
    double v = 0.0;

    // Ensure u and v aren't zero to prevent division by zero
    while (u == 0.0 || v == 0.0)
    {
        u = generate_random(); // Random number between 0 and 1
        v = generate_random(); // Random number between 0 and 1
    }

    // Box-Muller transform to generate normally distributed variables
    double z0 = sqrt(-2.0 * log(u)) * cos(2.0 * M_PI * v);
    double z1 = sqrt(-2.0 * log(u)) * sin(2.0 * M_PI * v);

    // Scale and shift the normal variables
    results.clear();
    results.push_back(z0 * sqrt(quarterType) + average);
    results.push_back(z1 * sqrt(quarterType) + average);
}

void normal_distribution_polar(std::vector<double>& results, double average, double quarterType)
{
    double x = 0.0;
    double y = 0.0;
    double u = 0.0;

    // Sample appropriate x and y to have u in ]0;1[
    while (u >= 1.0 || u == 0.0)
    {
        x = 2.0 * generate_random() - 1.0;
        y = 2.0 * generate_random() - 1.0;
        u = x * x + y * y;
    }

    // Apply Box-Muller transform
    double z0 = x * sqrt(-2.0 * log(u) / u);
    double z1 = y * sqrt(-2.0 * log(u) / u);

    // Scale and shift the normal variables
    results.clear();
    results.push_back(z0 * sqrt(quarterType) + average);
    results.push_back(z1 * sqrt(quarterType) + average);
}

// Simulate beta distribution with parameters alpha and beta, using inverse transform sampling
double beta_distribution(double alpha, double beta)
{
    const double u = generate_random(); // Random number between 0 and 1

    double       cdf          = 0.0; // Cumulative distribution function
    const double step         = 0.0001;
    double       x            = 0.0;
    double       beta_distrib = 0.0;

    const double gamma_alpha      = std::tgamma(alpha);
    const double gamma_beta       = std::tgamma(beta);
    const double gamma_alpha_beta = std::tgamma(alpha + beta);

    while (x <= 1.0)
    {
        // Calculation of the probability density function
        beta_distrib = (std::pow(x, alpha - 1) * std::pow(1 - x, beta - 1));
        beta_distrib = beta_distrib / (gamma_alpha * gamma_beta / gamma_alpha_beta);

        cdf += beta_distrib * step;

        if (cdf >= u)
        {
            return x;
        }

        x += step;
    }

    return x;
}

/*
int main()
{
    // Generate seed based on current time
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    // Test binomial_coefficient function
    std::cout << "Binomial Coefficient (5 choose 2): " << binomial_coefficient(5, 2) << std::endl;

    // Test bernoulli_distribution function
    double p = 0.5;
    std::cout << "Bernoulli Distribution (p = " << p << "): " << bernoulli_distribution(p) << std::endl;

    // Test uniform_distribution function
    double lower_bound = 0.0;
    double upper_bound = 1.0;
    std::cout << "Uniform Distribution (" << lower_bound << ", " << upper_bound << "): " << uniform_distribution(lower_bound, upper_bound) << std::endl;

    // Test discrete_uniform_distribution function
    int discrete_lower_bound = 0;
    int discrete_upper_bound = 255;
    std::cout << "Discrete Uniform Distribution (" << discrete_lower_bound << ", " << discrete_upper_bound << "): " << discrete_uniform_distribution(discrete_lower_bound, discrete_upper_bound) << std::endl;

    // Test binomial_distribution function
    int n = 10;
    p     = 0.3;
    std::cout << "Binomial Distribution (n = " << n << ", p = " << p << "): " << binomial_distribution(n, p) << std::endl;

    // Test binomial_distribution_cdf function
    std::cout << "Binomial Distribution (Monte Carlo) (n = " << n << ", p = " << p << "): " << binomial_distribution_cdf(n, p) << std::endl;

    // Test exponential_distribution function
    double lambda = 0.5;
    std::cout << "Exponential Distribution (lambda = " << lambda << "): " << exponential_distribution(lambda) << std::endl;

    // Test laplace_distribution function
    double mu = 0.0;
    double b  = 1.0;
    std::cout << "Laplace Distribution (mu = " << mu << ", b = " << b << "): " << laplace_distribution(mu, b) << std::endl;

    // Test normal_distribution function
    double average     = 0.0;
    double quarterType = 1.0;

    std::vector<double> results;
    normal_distribution(results, average, quarterType);
    std::cout << "Normal Distribution: " << results[0] << ", " << results[1] << "\n";

    // Test normal_distribution_polar function

    std::vector<double> results_polar;
    normal_distribution_polar(results_polar, average, quarterType);
    std::cout << "Normal Distribution Polar: " << results_polar[0] << ", " << results_polar[1] << "\n";

    // Test beta_distribution function
    double alpha      = 2.0;
    double beta_param = 3.0;
    std::cout << "Beta Distribution (alpha = " << alpha << ", beta = " << beta_param << "): " << beta_distribution(alpha, beta_param) << std::endl;

    return 0;
}
*/