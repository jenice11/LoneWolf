#include "utility.h"
#include <iostream>
#include <random>

void printBorderedText(const std::vector<std::string>& content) {
	char verticalBorderChar = '-';

	// Find the longest line
	size_t maxLength = 0;
	for (const auto& line : content) {
		if (line.length() > maxLength) {
			maxLength = line.length();
		}
	}

	std::string verticalBorder(maxLength + 2, verticalBorderChar);
	std::cout << verticalBorder << '\n';

	for (const auto& line : content) {
		std::cout << " " << line << '\n';
	}

	std::cout << verticalBorder << '\n';
}

int rollDice(int max) {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dist(1, max);
	return dist(gen);
}

void validateInput(size_t& choice, size_t maxSize) {
	int maxAttempts = 5;
	int attempts = 0;

	while (attempts < maxAttempts) {
		// Get and assign input and is valid
		if (std::cin >> choice && (choice <= maxSize || choice == 0)) {
			// Valid input, check for spacing
			char nextChar;
			if (std::cin.get(nextChar) && nextChar != '\n') {
				// Has spacing invalid input
				std::cout << "Invalid choice. Please try again: ";
				std::cin.clear();
				std::cin.ignore(10000, '\n');
				attempts++;
			}
			else {
				return;
			}
		}
		else {
			// Invalid input
			std::cout << "Invalid choice. Please try again: ";
			std::cin.clear();
			std::cin.ignore(10000, '\n');
			attempts++;
		}
	}
	std::cout << "\nToo many invalid attempts. Defaulting to 0.\n\n";
	choice = 0;
	return;
}