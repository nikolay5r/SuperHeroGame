#pragma once
#include <cstring>

namespace constants
{
	const short RESIZE_COEF = 2;
	const short INITIAL_CAPACITY = 4;

	const size_t MAX_PASSWORD_LENGTH = 16;
	const size_t MAX_USERNAME_LENGTH = 16;
	const unsigned MAX_POWER_ON_FIRST_LEVEL = 50;
	const unsigned PRINT_COUNT = 10;
	const unsigned MIN_INITIAL_POWER = 5;
	const unsigned MAX_INITIAL_POWER = 50;
	const unsigned MAX_LEVEL = 50;
	const unsigned POWER_TO_ADD = 5;
	const unsigned COINS_TO_START = 50;
	const unsigned COINS_TO_LOSE_WHEN_TIE = 15;
	const unsigned COINS_TO_WIN_AS_DEFENDER = 20;
	const unsigned COINS_TO_EARN_PERIODICALLY = 15;
	const unsigned INITIAL_PRICE_OF_POWER_UP = 5;
	const unsigned INITIAL_PRICE_OF_LEVEL_UP = 7;
	const unsigned INITIAL_PRICE_OF_SUPERHERO = 30;
	const short MAX_NUMBER_OF_SUPERHEROES_PER_PLAYER = 20;

	const unsigned ENTITIES_TO_LOAD = 15;

	const std::string PLAYERS_FILE_PATH = "players.bin";
	const std::string ADMINS_FILE_PATH = "admins.bin";
	const std::string SUPERHEROES_FILE_PATH = "superheroes.bin";
	const std::string DEAD_SUPERHEROES_FILE_PATH = "deadSuperheroes.bin";
	const std::string CONFIGS_FILE_PATH = "configs.bin";
}