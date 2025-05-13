#include "weapon.h"

Weapon::Weapon(const std::string& itemName, int atk)
	: name(itemName), attackBonus(atk) {
}

const std::string& Weapon::getName() const {
	return name;
}

int Weapon::getAttackBonus() const {
	return attackBonus;
}