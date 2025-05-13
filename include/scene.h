#pragma once
#include <string>
#include <vector>
#include "player.h"
#include "enemy.h"
#include "weapon.h"
#include "armor.h"
#include "potion.h"

// Forward declartion
class Choice;

/**
 * @brief Represents location in the story
 *
 * Manages scene description, choices, enemies, and loot.
 * Handles player interaction and progression to next scenes.
 */
class Scene {
private:
	int sceneNumber;
	std::string description;
	std::vector<Choice*> choices;
	Enemy* enemy = nullptr;
	std::vector<Weapon*> weaponLoot;
	std::vector<Armor*> armorLoot;
	std::vector<Potion*> potionLoot;

	/**
	 * @brief Processes roll check for a choice
	 *
	 * @param choice - Pointer to the chosen option
	 * @return bool True if check succeeds or no check needed, false if check fails
	 */
	bool processRollCheck(const Choice* choice) const;

	/**
	 * @brief Handles combat with an enemy
	 *
	 * @param player - Pointer to the player object
	 * @param enemy - Pointer to the enemy object
	 * @param currentScene - Pointer to current scene
	 * @return Scene* Next scene to continue
	 */
	Scene* handleCombatOutcome(Player* player, Enemy* enemy, Scene* currentScene);

	/**
	 * @brief Handles player choice selection
	 *
	 * @param player - Pointer to the player object
	 * @param choices - Vector of available choices
	 * @return Index of the chosen option (1-based)
	 */
	size_t getPlayerChoice(Player* player, const std::vector<Choice*>& choices);

public:
	// Constructor
	Scene(int number, const std::string& desc);

	// Destructor
	~Scene();

	// Delete Copy Constructor - prevent copying
	Scene(const Scene&) = delete;

	// Delete Copy Assignment Operator - prevent assignment
	Scene& operator=(const Scene&) = delete;

	/**
	 * @brief Adds a choice option to the scene
	 *
	 * @param choiceDescription - Text describing the choice
	 * @param nextScene - Scene to proceed to if choice is selected
	 * @param minRoll - Minimum dice roll needed (0 = no roll required)
	 * @param failScene - Scene to continue to if roll check fails
	 */
	void addChoice(const std::string& choiceDescription, Scene* nextScene, int minRoll = 0, Scene* failScene = nullptr);

	/**
	 * @brief Sets an enemy for this scene
	 *
	 * @param name - Enemy name
	 * @param hp - Enemy hit points
	 * @param atk - Enemy attack value
	 * @param def - Enemy defense value
	 */
	void setEnemy(const std::string& name, int hp, int atk, int def);

	/**
	 * @brief Adds a weapon to the scene's loot
	 *
	 * @param name - Weapon name
	 * @param attackBonus - Attack bonus value
	 */
	void addNewWeapon(const std::string& name, int attackBonus);

	/**
	 * @brief Adds armor to the scene's loot
	 *
	 * @param name - Armor name
	 * @param defenseBonus - Defense bonus value
	 */
	void addNewArmor(const std::string& name, int defenseBonus);

	/**
	 * @brief Adds a potion to the scene's loot
	 *
	 * @param name - Potion name
	 * @param healAmount - Amount of health restored by potion
	 */
	void addPotionLoot(const std::string& name, int healAmount);

	/**
	 * @brief Displays scene details to player
	 */
	void display() const;

	/**
	 * @brief Distributes loot items to the player
	 *
	 * @param player - Pointer to the player object
	 */
	void distributeLoot(Player* player);

	/**
	 * @brief Processes player input and handles scene progression
	 *
	 * @param player - Pointer to the player object
	 * @return Pointer to the next scene, or nullptr if game ends
	 */
	Scene* processInput(Player* player);

	/**
	 * @brief Check enemy existence and whether its alive
	 *
	 * @return bool True if enemy exists and is alive
	 */
	bool hasEnemy() const;
};

/**
 * @brief Represents a branching story option with optional skill check
 */
class Choice {
private:
	std::string description;
	Scene* nextScene;
	int minRoll;         // Minimum dice roll needed (0 = no roll required)
	Scene* failScene;    // Scene to go to if roll check fails

public:
	/**
	 * @brief Constructor for Choice
	 *
	 * @param desc - Description text for the choice
	 * @param next - Scene to continue
	 * @param min - Minimum roll required (default 0 = no roll required)
	 * @param fail - Scene to continue to if roll fails (default nullptr)
	 */
	Choice(const std::string& desc, Scene* next, int min = 0, Scene* fail = nullptr);

	/**
	 * @brief Gets the description of the choice
	 *
	 * @return const std::string& - The choice description
	 */
	const std::string& getDescription() const;

	/**
	 * @brief Gets the next scene if choice is successful
	 *
	 * @return Scene* - Pointer to the next scene
	 */
	Scene* getNextScene() const;

	/**
	 * @brief Gets the minimum roll required for success
	 *
	 * @return int - Minimum roll value
	 */
	int getMinRoll() const;

	/**
	 * @brief Gets the fail scene if roll check fails
	 *
	 * @return Scene* - Pointer to the fail scene
	 */
	Scene* getFailScene() const;
};

/**
 * @brief Manages a turn-based combat encounter between player and enemy
 *
 * @param player Pointer to the player object
 * @param enemy Pointer to the enemy object
 * @return bool True if player won, false if player lost or fled
 */
bool combat(Player* player, Enemy* enemy);