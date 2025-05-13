#pragma once
#include <string>

/**
 * @brief Represents the Potion recovery
 */
class Potion {
private:
	std::string name;
	int healAmount;

public:
	Potion(const std::string& potionName, int heal);

	const std::string& getName() const;
	int getHealAmount() const;
};