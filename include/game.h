#pragma once
#include <map>
#include <string>
#include "SceneID.h"
#include "Scene.h"
#include "Player.h"

/**
 * @brief Main game controller class
 *
 * Manages all game scenes, the player character, and the game loop.
 * Responsible for setting up the game world and orchestrating gameplay flow.
 */
class Game {
private:
	std::map<SceneID, Scene*> scenes;
	Scene* currentScene = nullptr;
	Player* player = nullptr;

public:
	// Constructor
	Game();

	// Destructor
	~Game();

	// Delete Copy Constructor - prevent copying
	Game(const Game&) = delete;

	// Delete Copy Assignment Operator - prevent assignment
	Game& operator=(const Game&) = delete;

	Scene* createScene(SceneID id, const std::string& description);
	Scene* getScene(SceneID id);
	void setStartScene(SceneID id);

	/**
	 * @brief Creates the player character with starting equipment
	 *
	 * Allocates Player object and provides initial gear (weapon, armor, potion)
	 *
	 * @param name - Name of the player character
	 */
	void createPlayer(const std::string& name);

	/**
	 * @brief Sets up the entire game storyline
	 *
	 * Creates all scenes, enemies, items, and establishes the connections
	 * between different scenes. Implemented in Game.cpp.
	 */
	void setStoryline();

	/**
	 * @brief Executes the main game loop
	 *
	 * Manages game flow from introduction to conclusion:
	 * 1. Displays introduction text
	 * 2. Creates player character
	 * 3. Processes scene transitions based on player input
	 * 4. Continues until game end or player death
	 */
	void run();
};