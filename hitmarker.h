#pragma once
#include <vector>
class hitmarker : public Singleton<hitmarker>
{
private:
	friend class Singleton<hitmarker>;
	
public:
	struct info //TODO: change to tuple later 
	{
		info(float f , Vector v) 
		{
			hurt_time = f;
			pos = v;
		}
		Vector pos;
		float hurt_time;
	};
	std::vector<info> players;
	void on_paint()
	{
		const static int lineSize = 8;

		int index = 0;
		for each(auto player in players) 
		{
			if (player.hurt_time + 0.85f >= global->curtime)
			{
				Vector pos2;
				if (Math::WorldToScreen(player.pos, pos2))
				{
					vguisurface->DrawSetColor(255, 255, 255, 255);
					vguisurface->DrawLine(pos2.x - lineSize, pos2.y - lineSize, pos2.x - (lineSize / 4), pos2.y - (lineSize / 4));
					vguisurface->DrawLine(pos2.x - lineSize, pos2.y + lineSize, pos2.x - (lineSize / 4), pos2.y + (lineSize / 4));
					vguisurface->DrawLine(pos2.x + lineSize, pos2.y + lineSize, pos2.x + (lineSize / 4), pos2.y + (lineSize / 4));
					vguisurface->DrawLine(pos2.x + lineSize, pos2.y - lineSize, pos2.x + (lineSize / 4), pos2.y - (lineSize / 4));

				}
			}
			else
			{
				players.erase(players.begin() + index);
				return;
			}
			index++;
		}
		
	}
};