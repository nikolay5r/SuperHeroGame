#pragma once
#include "MyString.h"

namespace constants
{
	const short RESIZE_COEF = 2;
	const short INITIAL_CAPACITY = 4;

	const size_t MAX_PASSWORD_LENGTH = 16;
	const size_t MAX_USERNAME_LENGTH = 3;
	const size_t MIN_PASSWORD_LENGTH = 5;
	const size_t MIN_NICKNAME_LENGTH = 3;
	const unsigned MAX_POWER_ON_FIRST_LEVEL = 50;
	const unsigned MIN_POWER = 5;
	const unsigned MAX_LEVEL = 50;
	const unsigned POWER_TO_ADD = 5;
	const unsigned COINS_TO_START = 100;
	const unsigned COINS_TO_LOSE_WHEN_TIE = 15;
	const unsigned COINS_TO_WIN_AS_DEFENDER = 20;
	const unsigned COINS_TO_EARN_WHEN_LOGGING_IN = 100;
	const unsigned INITIAL_PRICE_OF_POWER_UP = 5;
	const unsigned INITIAL_PRICE_OF_LEVEL_UP = 7;
	const unsigned INITIAL_PRICE_OF_SUPERHERO = 30;
	const short MAX_NUMBER_OF_SUPERHEROES_PER_PLAYER = 20;

	const unsigned ENTITIES_TO_LOAD = 15;

	const MyString PLAYERS_FILE_PATH = "players.bin";
	const MyString ADMINS_FILE_PATH = "admins.bin";
	const MyString MARKET_SUPERHEROES_FILE_PATH = "marketSuperheroes.bin";
	const MyString SOLD_SUPERHEROES_FILE_PATH = "soldSuperheroes.bin";
}