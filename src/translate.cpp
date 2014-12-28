
#include "translate.h"

#define TRANSLATE_RACE_FILE "ids/races.lua"
#define TRANSLATE_RACE_TABLE "race_ids"
#define TRANSLATE_ANIM_FILE "ids/animations.lua"
#define TRANSLATE_ANIM_TABLE "anim_ids"

namespace Translate
{
	void initialize()
	{
		//3 letter race names to race ids
		Lua::fileToTable(TRANSLATE_RACE_FILE, TRANSLATE_RACE_TABLE, false);
		//3 letter identifiers for animations in WLD files
		Lua::fileToTable(TRANSLATE_ANIM_FILE, TRANSLATE_ANIM_TABLE, false);
	}

	int raceID(std::string id)
	{
		return Lua::getInt(id.c_str(), TRANSLATE_RACE_TABLE);
	}

	int gender(std::string race3letter)
	{
		char c = race3letter.back();
		switch (c)
		{
		case 'M':
			return 0;
		case 'F':
			return 1;
		default:
			return 2;
		}
	}

	int animationID(std::string id)
	{
		return Lua::getInt(id.c_str(), TRANSLATE_ANIM_TABLE);
	}

	uint32 invertHeadingRace(int race)
	{
		switch (race)
		{
		case 1: //human
		case 2: //barbarian
		case 3: //erudite
		case 4: //wood elf
		case 5: //high elf
		case 6: //dark elf
		case 7: //half elf
		case 60: //skeleton
		case 70: //zombie
		case 127: //invisible man
			//human citizens, probably
			//other citizens
			return 1;
		default:
			return 0;
		}
	}
}
