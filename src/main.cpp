#include <string>
#include <vector>
#include "game.h"
#include "utility.h"

int main() {
	std::vector<std::string> text = {
		"~ Inspired by the Lone Wolf: Flight from the Dark ~",
		"~ A simplified version of text RPG ~",
	};

	printBorderedText(text);

	Game game;
	game.setStoryline();

	game.run();

	return 0;
}