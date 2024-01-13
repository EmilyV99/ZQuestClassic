#ifndef ZC_HEROANIM_H
#define ZC_HEROANIM_H

#include "base/headers.h"
#include <tuple>

enum
{
	hspr_holdup, //6 (land1,land2,swim1,swim2,sswim1,sswim2)
	hspr_max
};
extern std::array<AnimContainer, hspr_max> hero_animations;

struct AnimPosData
{
	//Stores a relative position + dir
	//Used for a weapon position, held-up item position, etc
	zfix x, y;
	byte dir;
};

class HeroAnim
{
public:
	virtual uint get(int& tile, int& flip, int& extend, bool& useltm, uint aclk, int dir) = 0;
	bool ltm;
	bool directional;
	vector<AnimPosData> posdatas[4];
	
	AnimPosData* get_pos(uint frame, int dir);
	
	bool isDirectional() const {return directional;}
	virtual uint numFrames() const = 0;
};
//A standard stillframe animation
class HeroAnim_Still : public HeroAnim
{
public:
	virtual uint get(int& tile, int& flip, int& extend, bool& useltm, uint aclk, int dir) override;
	int o_tile, o_extend, o_flip;
	
	virtual uint numFrames() const override {return 1;}
}
//A standard looping animation (aframes,aspeed)
class HeroAnim_Loop : public HeroAnim
{
public:
	virtual uint get(int& tile, int& flip, int& extend, bool& useltm, uint aclk, int dir) override;
	int o_tile, o_extend, o_flip;
	int aframes;
	int aspeed;
}
//A sequence of tiles (each tile has frames >= 1)
class HeroAnim_Sequence : public HeroAnim
{
public:
	virtual uint get(int& tile, int& flip, int& extend, bool& useltm, uint aclk, int dir) override;
	vector<std::tuple<int32_t,byte,byte,word>> data; //[tile,flip,extend,frames]
	int32_t dir_offset;
private:
	uint total_frames() const
	{
		uint ret = 0;
		for(auto& tup : data)
			ret += std::get<3>(tup);
		return ret;
	}
}

struct AnimContainer
{
	vector<HeroAnim> anims; //variable size
	vector<uint> ids; //set size per sprite type, min 1
	HeroAnim& find(uint arg);
};

optional<AnimPosData> new_hero_tile(int& tile, int& flip, int& extend, bool& useltm, uint aclk, int dir, uint type, uint arg);

#endif

