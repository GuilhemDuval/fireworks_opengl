#pragma once

#include <iostream>
#include <vector>

class MarkovChain {
private:
    std::vector<std::vector<double>> transition_matrix;
    std::vector<double>              current_state;
    std::vector<int>                 state_counts;

public:
    MarkovChain(const std::vector<std::vector<double>>& transition_matrix, const std::vector<double>& initial_state);

    void check_probability_sum(const std::vector<double>& probabilities, const std::string& error_message) const;

    void transition_probabilities();

    void collapse_values();

    void transition_values();

    const std::vector<double>& get_current_state() const;

    const std::vector<int>& get_state_counts() const;

    int get_number_of_states() const;

    void display_current_state();

    void display_state_counts();

    int get_deterministic_current_state();

    std::vector<double> calculate_stationary_distribution();
};

// Function to print the content of a container
template<typename T>
void print_container(const T& container)
{
    for (const auto& element : container)
    {
        std::cout << element << " ";
    }
    std::cout << std::endl;
}