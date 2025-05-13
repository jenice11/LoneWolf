#include <iostream>
#include <chrono>
#include <thread>
#include "scene.h"
#include "player.h"
#include "enemy.h"
#include "weapon.h"
#include "armor.h"
#include "potion.h"
#include "utility.h"

Choice::Choice(const std::string& desc, Scene* next, int min, Scene* fail)
	: description(desc), nextScene(next), minRoll(min), failScene(fail) {
}

const std::string& Choice::getDescription() const {
	return description;
}

Scene* Choice::getNextScene() const {
	return nextScene;
}

int Choice::getMinRoll() const {
	return minRoll;
}

Scene* Choice::getFailScene() const {
	return failScene;
}

// Scene implementation
Scene::Scene(int number, const std::string& desc)
	: sceneNumber(number), description(desc) {
}

Scene::~Scene() {
	// Clean up choices
	for (auto choice : choices) {
		delete choice;
	}
	// Clean up enemy
	delete enemy;
	// Clean up unlooted equipment
	for (Weapon* weapon : weaponLoot) {
		delete weapon;
	}
	for (Armor* armor : armorLoot) {
		delete armor;
	}
	for (Potion* potion : potionLoot) {
		delete potion;
	}
}

void Scene::addChoice(const std::string& choiceDescription, Scene* nextScene, int minRoll, Scene* failScene) {
	auto* newChoice = new Choice(choiceDescription, nextScene, minRoll, failScene);
	choices.push_back(newChoice);
}

void Scene::setEnemy(const std::string& name, int hp, int atk, int def) {
	auto* newEnemy = new Enemy(name, hp, atk, def);
	auto* oldEnemy = enemy;
	enemy = newEnemy;
	delete oldEnemy;
}

void Scene::addNewWeapon(const std::string& name, int attackBonus) {
	auto* item = new Weapon(name, attackBonus);
	weaponLoot.push_back(item);
}

void Scene::addNewArmor(const std::string& name, int defenseBonus) {
	auto* item = new Armor(name, defenseBonus);
	armorLoot.push_back(item);
}

void Scene::addPotionLoot(const std::string& name, int healAmount) {
	auto* potion = new Potion(name, healAmount);
	potionLoot.push_back(potion);
}

void Scene::display() const {
	std::cout << "\n * * * * * * * * * *";
	std::cout << "\n- - - Scene " << sceneNumber << " - - -\n";
	std::cout << description;

	// Display enemy if present
	if (enemy && enemy->isAlive()) {
		std::cout << "\n\nA " << enemy->getName() << " is here! (HP: "
			<< enemy->getHitPoints() << ")";
	}

	std::cout << "\n\n * * * * * * * * * *\n";
}

void Scene::distributeLoot(Player* player) {
	if (weaponLoot.empty() && armorLoot.empty() && potionLoot.empty()) {
		return;
	}
	std::cout << "\nYou received :\n";

	// Process weapons
	while (!weaponLoot.empty()) {
		Weapon* item = weaponLoot.back();
		weaponLoot.pop_back();
		std::cout << "- " << item->getName() << " (Attack: +" << item->getAttackBonus() << ")\n";
		player->addWeapon(item);
	}

	// Process armor
	while (!armorLoot.empty()) {
		Armor* item = armorLoot.back();
		armorLoot.pop_back();
		std::cout << "- " << item->getName() << " (Defense: +" << item->getDefenseBonus() << ")\n";
		player->addArmor(item);
	}

	// Process potions
	while (!potionLoot.empty()) {
		Potion* item = potionLoot.back();
		potionLoot.pop_back();
		std::cout << "- " << item->getName() << " (Heals: +" << item->getHealAmount() << " HP)\n";
		player->addPotion(item);
	}
}

size_t Scene::getPlayerChoice(Player* player, const std::vector<Choice*>& listChoice) {
	// Display choices
	for (size_t i = 0; i < listChoice.size(); ++i) {
		std::cout << i + 1 << ". " << listChoice[i]->getDescription() << "\n";
	}
	// Add inventory access option
	std::cout << listChoice.size() + 1 << ". Open inventory\n";
	std::cout << "\nEnter your choice (1-" << listChoice.size() + 1 << "): ";

	size_t input;
	validateInput(input, listChoice.size() + 1);

	if (input == 0) {
		return getPlayerChoice(player, listChoice);
	}
	// Handle inventory option
	if (input == listChoice.size() + 1) {
		player->manageInventory();
		display();
		return getPlayerChoice(player, listChoice);
	}

	return input;
}

bool Scene::processRollCheck(const Choice* choice) const {
	int minRoll = choice->getMinRoll();

	if (minRoll <= 0) {
		return true;  // No roll check needed
	}

	std::cout << "Rolling check (D20)...\n";
	int roll = rollDice(20);
	std::this_thread::sleep_for(std::chrono::milliseconds(500));
	std::cout << "You rolled: " << roll << "\n";

	if (roll >= minRoll) {
		std::cout << "You succeeded in roll check!\n";
		return true;
	}

	std::cout << "You failed in roll check\n";
	return false;
}

Scene* Scene::handleCombatOutcome(Player* player, Enemy* currentEnemy, Scene* currentScene) {
	if (!currentEnemy || !currentEnemy->isAlive()) {
		return nullptr;  // No combat needed
	}

	if (!combat(player, currentEnemy)) {
		if (!player->isAlive()) {
			std::cout << "\nGAME OVER - You died.\n";
			return nullptr;
		}
		std::cout << "You fled, but the enemy will remain there upon your return.\n";
		return currentScene;
	}

	distributeLoot(player);
	return nullptr;  // Combat successfully completed
}

Scene* Scene::processInput(Player* player) {
	// Handle initial loot if there's no enemy or enemy is dead
	if (!enemy || !enemy->isAlive()) {
		distributeLoot(player);
	}

	// Check if game should end
	if (choices.empty()) {
		std::cout << "\nGame over!\n - The End - \n";
		return nullptr;
	}

	// Get player's choice
	std::cout << "\n";
	size_t input = getPlayerChoice(player, choices);
	const Choice* selectedChoice = choices[input - 1];

	// Handle roll check if needed
	if (!processRollCheck(selectedChoice)) {
		return selectedChoice->getFailScene();
	}

	// Handle combat if needed
	if (enemy && enemy->isAlive() && input == 1) {
		Scene* combatResult = handleCombatOutcome(player, enemy, this);
		if (combatResult != nullptr) {
			return combatResult;
		}
	}

	return selectedChoice->getNextScene();
}

/**
 * @brief Check enemy existence and whether its alive
 *
 * @return bool True if player won, false if player lost or fled
 */
bool Scene::hasEnemy() const {
	return enemy != nullptr && enemy->isAlive();
}

/**
 * @brief Manages a turn-based combat encounter between player and enemy
 *
 * @param player Pointer to the player object
 * @param enemy Pointer to the enemy object
 * @return bool True if player won, false if player lost or fled
 */
bool combat(Player* player, Enemy* enemy) {
	std::cout << "\n- - - COMBAT BEGINS - - -\n";
	std::cout << "You face a " << enemy->getName() << " (HP: " << enemy->getHitPoints() << ")\n";

	while (player->isAlive() && enemy->isAlive()) {
		std::cout << "\nYour turn:\n";
		std::cout << "1. Attack\n";
		std::cout << "2. Check status\n";
		std::cout << "3. Manage inventory\n";
		std::cout << "4. Try to flee\n";
		std::cout << "\nEnter your choice: ";

		size_t input;
		validateInput(input, 4);

		switch (input) {
		case 1: {
			std::cout << "Rolling attack dice (D20)...\n";
			int roll = rollDice(20);
			std::this_thread::sleep_for(std::chrono::milliseconds(500));
			std::cout << "You rolled: " << roll << "\n";

			if (roll >= 5) { // Hit threshold
				int damage = player->getTotalAttack() + rollDice(6);
				std::cout << "You strike the " << enemy->getName() << "!\n";
				enemy->takeDamage(damage);
			}
			else {
				std::cout << "Critical miss! You missed your attack\n";
			}
			break;
		}

		case 2:
			player->displayStatus();
			std::cout << enemy->getName() << " HP: " << enemy->getHitPoints()
				<< "/" << enemy->getMaxHitPoints() << "\n";
			continue;

		case 3:
			std::cout << "\nNote: Potions cannot be used during combat.\n";
			player->manageInventory();
			continue;

		case 4: {
			std::cout << "Rolling escape dice (D20)...\n";
			int roll = rollDice(20);
			std::this_thread::sleep_for(std::chrono::milliseconds(500));
			std::cout << "You rolled: " << roll << "\n";

			if (roll >= 12) {
				std::cout << "You successfully escape from the " << enemy->getName() << "!\n";
				return false; // Combat ends, player escaped
			}
			else {
				std::cout << "You failed to escape!\n";
			}
			break;
		}

		case 0:
			std::cout << "Cannot cancel during combat.\n";
			continue;

		default:
			std::cout << "Invalid choice. Please try again.\n";
			continue;
		}

		// Enemy's turn
		if (enemy->isAlive()) {
			std::this_thread::sleep_for(std::chrono::milliseconds(500));
			std::cout << "\nEnemy's turn:\n";
			std::cout << "The " << enemy->getName() << " attacks you!\n";
			std::cout << "Rolling enemy attack dice (D20)...\n";
			int roll = rollDice(20);
			std::this_thread::sleep_for(std::chrono::milliseconds(500));
			std::cout << "Enemy rolled: " << roll << "\n";

			if (roll >= 8) { // Hit atk threshold
				int damage = enemy->getAttackValue() + rollDice(4); // randomize attack value
				std::cout << "HIT! The " << enemy->getName() << " strikes you!\n";
				player->takeDamage(damage);
			}
			else {
				std::cout << "MISS! The " << enemy->getName() << " fails to hit you.\n";
			}
		}

		// Check if combat is over
		if (!player->isAlive()) {
			std::cout << "\nYou have been defeated by the " << enemy->getName() << ".\n";
			return false;
		}

		if (!enemy->isAlive()) {
			std::cout << "\nVictory! You defeated the " << enemy->getName() << ".\n";
			return true;
		}
	}

	return player->isAlive();
}