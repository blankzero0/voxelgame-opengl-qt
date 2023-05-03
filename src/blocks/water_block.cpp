#include "water_block.h"

bool WaterBlock::is_visible() const
{
	return true;
}

uint8_t WaterBlock::texture_index() const
{
	return 6;
}
