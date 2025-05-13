#pragma once
#include <string>
#include <vector>
#include "weapon.h"
#include "armor.h"
#include "potion.h"

/**
 * @brief Represents the player character with inventory and status management
 *
 * Handles player stats, equipment, and interactions with the game world.
 * Manages inventory operations (equip/unequip/use/drop items).
 */
class Player {
private:
	std::string name;
	int hitPoints;
	int maxHitPoints;
	int baseAttack;
	int baseDefense;
	std::vector<Weapon*> weaponInventory;
	std::vector<Armor*> armorInventory;
	std::vector<Potion*> potionInventory;
	Weapon* equippedWeapon = nullptr;
	Armor* equippedArmor = nullptr;

	/**
	 * @brief Handles equipping or unequipping weapons from inventory
	 */
	void manageWeapon();

	/**
	 * @brief Handles equipping or unequipping armor from inventory
	 */
	void manageArmor();

	/**
	 * @brief Helper method to use a potion from inventory
	 */
	void useInventoryPotion();

	/**
	 * @brief Handles dropping items from any inventory category
	 *
	 * Presents a unified interface for removing weapons, armor, and potions.
	 * Prompts for confirmation when dropping equipped items.
	 */
	void dropItem();

public:
	// Constructor
	Player(const std::string& playerName, int hp, int atk, int def);

	// Destructor
	~Player();

	// Delete Copy Constructor - prevent copying
	Player(const Player&) = delete;

	// Delete Copy Assignment Operator - prevent assignment
	Player& operator=(const Player&) = delete;

	int getHitPoints() const;
	int getMaxHitPoints() const;
	int getTotalAttack() const;
	int getTotalDefense() const;

	/**
	 * @brief Applies damage to the player after defense calculations
	 * @param damage - damage amount before defense reduction
	 */
	void takeDamage(int damage);

	void heal(int amount);
	bool isAlive() const;

	void addWeapon(Weapon* weapon);
	void addArmor(Armor* armor);
	void addPotion(Potion* item);

	void equipWeapon(Weapon* weapon);
	void equipArmor(Armor* armor);

	/**
	 * @brief Consumes a potion from inventory and applies its healing effect
	 * @param index - Index of the potion in the potionInventory
	 */
	void usePotion(size_t index);

	/**
	 * @brief Displays detailed player information including equipment and stats
	 */
	void displayStatus() const;

	/**
	 * @brief Provides interactive menu for managing player inventory
	 *
	 * Allows equipping items, using potions, viewing status, and dropping items.
	 */
	void manageInventory();
};