#pragma once
#include <string>
#include <vector>

/**
 * @brief Displays text content with horizontal borders
 * @param content - Vector of strings to display
 */
void printBorderedText(const std::vector<std::string>& content);

/**
 * @brief Generates a random integer between 1 and the specified number of nax
 * @param max - The maximum number (inclusive) that can be rolled
 * @return A random integer in the range [1, sides]
 */
int rollDice(int max);

/**
 * @brief Validates user input to ensure it's within acceptable range
 * @param choice Reference to the user's input choice
 * @param maxSize The maximum acceptable value (0 is always valid as cancel)
 */
void validateInput(size_t& choice, size_t maxSize);
