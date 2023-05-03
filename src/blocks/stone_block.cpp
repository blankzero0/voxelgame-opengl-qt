#include "stone_block.h"

bool StoneBlock::is_visible() const
{
	return true;
}

uint8_t StoneBlock::texture_index() const
{
	return 3;
}
