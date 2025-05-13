#include <iostream>
#include "enemy.h"

Enemy::Enemy(const std::string& enemyName, int hp, int atk, int def)
	: name(enemyName),
	hitPoints(hp),
	maxHitPoints(hp),
	attackValue(atk),
	defenseValue(def) {
}

const std::string& Enemy::getName() const {
	return name;
}

int Enemy::getHitPoints() const {
	return hitPoints;
}

int Enemy::getMaxHitPoints() const {
	return maxHitPoints;
}

int Enemy::getAttackValue() const {
	return attackValue;
}

int Enemy::getDefenseValue() const {
	return defenseValue;
}

void Enemy::takeDamage(int damage) {
	int actualDamage = std::max(1, damage - defenseValue);
	hitPoints = std::max(0, hitPoints - actualDamage);
	std::cout << name << " takes " << actualDamage << " damage! ";
	std::cout << "Enemy HP: " << hitPoints << "/" << maxHitPoints << "\n";
}

bool Enemy::isAlive() const {
	return hitPoints > 0;
}