#include <iostream>
#include <vector>
#include <string>
#include "game.h"
#include "scene.h"
#include "player.h"
#include "weapon.h"
#include "armor.h"
#include "potion.h"
#include "utility.h"
#include "sceneID.h"

constexpr int PLAYER_HP = 30;
constexpr int PLAYER_ATK = 5;
constexpr int PLAYER_DEF = 2;

Game::Game() = default;

Game::~Game() {
	// For each key-value pair in the scenes map 
	for (auto& [id, scene] : scenes) {
		delete scene;
	}
	delete player;
}

Scene* Game::createScene(SceneID id, const std::string& description) {
	auto* scene = new Scene(id, description);
	scenes[id] = scene;
	return scene;
}

Scene* Game::getScene(SceneID id) {
	auto it = scenes.find(id);
	return (it != scenes.end()) ? it->second : nullptr;
}

void Game::setStartScene(SceneID id) {
	currentScene = getScene(id);
}

void Game::createPlayer(const std::string& name) {
	player = new Player(name, PLAYER_HP, PLAYER_ATK, PLAYER_DEF);

	// Give player starting equipment
	auto* woodenSword = new Weapon("Wooden Sword", 2);
	auto* leatherArmor = new Armor("Leather Armor", 1);
	auto* healingPotion = new Potion("Healing Potion", 5);

	player->addWeapon(woodenSword);
	player->addArmor(leatherArmor);
	player->addPotion(healingPotion);
	player->equipWeapon(woodenSword);
	player->equipArmor(leatherArmor);
}

void Game::setStoryline() {
	createScene(START,
		""
		"You must make haste for you sense it is not safe to linger by the smoking remains of the ruined monastery.\n"
		"At the foot of the hill, the path splits into two directions, both leading into a large wood.");

	createScene(WIDEPATH,
		"The path is wide and leads straight into thick undergrowth. The trees are tall here and unusually quiet.\n"
		"You walk for over a mile when suddenly you hear the beating of large wings directly above you.\n"
		"Looking up, you are shocked to see the sinister black outline of a Kraan diving to attack you.");

	createScene(FIGHT_KRAAN,
		"The Kraan hovers above you, raising dust with the beat of its huge black wings.\n"
		"The dust gets into your eyes and nose, and you start to cough. Now the beast attacks.");
	getScene(FIGHT_KRAAN)->setEnemy("Kraan", 20, 6, 2);
	getScene(FIGHT_KRAAN)->addNewWeapon("Dagger", 3);

	createScene(CLEARING,
		"You continue eastwards along the path. The path opens out into a large clearing.\n"
		"You notice strange claw prints in the earth. Kraan have landed here. By the number of prints and by the\n"
		"size of the area disturbed, you judge that at least five of the foul creatures landed here recently.\n\n"
		"You see two exits on the far side of the clearing. One leads west, the other south.");

	createScene(FALLENTREE,
		"You walk along this path for over an hour, carefully watching the sky above you in case the Kraan attack\n"
		"again. Up ahead, a large tree has fallen across the path. As you approach, you can hear voices coming\n"
		"from the other side of the massive trunk.");

	createScene(KAKARMI,
		"Leaping from the top of the trunk, you land in front of two small furry creatures. You recognize that\n"
		"they are Kakarmi, an intelligent race of animals that inhabit and tend the forests of Sommerlund. Before\n"
		"you can apologize for your dramatic entrance, the frightened little creatures scurry off into the forest.");

	createScene(STREAM,
		"The Kakarmi disappear into the dense undergrowth and you soon find yourself lost. After nearly two \n"
		"hours of walking you hear the sound of running water. You decide to investigate a little closer.\n"
		"Eventually you come to the edge of a fast-flowing icy stream. You follow the stream as it makes its way\n"
		"towards the east. Suddenly you notice something in the distance that brings you to a halt. You can see\n"
		"on the track above four soldiers and their officer. They wear the uniform of the King's army.");

	createScene(CONTINUE_FOREST,
		"You decide not to follow the Kakarmi and continue your journey through the forest.\n"
		"The path becomes narrower and more overgrown, making progress difficult.\n"
		"After an hour of hacking through thorny bushes, you emerge into a small glade.");

	createScene(CAMOUFLAGE,
		"You quickly gather branches and leaves to camouflage yourself and wait for the soldiers to pass.\n"
		"They march directly past your hiding spot, unaware of your presence.\n"
		"As they pass, you overhear them discussing troop movements and a planned ambush.\n"
		"After they've gone, you consider your next move.");

	createScene(APPROACH,
		"As you get nearer to the men, you call to them. As they turn to face you, your skin turns cold and your\n"
		"heart pounds, for they are Drakkarim in disguise. Suddenly they charge at you. Forced to the ground, you\n"
		"are tied up with ropes and dragged behind them along a track. They take all of your items. They cackle\n"
		"menacingly to themselves, and talk at great length of the tortures that await you at their camp.");

	createScene(MARCHING,
		"After an hour of marching, the Drakkarim suddenly halt as a large, grey scaly creature approaches along the\n"
		"track. As the beast draws closer, you can smell its fetid breath on your face. It lets out a roar and grabs\n"
		"your head in its powerful webbed hands. The last thing you hear is the sharp crack of your spine snapping.\n"
		"Game over.");

	createScene(BATTLE,
		"As you journey ahead, you can see a fierce battle raging across a stone bridge. The clash of steel and the cries\n"
		"of men and beasts echo through the forest. In the midst of the fighting, you see Prince Pelathar, the King's son.\n"
		"He is in combat with a large grey Gourgaz who is wielding a black axe above his scaly head.\n\n"
		"You picked up the Prince's sword and may use it.");
	getScene(BATTLE)->addNewWeapon("Prince's Sword", 5);

	createScene(FIGHT_GOURGAZ,
		"You rush to aid the Prince. The creature that you now face is a Gourgaz, one of a race of cold-blooded reptilian\n"
		"creatures that dwell deep in the treacherous Maakenmire swamps. Their favourite food is human flesh!");
	getScene(FIGHT_GOURGAZ)->setEnemy("Gourgaz", 30, 8, 3);

	createScene(DEFEND_PRINCE,
		"The giant Gourgaz lies dead at your feet. His evil followers hiss at you and then fall back from the bridge.\n"
		"The Prince's soldiers killed off the remaining enemies and surrounded the Prince with their shields.\n"
		"The battle is over. The prince thanked you and offers to take you into the town.");

	createScene(TOWN_END,
		"As you follow the entourage of the Prince into the town, you finally have a sense of safety inside the walls.\n"
		"But something feels amiss and you do not know what the future brings you.\n"
		"Perhaps one day you will complete your journey, but for now you retire into the inn.");

	createScene(FLEE_BATTLE,
		"You turn and flee from the battle, disappearing into the dense forest.\n"
		"The sounds of combat fade behind you as you push deeper into the woods.\n"
		"Eventually, you find a small cave in which to rest and gather your thoughts.");

	createScene(CONTINUE_BATTLE,
		"You decide to avoid the fighting and change direction, heading deeper into the forest.\n"
		"The sounds of battle fade as you make your way through the thick undergrowth.\n"
		"After several hours of walking, you find yourself on the edge of a clearing.");

	createScene(WALK_AWAY,
		"You politely decline the Prince's offer and decide to continue your journey alone.\n"
		"With a respectful bow, you turn and head back into the forest, seeking your own path.");

	createScene(UNDERGROWTH,
		"The path is wide and leads straight into thick undergrowth. The trees are tall here and unusually quiet.\n"
		"You walk for over a mile when suddenly you hear the beating of large wings directly above you.\n"
		"Looking up, you are shocked to see the sinister black outline of a Kraan diving to attack you.");

	createScene(FOGWOOD,
		"You move quickly along the track. You recall that this route leads to Fogwood, a small cluster of huts\n"
		"that have been used by a family of charcoal burners for nearly fifty years. After twenty minutes you\n"
		"reach the edge of a clearing where the huts are grouped in a small circle. There is no sign of the\n"
		"usual mist of wood smoke which gives Fogwood its  name, and the huts are unusually quiet.");

	createScene(TRACK_PERIMETER,
		"You detect Giak tracks around the perimeter of the clearing. The prints are fresh and you can tell that\n"
		"these cruel minions of the Darklords were in this area less than two hours ago.");

	createScene(INVESTIGATE_HUTS,
		"Through the open doorway of the first hut, you can see the body of a charcoal burner lying\n"
		"face down on the rough stone floor. He has been murdered, stabbed in the back by a spear. All his\n"
		"furniture and belongings have been smashed and broken and not one piece remains intact.\n"
		"This is the evil handiwork of Giaks without any doubt, for they delight in the destruction of all things.\n"
		"You search the hut and discovered a Giak Spear, proof of your suspicions. You continue along the track.\n"
		"In the distance, perched on the branch of an old oak tree is a jet-black raven.");
	getScene(INVESTIGATE_HUTS)->addNewWeapon("Giak Spear", 4);

	createScene(FIGHT_GIAK,
		"The Kraan and its riders land on the track barely ten feet from where you are hidden.A Giak leap\n"
		"from the scaly backs of the Kraan and move towards you, its spears raised to strike. You have been seen.");
	getScene(FIGHT_GIAK)->setEnemy("Giak", 10, 13, 4);

	createScene(CALL_BIRD,
		"The head of the bird slowly turns and it curses you. An instant later, it flies off above the trees and has\n"
		"soon disappeared. Shocked by what you have heard you are now sure that the fledgling was a scout of the\n"
		"Darklords and is now probably on its way to inform them of your whereabouts.");

	createScene(CONTINUE_TRACK,
		"After a few minutes walking you see a stranger, clad in red, standing in the centre of the track ahead.\n"
		"He has his back towards you, and his head is covered by the hood of his robes. Perched on his\n"
		"outstretched arm is the black raven that you saw earlier.");

	createScene(LEAVE_TRACK,
		"For half an hour or more you press on through the forest, through the rich vegetation and ferns.\n"
		"You happen upon a small clear stream where you stop for a few minutes to wash your face and drink\n"
		"of the cold, fresh water .Feeling revitalized, you cross the stream and press on. You soon notice\n"
		"the smell of wood smoke which seems to be drifting towards you from the north.");

	createScene(CONFRONT_STRANGER,
		"As your voice echoes through the trees, the stranger slowly turns to face you.\n"
		"Your heart pounds and your blood freezes as you realize that the stranger is not human. It is a Vordak,\n"
		"a hideous lieutenant of the Darklords and one of the undead. A piercing scream fills your ears, and\n"
		"the creature raises a huge black mace above its head and charges at you. Frozen with horror, you can\n"
		"also feel the Vordak attacking you with the force of its mind.");
	getScene(CONFRONT_STRANGER)->setEnemy("Vordak", 25, 7, 3);
	getScene(CONFRONT_STRANGER)->addNewArmor("Mage Armor", 4);

	createScene(KILLED_MAGE,
		"As the mage collapse, you can finally catch a breath. The raven has flown away and you are left with a corpse.\n"
		"You searched him and found a gem.");
	createScene(TAKE_MAGE_GEM,
		"As you picked up the gem, you hands burns through your bones and you felt excruciating pain.\n"
		"As you see yourself burn through the cursed gem, nothing mattered.\n"
		" Game Over! You died.");

	createScene(RUN_FROM_GIAKS,
		"You have been trudging through the forest for nearly four hours. As you escaped out of the forest,\n"
		"over a distance, you see a group of people with horse carriage.");

	createScene(APPROACH_MERCHANTS,
		"You found a merchant group heading to town. You asked to travel with them for the time being.\n"
		"You reach the town, exhausted but alive.");

	createScene(WALK_OPPOSITE,
		"As you kept walking, exhaustion overtook you and you collapse and died.\nGame over.");

	createScene(AVOID_CLEARING,
		"You decide to avoid the clearing, taking a detour through the dense forest instead.\n"
		"The journey is difficult as you push through thick undergrowth, but you eventually find a small path.\n"
		"After an hour of careful travel, you emerge from the forest near a rocky outcrop.");

	createScene(FELL_DEATH,
		"As you climb up the rocky outcrop, your feet slipped and you have fallen to your death. \nGame Over.");

	createScene(IGNORE_BIRD,
		"You ignore the raven and continue walking along the track. The bird watches you intently as you pass,\n"
		"its beady eyes following your every move. After a while, you hear the flapping of wings and notice\n"
		"the raven has taken flight, circling above you before heading off in the direction you came from.");

	createScene(INVESTIGATE_SMOKE,
		"You follow the scent of wood smoke through the trees. After about fifteen minutes, you come to a small\n"
		"clearing where an old man sits beside a campfire. He looks up as you approach, seemingly unsurprised\n"
		"by your presence. He introduces himself as a sage who has lived in these woods for many years.");

	createScene(AVOID_SMOKE,
		"Deciding not to risk investigating the source of the smoke, you change direction and head east.\n"
		"The forest grows denser here, with tall trees blotting out much of the sunlight. You push on through\n"
		"the growing darkness, hoping to find your way to safer lands.");

	getScene(START)->addChoice("Take the right path into the wood", getScene(WIDEPATH));
	getScene(START)->addChoice("Follow the left track", getScene(FOGWOOD));

	getScene(WIDEPATH)->addChoice("Draw your weapon and prepare to fight", getScene(FIGHT_KRAAN));
	getScene(WIDEPATH)->addChoice("Evade the attack by running south, deeper into the forest", getScene(UNDERGROWTH));

	getScene(FIGHT_KRAAN)->addChoice("Engage in combat", getScene(CLEARING));
	getScene(FIGHT_KRAAN)->addChoice("Flee to the east path", getScene(FOGWOOD));

	getScene(CLEARING)->addChoice("Take the south path", getScene(FALLENTREE));
	getScene(CLEARING)->addChoice("Take the west path", getScene(AVOID_CLEARING));

	getScene(FALLENTREE)->addChoice("Try to attack", getScene(KAKARMI));
	getScene(FALLENTREE)->addChoice("Listen to what the voices say", getScene(TRACK_PERIMETER));

	getScene(KAKARMI)->addChoice("Follow the Kakarmi creatures", getScene(STREAM));
	getScene(KAKARMI)->addChoice("Continue your journey without following them", getScene(CONTINUE_FOREST));

	getScene(STREAM)->addChoice("Camouflage yourself and wait for the soldiers to pass", getScene(CAMOUFLAGE));
	getScene(STREAM)->addChoice("Approach the soldiers", getScene(APPROACH));

	getScene(APPROACH)->addChoice("Attempt to escape (Success on roll of 10+)", getScene(BATTLE), 10, getScene(MARCHING));
	getScene(APPROACH)->addChoice("Wait for something to happen", getScene(MARCHING));

	getScene(BATTLE)->addChoice("Defend the Prince", getScene(FIGHT_GOURGAZ));
	getScene(BATTLE)->addChoice("Run into the forest", getScene(FLEE_BATTLE));

	getScene(FIGHT_GOURGAZ)->addChoice("Engage to battle", getScene(DEFEND_PRINCE));

	getScene(DEFEND_PRINCE)->addChoice("Follow the Prince to town", getScene(TOWN_END));
	getScene(DEFEND_PRINCE)->addChoice("Politely reject and walk away", getScene(WALK_AWAY));

	getScene(FOGWOOD)->addChoice("Track the perimeter", getScene(TRACK_PERIMETER));
	getScene(FOGWOOD)->addChoice("Prepare your weapon and stealthily approach the huts", getScene(FIGHT_GIAK));

	getScene(FIGHT_GIAK)->addChoice("Engage in battle", getScene(INVESTIGATE_HUTS));
	getScene(FIGHT_GIAK)->addChoice("Run away", getScene(AVOID_CLEARING));

	getScene(TRACK_PERIMETER)->addChoice("Forewarned by this knowledge, you decide to investigate the huts", getScene(INVESTIGATE_HUTS));
	getScene(TRACK_PERIMETER)->addChoice("Avoid the clearing", getScene(AVOID_CLEARING));

	getScene(INVESTIGATE_HUTS)->addChoice("Call the bird", getScene(CALL_BIRD));
	getScene(INVESTIGATE_HUTS)->addChoice("Ignore it", getScene(IGNORE_BIRD));

	getScene(CALL_BIRD)->addChoice("Continue your journey along the track", getScene(CONTINUE_TRACK));
	getScene(CALL_BIRD)->addChoice("Leave the track and continue through the forest instead", getScene(LEAVE_TRACK));

	getScene(CONTINUE_TRACK)->addChoice("Call the stranger", getScene(CONFRONT_STRANGER));
	getScene(CONTINUE_TRACK)->addChoice("Draw your weapon and attack", getScene(CONFRONT_STRANGER));

	getScene(CONFRONT_STRANGER)->addChoice("Engage battle", getScene(KILLED_MAGE));
	getScene(CONFRONT_STRANGER)->addChoice("Flee", getScene(RUN_FROM_GIAKS));

	getScene(KILLED_MAGE)->addChoice("Take the gem", getScene(TAKE_MAGE_GEM));
	getScene(KILLED_MAGE)->addChoice("Walked away", getScene(RUN_FROM_GIAKS));

	getScene(RUN_FROM_GIAKS)->addChoice("Approach them", getScene(APPROACH_MERCHANTS));
	getScene(RUN_FROM_GIAKS)->addChoice("Walk in the opposite direction", getScene(WALK_OPPOSITE));

	getScene(IGNORE_BIRD)->addChoice("Continue along the path", getScene(CONTINUE_TRACK));
	getScene(IGNORE_BIRD)->addChoice("Take a detour through the forest", getScene(LEAVE_TRACK));

	getScene(LEAVE_TRACK)->addChoice("Investigate the smell of wood smoke", getScene(INVESTIGATE_SMOKE));
	getScene(LEAVE_TRACK)->addChoice("Avoid the source of this smoke", getScene(AVOID_SMOKE));

	getScene(INVESTIGATE_SMOKE)->addChoice("Ask the sage for guidance", getScene(APPROACH_MERCHANTS));
	getScene(INVESTIGATE_SMOKE)->addChoice("Thank him and continue your journey", getScene(BATTLE));

	getScene(AVOID_SMOKE)->addChoice("Head towards the mountains", getScene(TOWN_END));
	getScene(AVOID_SMOKE)->addChoice("Follow a faint path through the trees", getScene(MARCHING));

	getScene(AVOID_CLEARING)->addChoice("Climb the rocky outcrop for a better view", getScene(FELL_DEATH));
	getScene(AVOID_CLEARING)->addChoice("Continue east through the forest", getScene(APPROACH_MERCHANTS));

	getScene(CONTINUE_FOREST)->addChoice("Investigate a strange sound in the bushes", getScene(FIGHT_GIAK));
	getScene(CONTINUE_FOREST)->addChoice("Keep moving forward cautiously", getScene(APPROACH_MERCHANTS));

	getScene(UNDERGROWTH)->addChoice("Hide under dense foliage", getScene(CONTINUE_BATTLE));
	getScene(UNDERGROWTH)->addChoice("Draw your weapon and prepare to fight", getScene(FIGHT_GIAK));

	getScene(CAMOUFLAGE)->addChoice("Continue your journey after they pass", getScene(APPROACH_MERCHANTS));
	getScene(CAMOUFLAGE)->addChoice("Follow the soldiers at a safe distance", getScene(MARCHING));

	getScene(FLEE_BATTLE)->addChoice("Rest and recover your strength", getScene(WALK_OPPOSITE));
	getScene(FLEE_BATTLE)->addChoice("Explore the surrounding area", getScene(APPROACH_MERCHANTS));

	getScene(CONTINUE_BATTLE)->addChoice("Follow the wind", getScene(CLEARING));
	getScene(CONTINUE_BATTLE)->addChoice("Continue on your current path", getScene(TOWN_END));

	setStartScene(START);
}

void Game::run() {
	std::string playerName;
	std::cout << "What is your name: ";
	std::cin >> playerName;

	std::vector<std::string> text = {
		"On this fateful morning, you, " + playerName + ", have been sent to collect firewood in the forest as a punishment",
		"for your inattention in class. As you are preparing to return, you see to your horror a vast cloud of black",
		"leathery creatures swoop down and engulf the monastery. Dropping the wood, you race to the battle. You grabbed",
		"your equipments but in the unnatural dark, you stumble and strike your head on a low tree branch. As you lose",
		"consciousness, the last thing that you see in the poor light are the walls of the monastery crashing to the ground."
	};

	printBorderedText(text);

	createPlayer(playerName);

	while (currentScene && player->isAlive()) {
		currentScene->display();
		currentScene = currentScene->processInput(player);
	}

	if (!player->isAlive()) {
		std::cout << "\nYour life and your mission end here.\n";
	}
}