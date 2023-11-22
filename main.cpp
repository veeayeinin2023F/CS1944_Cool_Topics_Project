#include <iostream>
#include <string>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <chrono>


/// How many individuals a population should be comprised of.
static constexpr int POPULATION_SIZE = 100;

/// The target value for the mutations.
static const std::string TARGET = "Computer Science 1944 Cool Topics Project";

/// The chance for each value to mutate.
static constexpr double MUTATION_CHANCE = 0.01;

/// Lambda for generating a pseudo-random character.
static constexpr auto random_char = []() -> char {
    return rand() % CHAR_MAX;
};


/**
 * Evaluates a string's fitness score.
 *
 * @param individual The string to compare to the target.
 *
 * @return The fitness score of the string.
 */
double fitness(const std::string &individual) {

    int matches = 0;

    // Check how many values match.
    for (int i = 0; i < individual.length(); i++) {
        if (individual.at(i) == TARGET.at(i)) {
            matches++;
        }
    }

    return static_cast<double>(matches) / static_cast<double>(individual.length());

}


/**
 * Attempts to mutate characters within a string, and the chance to mutate is defined in {@link MUTATION_CHANCE}.
 *
 * @param individual The individual to mutate.
 *
 * @return The amount of mutations that occurred.
 */
int mutate(std::string &individual) {

    int mutations = 0;

    // Loop through each character in the string.
    for (char & c : individual) {

        // Generate a double within [0, 1].
        // const double random = (double) rand() / (double) RAND_MAX;

        // Check to see if the value should mutate.
        if (MUTATION_CHANCE >= static_cast<double>(rand()) / static_cast<double>(RAND_MAX)) {

            // Select a random character to mutate into.
            const char mutation = random_char();

            // Mutate the value.
            c = mutation;

            mutations++;

        }

    }

    return mutations;

}


/**
 * Finds the highest scoring individual in a population.
 *
 * @param population The population of individuals to search through.
 *
 * @return The index of the highest scoring individual in the population.
 */
int highest_scoring(const std::vector<std::string> &population) {

    double highest = fitness(population[0]);
    int index = 0;

    // Index 0 is excluded as it is used as the default.
    for (int i = 1; i < POPULATION_SIZE; i++) {

        // Check if the fitness score of the current individual is the highest.
        if (const double fitness_score = fitness(population[i]); fitness_score > highest) {
            highest = fitness_score;
            index = i;
        }

    }

    return index;

}


/**
 * The entry-point for the program. Utilizes a genetic algorithm to mutate a random string into the target string.
 *
 * @param argc The amount of arguments passed.
 * @param argv The arguments passed.
 *
 * @return The exit code.
 */
int main(const int argc, const char *argv[]) {

    // Should the program prompt for user input before terminating the program? This is useful is an external console
    // is used, as the console will close after the program terminates.
    bool pause = false;

    // Stores the arguments as a vector of strings.
    const std::vector<std::string> args(argv, argv + argc);

    // Loop over each argument, except for argv[0], because the program path is in argv[0].
    for (std::size_t i = 1; i < args.size(); i++) {
        if (args[i] == "--pause") {
            pause = true;
        }
    }

    // Seed the random number generator. Note, this is an old and non-uniform way to generate pseudo-random numbers, and
    // should not be used in most cases, unless it is for a proof-of-concept or similar.
    srand(static_cast<unsigned>(time(nullptr)));

    // The starting value for individuals.
    std::string current(TARGET.length(), 0);
    std::generate(current.begin(), current.end(), random_char);

    // Initializes a population.
    std::vector<std::string> population(POPULATION_SIZE);
    std::fill(population.begin(), population.end(), current);

    std::cout << "Population Size: " << POPULATION_SIZE << std::endl;
    std::cout << "Mutation Chance: " << (MUTATION_CHANCE * 100) << "%" << std::endl;

    // Get the time in which the program started.
    const auto start_time = std::chrono::high_resolution_clock::now();

    int generation = 0;
    for (;;) {

        // Increment to the next generation.
        generation++;

        // Attempt to mutate each individual in the population.
        std::for_each(population.begin(), population.end(), mutate);

        // Get the individual with the highest score.
        const int highest_scorer = highest_scoring(population);
        const std::string &value = population[highest_scorer];
        const double fitness_score = fitness(value);

        // Output the individual with the peak fitness score.
        std::cout << value << "  |  " << fitness_score << '\n';

        // If the algorithm is done, break out of the loop.
        if (fitness_score == 1) {
            break;
        }

        // Replace each individual with the peak individual.
        std::fill(population.begin(), population.end(), value);

    }

    // Calculate the total time elapsed since the program started.
    const auto end_time = std::chrono::high_resolution_clock::now();
    const auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
    std::cout << "Time Elapsed: " << duration.count() << "ms" << std::endl;

    std::cout << "Completed in " << generation << " generations." << std::endl;

    if (pause) {

        // Requires the user to press 'enter' to terminate the program.
        std::cout << "\nPlease press 'Enter' to exit." << std::endl;
        std::cin.ignore();

    }

    return 0;

}