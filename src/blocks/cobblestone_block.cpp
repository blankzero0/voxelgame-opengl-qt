#include "cobblestone_block.h"

bool CobblestoneBlock::is_visible() const
{
	return true;
}

uint8_t CobblestoneBlock::texture_index() const
{
	return 4;
}
