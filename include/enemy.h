#pragma once
#include <string>

/**
 * @brief Represents an enemy entity in the game
 *
 * Handles combat stats including health, attack, and defense.
 * Includes damage calculation logic that factors in the enemy's defense value.
 */
class Enemy {
private:
	std::string name;
	int hitPoints;
	int maxHitPoints;
	int attackValue;
	int defenseValue;

public:
	Enemy(const std::string& enemyName, int hp, int atk, int def);

	const std::string& getName() const;
	int getHitPoints() const;
	int getMaxHitPoints() const;
	int getAttackValue() const;
	int getDefenseValue() const;

	/**
	 * @brief Applies damage to the enemy after defense calculation
	 */
	void takeDamage(int damage);

	bool isAlive() const;
};