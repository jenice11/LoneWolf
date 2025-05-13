#pragma once
#include <string>

/**
 * @brief Represents the Armor equipment
 */
class Armor {
private:
	std::string name;
	int defenseBonus;

public:
	Armor(const std::string& itemName, int def);

	const std::string& getName() const;
	int getDefenseBonus() const;
};