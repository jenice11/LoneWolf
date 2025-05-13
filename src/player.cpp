#include <iostream>
#include <cassert>
#include "player.h"
#include "weapon.h"
#include "armor.h"
#include "potion.h"
#include "utility.h"

Player::Player(const std::string& playerName, int hp, int atk, int def)
	: name(playerName), hitPoints(hp), maxHitPoints(hp), baseAttack(atk), baseDefense(def) {
}

Player::~Player() {
	for (Weapon* weapon : weaponInventory) {
		delete weapon;
	}
	for (Armor* armor : armorInventory) {
		delete armor;
	}
	for (Potion* potion : potionInventory) {
		delete potion;
	}
}

int Player::getHitPoints() const {
	return hitPoints;
}

int Player::getMaxHitPoints() const {
	return maxHitPoints;
}

int Player::getTotalAttack() const {
	int total = baseAttack;
	if (equippedWeapon)
		total += equippedWeapon->getAttackBonus();
	return total;
}

int Player::getTotalDefense() const {
	int total = baseDefense;
	if (equippedArmor)
		total += equippedArmor->getDefenseBonus();
	return total;
}

void Player::takeDamage(int damage) {
	int actualDamage = std::max(1, damage - getTotalDefense());
	hitPoints = std::max(0, hitPoints - actualDamage);
	std::cout << name << " takes " << actualDamage << " damage! ";
	std::cout << "Player's HP: " << hitPoints << "/" << maxHitPoints << "\n";
}

void Player::heal(int amount) {
	hitPoints = std::min(maxHitPoints, hitPoints + amount);
	std::cout << name << " heals " << amount << " HP! ";
	std::cout << "Player's HP: " << hitPoints << "/" << maxHitPoints << "\n";
}

bool Player::isAlive() const {
	return hitPoints > 0;
}

void Player::addWeapon(Weapon* weapon) {
	// Check pointer is valid before using
	assert(weapon != nullptr);
	weaponInventory.push_back(weapon);
	std::cout << " * Added " << weapon->getName() << " to inventory. *\n";
}

void Player::addArmor(Armor* armor) {
	assert(armor != nullptr);
	armorInventory.push_back(armor);
	std::cout << " * Added " << armor->getName() << " to inventory. *\n";
}

void Player::addPotion(Potion* item) {
	assert(item != nullptr);
	potionInventory.push_back(item);
	std::cout << " * Added " << item->getName() << " to inventory. *\n";
}

void Player::equipWeapon(Weapon* weapon) {
	equippedWeapon = weapon;
	std::cout << " * Equipped " << weapon->getName() << " as weapon. *\n";
}

void Player::equipArmor(Armor* armor) {
	equippedArmor = armor;
	std::cout << " * Equipped " << armor->getName() << " as armor. *\n";
}

void Player::usePotion(size_t index) {
	if (index >= potionInventory.size()) {
		std::cout << " * Invalid potion selection. *\n";
		return;
	}

	Potion* potion = potionInventory[index];
	heal(potion->getHealAmount());

	// Remove the potion from inventory
	potionInventory.erase(potionInventory.begin() + index);
	delete potion;
	std::cout << " * The potion has been consumed. *\n";
}

void Player::displayStatus() const {
	std::cout << "\n * * * * *\n- - - " << name << "'s Status - - -\n";
	std::cout << name << "'s HP: " << hitPoints << "/" << maxHitPoints << "\n";

	std::cout << "Attack: " << getTotalAttack() << " (Base: " << baseAttack;
	if (equippedWeapon) {
		std::cout << " + " << equippedWeapon->getAttackBonus()
			<< " from " << equippedWeapon->getName();
	}
	std::cout << ")\n";

	std::cout << "Defense: " << getTotalDefense() << " (Base: " << baseDefense;
	if (equippedArmor) {
		std::cout << " + " << equippedArmor->getDefenseBonus()
			<< " from " << equippedArmor->getName();
	}
	std::cout << ")\n";

	// Display weapon inventory
	std::cout << "Weapon: ";
	if (weaponInventory.empty()) {
		std::cout << "None\n";
	}
	else {
		std::cout << "\n";
		for (size_t i = 0; i < weaponInventory.size(); ++i) {
			std::cout << "  " << i + 1 << ". " << weaponInventory[i]->getName();
			if (weaponInventory[i] == equippedWeapon) {
				std::cout << " (Equipped)";
			}
			std::cout << "\n";
		}
	}
	// Display armor inventory
	std::cout << "Armor: ";
	if (armorInventory.empty()) {
		std::cout << "None\n";
	}
	else {
		std::cout << "\n";
		for (size_t i = 0; i < armorInventory.size(); ++i) {
			std::cout << "  " << i + 1 << ". " << armorInventory[i]->getName();
			if (armorInventory[i] == equippedArmor) {
				std::cout << " (Equipped)";
			}
			std::cout << "\n";
		}
	}

	// Display potion inventory
	std::cout << "Potions: ";
	if (potionInventory.empty()) {
		std::cout << "None\n";
	}
	else {
		std::cout << "\n";
		for (size_t i = 0; i < potionInventory.size(); ++i) {
			std::cout << "  " << i + 1 << ". " << potionInventory[i]->getName() << "\n";
		}
	}
	std::cout << "\n";
}

void Player::manageInventory() {
	while (true) {
		std::cout << "\n * * Menu * *\n";
		std::cout << "1. Character Status\n";
		std::cout << "2. Equip a weapon\n";
		std::cout << "3. Equip armor\n";
		std::cout << "4. Use a potion\n";
		std::cout << "5. Drop an item\n";
		std::cout << "\n - - - - - - - - - - - - - - - - - - -";
		std::cout << "\nEnter your choice (0 to return): ";

		size_t input;
		validateInput(input, 5);

		// Return
		if (input == 0) break;

		switch (input) {
		case 1:
			displayStatus();
			break;
		case 2:
			manageWeapon();
			break;
		case 3:
			manageArmor();
			break;
		case 4:
			useInventoryPotion();
			break;
		case 5:
			dropItem();
			break;
		default:
			std::cout << "Invalid choice.\n";
			break;
		}
	}
}

void Player::manageWeapon() {
	if (weaponInventory.empty()) {
		std::cout << "You have no equipment to equip.\n";
		return;
	}

	std::cout << "\nSelect " << "weapon" << " to equip:\n";

	for (int i = 0; i < weaponInventory.size(); ++i) {
		std::cout << i + 1 << ". " << weaponInventory[i]->getName();

		// Show bonuses
		std::cout << " [Attack: +" << weaponInventory[i]->getAttackBonus() << "]";

		// Show if currently equipped
		if (weaponInventory[i] == equippedWeapon) {
			std::cout << " (Currently equipped)";
		}
		std::cout << "\n";
	}

	std::cout << "\nEnter your choice: (0 to cancel): ";
	size_t input;
	validateInput(input, weaponInventory.size());

	if (input == 0) {
		std::cout << "Cancelled weapon equip.\n";
		return;
	}

	// Confirmation unequip if equipped
	// weapon index starts from 0, choice starts from 1, hence minus 1
	if (weaponInventory[input - 1] == equippedWeapon) {
		std::cout << "This item is currently equipped. Would you like to unequip it? (1 = Yes, 0 = No): ";
		size_t confirm;
		validateInput(confirm, 1);

		if (confirm != 1) {
			std::cout << "Cancelled weapon equip.\n";
			return;
		}
		std::cout << equippedWeapon->getName() << " has been unequipped.\n";
		equippedWeapon = nullptr;
		return;
	}

	Weapon* selected = weaponInventory[input - 1];
	equipWeapon(selected);
}

void Player::manageArmor() {
	if (armorInventory.empty()) {
		std::cout << "You have no armor to equip.\n";
		return;
	}

	std::cout << "\nSelect " << "armor" << " to equip:\n";

	for (size_t i = 0; i < armorInventory.size(); ++i) {
		std::cout << i + 1 << ". " << armorInventory[i]->getName();

		std::cout << " [Defense: +" << armorInventory[i]->getDefenseBonus() << "]";

		if (armorInventory[i] == equippedArmor) {
			std::cout << " (Currently equipped)";
		}
		std::cout << "\n";
	}

	std::cout << "\nEnter your choice: (0 to cancel): ";
	size_t input;
	validateInput(input, armorInventory.size());

	if (input == 0) {
		std::cout << "Cancelled armor equip.\n";
		return;
	}

	if (armorInventory[input - 1] == equippedArmor) {
		std::cout << "This item is currently equipped. Would you like to unequip it? (1 = Yes, 0 = No): ";
		size_t confirm;
		validateInput(confirm, 1);

		if (confirm != 1) {
			std::cout << "Cancelled armor equip.\n";
			return;
		}
		std::cout << "Unequipped armor:" << equippedArmor->getName() << "\n";
		equippedArmor = nullptr;
		return;
	}

	Armor* selected = armorInventory[input - 1];
	equipArmor(selected);
}

void Player::useInventoryPotion() {
	if (potionInventory.empty()) {
		std::cout << "You have no potions to use.\n";
		return;
	}

	std::cout << "\nSelect a potion to use:\n";
	for (size_t i = 0; i < potionInventory.size(); ++i) {
		std::cout << i + 1 << ". " << potionInventory[i]->getName()
			<< " (+" << potionInventory[i]->getHealAmount() << " HP)\n";
	}

	std::cout << "\nEnter your choice: (0 to cancel): ";
	size_t input;
	validateInput(input, potionInventory.size());

	if (input == 0) {
		std::cout << "Cancelled potion use.\n";
		return;
	}

	usePotion(input - 1);
}

void Player::dropItem() {
	if (weaponInventory.empty() && armorInventory.empty() && potionInventory.empty()) {
		std::cout << "You have nothing to drop.\n";
		return;
	}
	std::cout << "\nSelect item to drop:\n";
	// Display all weapon
	int itemIndex = 1;
	for (const auto& weapon : weaponInventory) {
		std::cout << itemIndex << ". " << weapon->getName() << " [Weapon]";
		if (weapon == equippedWeapon) {
			std::cout << " (Equipped)";
		}
		std::cout << "\n";
		itemIndex++;
	}
	// Display all armor
	for (const auto& armor : armorInventory) {
		std::cout << itemIndex << ". " << armor->getName() << " [Armor]";
		if (armor == equippedArmor) {
			std::cout << " (Equipped)";
		}
		std::cout << "\n";
		itemIndex++;
	}
	// List all potions
	for (const auto& potion : potionInventory) {
		std::cout << itemIndex << ". " << potion->getName() << " [Recovery]";
		std::cout << "\n";
		itemIndex++;
	}

	std::cout << "\nEnter your choice: (0 to cancel): ";
	size_t input;
	size_t totalItems = weaponInventory.size() + armorInventory.size() + potionInventory.size();
	validateInput(input, totalItems);

	if (input == 0) {
		std::cout << "Cancelled dropping item.\n";
		return;
	}

	if (input <= weaponInventory.size()) {
		size_t weaponIndex = input - 1;
		Weapon* selectedItem = weaponInventory[weaponIndex];
		if (selectedItem == equippedWeapon) {
			std::cout << "This item is currently equipped. Are you sure? (1=Yes, 0=No): ";
			size_t confirm;
			validateInput(confirm, 1);

			if (confirm != 1) {
				std::cout << "Item kept.\n";
				return;
			}
			equippedWeapon = nullptr;
		}
		// Remove from inventory
		std::cout << "Dropped " << selectedItem->getName() << ".\n";
		weaponInventory.erase(weaponInventory.begin() + weaponIndex);
		delete selectedItem;
	}
	else if (input <= weaponInventory.size() + armorInventory.size()) {
		size_t armorIndex = input - weaponInventory.size() - 1;
		Armor* selectedItem = armorInventory[armorIndex];
		if (selectedItem == equippedArmor) {
			std::cout << "This item is currently equipped. Are you sure? (1=Yes, 0=No): ";
			size_t confirm;
			validateInput(confirm, 1);

			if (confirm != 1) {
				std::cout << "Item kept.\n";
				return;
			}
			equippedArmor = nullptr;
		}
		std::cout << "Dropped " << selectedItem->getName() << ".\n";
		armorInventory.erase(armorInventory.begin() + armorIndex);
		delete selectedItem;
	}
	else {
		size_t potionIndex = input - weaponInventory.size() - armorInventory.size() - 1;
		Potion* selectedPotion = potionInventory[potionIndex];
		std::cout << "Dropped " << selectedPotion->getName() << ".\n";
		potionInventory.erase(potionInventory.begin() + potionIndex);
		delete selectedPotion;
	}
}