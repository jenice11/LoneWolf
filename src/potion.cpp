#include "potion.h"

Potion::Potion(const std::string& potionName, int heal)
	: name(potionName), healAmount(heal) {
}

const std::string& Potion::getName() const {
	return name;
}

int Potion::getHealAmount() const {
	return healAmount;
}