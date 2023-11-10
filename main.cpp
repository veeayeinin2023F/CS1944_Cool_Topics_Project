#include <iostream>
#include <string>
#include <cstdlib>
#include <vector>
#include <algorithm>


/// How many individuals a population should be comprised of.
static const int POPULATION_SIZE = 100;

/// Valid characters for mutations.
static const std::string CHARACTERS = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz ";

/// The target value for the mutations.
static const std::string TARGET = "Computer Science 1944 Cool Topics Project";

/// The chance for each value to mutate.
static const double MUTATION_CHANCE = 0.01;


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

    return (double) matches / (double) individual.length();

}


/**
 * Attempts to mutate characters within a string. Characters are limited to those within {@link CHARACTERS}, and the
 * chance to mutate is defined in {@link MUTATION_CHANCE}.
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
        double random = (double) rand() / (double) RAND_MAX;

        // Check to see if the value should mutate.
        if (MUTATION_CHANCE >= random) {

            // Select a random character to mutate into.
            char mutation = CHARACTERS[rand() % CHARACTERS.length()];

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
int highest_scoring(std::vector<std::string> &population) {

    double highest = fitness(population[0]);
    int index = 0;

    // Index 0 is excluded as it is used as the default.
    for (int i = 1; i < POPULATION_SIZE; i++) {

        double fitness_score = fitness(population[i]);

        // Check if the fitness score of the current individual is the highest.
        if (fitness_score > highest) {
            highest = fitness_score;
            index = i;
        }

    }

    return index;

}


/**
 * The entry-point for the program. Utilizes a genetic algorithm to mutate a random string into the target string.
 *
 * @return The exit code.
 */
int main() {

    // Seed the random number generator.
    srand((unsigned) time(nullptr));

    // The starting value for individuals.
    std::string current   = "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX";

    // Initializes a population.
    std::vector<std::string> population(POPULATION_SIZE);
    std::fill(population.begin(), population.end(), current);

    std::cout << "Population Size: " << POPULATION_SIZE << std::endl;
    std::cout << "Mutation Chance: " << (MUTATION_CHANCE * 100) << "%" << std::endl;

    int generation = 0;
    for (;;) {

        // Increment to the next generation.
        generation++;

        // Attempt to mutate each individual in the population.
        std::for_each(population.begin(), population.end(), mutate);

        // Get the individual with the highest score.
        int highest_scorer = highest_scoring(population);
        std::string &value = population[highest_scorer];
        double fitness_score = fitness(value);

        // Output the individual with the peak fitness score.
        std::cout << value << "  |  " << fitness_score << std::endl;

        // If the algorithm is done, break out of the loop.
        if (fitness_score == 1) {
            break;
        }

        // Replace each individual with the peak individual.
        std::fill(population.begin(), population.end(), value);

    }

    std::cout << "Completed in " << generation << " generations." << std::endl;
    return 0;

}