#include "HelperFunctions.h"
#include "Regex_Error.h"

bool helper::isUpper(char a)
{
	return a >= 'A' && a <= 'Z';
}

bool helper::isLower(char a)
{
	return !isUpper(a);
}
