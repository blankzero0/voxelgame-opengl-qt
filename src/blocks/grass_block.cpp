#include "grass_block.h"

bool GrassBlock::is_visible() const
{
	return true;
}

uint8_t GrassBlock::texture_index() const
{
	return 0;
}

uint8_t GrassBlock::texture_index_side() const
{
	return 1;
}

uint8_t GrassBlock::texture_index_bottom() const
{
	return 2;
}
