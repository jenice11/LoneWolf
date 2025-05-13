#include "armor.h"

Armor::Armor(const std::string& itemName, int def)
	: name(itemName), defenseBonus(def) {
}

const std::string& Armor::getName() const {
	return name;
}

int Armor::getDefenseBonus() const {
	return defenseBonus;
}