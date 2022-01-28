#ifndef GLOBALS_H
#define GLOBALS_H

enum class gamemodes : unsigned char
{
	none,
	washing,
	grooverailroad
};

namespace globals
{
	extern gamemodes currentGamemode;
}
#endif