#include "heroanim.h"

std::array<AnimContainer, hspr_max> hero_animations;

AnimPosData* HeroAnim::get_pos(uint frame, int dir)
{
	if(!isDirectional())
		dir = 0;
	if(dir > 3)
		return nullptr;
	auto& vec = posdatas[dir];
	if(frame >= vec.size())
		return nullptr;
	return &vec[frame];
}

uint HeroAnim_Still::get(int& tile, int& flip, int& extend, bool& useltm, uint aclk, int dir)
{
	flip = o_flip;
	extend = o_extend;
	useltm = ltm;
	tile = o_tile;
	if(isDirectional())
		tile += dir;
	return 0;
}
uint HeroAnim_Loop::get(int& tile, int& flip, int& extend, bool& useltm, uint aclk, int dir)
{
	flip = o_flip;
	extend = o_extend;
	useltm = ltm;
	tile = o_tile;
	if(isDirectional())
		tile += (dir*aframes);
	uint aframe = aclk/aspeed;
	tile += aframe%aframes;
	return aframe;
}

uint HeroAnim_Sequence::get(int& tile, int& flip, int& extend, bool& useltm, uint aclk, int dir)
{
	aclk %= total_frames();
	uint ret = 0;
	for(auto& tup : data)
	{
		auto [t,f,e,frames] = tup;
		if(aclk >= frames)
		{
			aclk -= frames;
			++ret;
			continue;
		}
		tile = t;
		flip = f;
		extend = e;
	}
	useltm = ltm;
	if(isDirectional() && dir_offset)
		tile += (dir*dir_offset);
	return ret;
}

HeroAnim& AnimContainer::find(uint arg)
{
	if(arg >= ids.size() || ids[arg] >= anims.size())
		return anims.front();
	return anims[ids[arg]];
}

uint new_hero_tile(int& tile, int& flip, int& extend, bool& useltm, uint aclk, int dir, uint type, uint arg)
{
	HeroAnim& anim = hero_animations[type].find(arg);
	return anim.get(tile,flip,extend,useltm,aclk,dir);
}

