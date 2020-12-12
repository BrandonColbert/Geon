Inputs
	Up: W
	Left: A
	Down: S
	Right: D
	Fire: Space - Shoots an arrow that damages enemies on impact

	Zoom In: +
	Zoom Out: -
	Zoom Reset: =

	Toggle Pause Menu: Escape

Requirements
	1-2) AI 1 & 2
		The Mob System handles NPC behavior by delivering behavior updates every 1/20 of a second to mobs based on their defined type. Mob types are defined in the Mob component.

		Slimes, wolves, and skeletons have the same behavior. They will chase the player when he enters the enemy's line of sight within a certain distance. Wolves can see the furthest, followed closely by skeletons, and lastly slimes who require the player to be much closer.

		If the enemy loses line of sight of the player while chasing him or the player moves out of range, the enemy will pathfind to the location they last saw the player at using A*. This allows enemies to follow the player around corners that they saw him run behind.

		This behavior can be seen most easily when zooming out with an enemy in pursuit, rounding a corner, and standing still.

		Classes
			Pathfinder (FSM & A*)
				Header: include/scenes/dungeon/pathfinder.h
				Source: src/pathfinder.cpp
			Search (A*)
				Header: include/utils/search.h
				Source: src/search.cpp
			Slime (FSM)
				Header: include/scenes/dungeon/slime.h
				Source: src/slime.cpp
			Wolf (FSM)
				Header: include/scenes/dungeon/wolf.h
				Source: src/wolf.cpp
			Skeleton (FSM)
				Header: include/scenes/dungeon/skeleton.h
				Source: src/skeleton.cpp
			MobSystem
				Header: include/systems/mob_system.h
				Source: src/mob_system.cpp
			Mob
				Header: include/components/mob.h
				Source: src/mob.cpp
	3) Level Editor
		When creating a floor for the dungeon, the configuration file 'floor.txt' will be loaded to determine how the generation occurs.

		Details on each generation parameter can be found in 'floor.txt' along with default values that can be changed.

		The format does not accept spaces. Any errors in parsing the file will be sent to standard error (stderr).

		If this file is not found, default hard-coded configuration values will be used.

		Classes
			DungeonScene
				Header: include/scenes/dungeon_scene.h
				Source: src/dungeon_scene.cpp
		Files
			Floor Generation Configuration
				Path: floor.txt
	4) Start/Finish
		To see the Main Menu, run "./geon" after compiling with "make".

		To see the Game Over menu, take damage from enemies until the hearts in the bottom left corner are fully depleted. 

		Menu options can be selected via W/S or Up/Down Arrow. Pressing Spacebar will activate the selected option.

		Classes
			MenuSystem (Primary)
				Header: include/scenes/dungeon/menu_system.h
				Source: src/menu_system.cpp
			DungeonSystem (Flow Control)
				Header: include/scenes/dungeon_scene.h
				Source: src/dungeon_scene.cpp
	5) In-Play UI
		To see the Pause Menu, press Escape while completing a floor.

		Health is displayed in the bottom left corner as hearts.

		The current floor level is shown on the top of the screen.

		The number of remaining enemies is shown on the top of the screen adjacent to the floor level.

		Classes
			Warrior (Health)
				Header: include/scenes/dungeon/warrior.h
				Source: src/warrior.cpp
			MenuSystem (Floor Level and Remaining Enemies)
				Header: include/scenes/dungeon/menu_system.h
				Source: src/menu_system.cpp
	6) Textual Items
		All menu elements are textual.

		The number of remaining enemies and floor level are textual.

		Classes
			MenuSystem
				Header: include/scenes/dungeon/menu_system.h
				Source: src/menu_system.cpp

Notes
	Generation is indeed random this time. However, the floor layout along with enemy types and positions for each level are entirely deterministic and primarily based on the 'seed' parameter specified in 'floor.txt'.

	All values I put in 'floor.txt' are the same as the hard-coded defaults.