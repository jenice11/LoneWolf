#pragma once
#include <string>

/**
 * @brief Represents the Weapon equipment
 */
class Weapon {
private:
	std::string name;
	int attackBonus;

public:
	Weapon(const std::string& itemName, int atk);

	const std::string& getName() const;
	int getAttackBonus() const;
};