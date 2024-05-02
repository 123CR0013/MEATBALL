#include "enemy_data.h"



// ïÇóVìG
// èâä˙ç¿ïW
FLOAT_DATA GetFloatData(int stageNo, int index)
{
	FLOAT_DATA floatData[STAGE_MAX][ENEMY_FLOAT_MAX + 1] = {
		// stage1
		{

			{SMALL, VGet(41, 15, 0), VGet(1, 0, 0)},
			{SMALL, VGet(45, 21, 0), VGet(-1, 0, 0)},
			{SMALL, VGet(94, 15, 0), VGet(1, 0, 0)},
			{SMALL, VGet(40, 14, 0), VGet(1, 0, 0)},
			{SMALL, VGet(46, 22, 0), VGet(-1, 0, 0)},
			{SMALL, VGet(136, 28, 0), VGet(1, 0, 0)},
			{SMALL, VGet(146, 28, 0), VGet(1, 0, 0)},


			{ -1, VGet(-1, -1, -1), VGet(-1, -1, -1) },

		},


		// stage2
		{
			{SMALL, VGet(39, 26, 0), VGet(1, 0, 0)},
			{SMALL, VGet(52, 39, 0), VGet(1, 0, 0)},
			{SMALL, VGet(57, 39, 0), VGet(1, 0, 0)},
			{SMALL, VGet(62, 39, 0), VGet(1, 0, 0)},
			{SMALL, VGet(55, 19, 0), VGet(1, 0, 0)},
			{SMALL, VGet(59, 25, 0), VGet(1, 0, 0)},

			{MEDIUM, VGet(54, 59, 0), VGet(0, 0, 0)},
			{MEDIUM, VGet(59, 59, 0), VGet(0, 0, 0)},
			{MEDIUM, VGet(64, 59, 0), VGet(0, 0, 0)},
			{MEDIUM, VGet(69, 59, 0), VGet(0, 0, 0)},
			{MEDIUM, VGet(74, 59, 0), VGet(0, 0, 0)},

			{MEDIUM, VGet(54, 62, 0), VGet(0, 0, 0)},
			{MEDIUM, VGet(59, 62, 0), VGet(0, 0, 0)},
			{MEDIUM, VGet(64, 62, 0), VGet(0, 0, 0)},
			{MEDIUM, VGet(69, 62, 0), VGet(0, 0, 0)},
			{MEDIUM, VGet(74, 62, 0), VGet(0, 0, 0)},
			


			{ -1, VGet(-1, -1, -1), VGet(-1, -1, -1) },
	
		}

	};
	return floatData[stageNo][index];
}



// ïÇóVìG
// èÑâÒÉãÅ[Ég
ENEMY_ROUTE GetFloatRoute(int stageNo, int enemyIndex, int routeIndex)
{
	ENEMY_ROUTE enemyRoute[STAGE_MAX][ENEMY_FLOAT_MAX + 1][ROUTE_MAX] = {
		// stage1
		{
			

			{
				{VGet(40, 15, 0), VGet(1, 0, 0)},
				{VGet(46, 15, 0), VGet(0, 1, 0)},
				{VGet(46, 21, 0), VGet(-1, 0, 0)},
				{VGet(40, 21, 0), VGet(0, -1, 0)},

				{-1}
			},

		    {
				{VGet(40, 15, 0), VGet(1, 0, 0)},
				{VGet(46, 15, 0), VGet(0, 1, 0)},
				{VGet(46, 21, 0), VGet(-1, 0, 0)},
				{VGet(40, 21, 0), VGet(0, -1, 0)},

				{-1}
		    },

		    {
				{VGet(93, 15, 0), VGet(1, 0, 0)},
				{VGet(98, 15, 0), VGet(0, 1, 0)},
				{VGet(93, 20, 0), VGet(0, -1, 0)},
				{VGet(98, 20, 0), VGet(-1, 0, 0)},

				{-1}
		    },

		    {
				{VGet(39, 14, 0), VGet(1, 0, 0)},
				{VGet(47, 14, 0), VGet(0, 1, 0)},
				{VGet(47, 22, 0), VGet(-1, 0, 0)},
				{VGet(39, 22, 0), VGet(0, -1, 0)},

				{-1}
		    },

		    {
				{VGet(39, 14, 0), VGet(1, 0, 0)},
				{VGet(47, 14, 0), VGet(0, 1, 0)},
				{VGet(47, 22, 0), VGet(-1, 0, 0)},
				{VGet(39, 22, 0), VGet(0, -1, 0)},

				{-1}
			},

		    {
				{VGet(135, 28, 0), VGet(1, 0, 0)},
				{VGet(141, 28, 0), VGet(-1, 0, 0)},
			

				{-1}
			},

		    {
				{VGet(145, 28, 0), VGet(1, 0, 0)},
				{VGet(151, 28, 0), VGet(-1, 0, 0)},
			

				{-1}
			},

		},



		// stage2
		{
			{
				{VGet(38, 26, 0), VGet(1, 0, 0)},
				{VGet(42, 26, 0), VGet(-1, 0, 0)},


				{-1}
			},

			{
				{VGet(51, 39, 0), VGet(1, 0, 0)},
				{VGet(63, 39, 0), VGet(-1, 0, 0)},

				{-1}
			},

			{
				{VGet(51, 39, 0), VGet(1, 0, 0)},
				{VGet(63, 39, 0), VGet(-1, 0, 0)},

				{-1}
			},
			{
				{VGet(51, 39, 0), VGet(1, 0, 0)},
				{VGet(63, 39, 0), VGet(-1, 0, 0)},

				{-1}
			},

			{
				{VGet(54, 19, 0), VGet(1, 0, 0)},
				{VGet(60, 19, 0), VGet(0, 1, 0)},
				{VGet(54, 25, 0), VGet(0, -1, 0)},
				{VGet(60, 25, 0), VGet(-1, 0, 0)},

				{-1}
			},

			{
				{VGet(54, 19, 0), VGet(1, 0, 0)},
				{VGet(60, 19, 0), VGet(0, 1, 0)},
				{VGet(54, 25, 0), VGet(0, -1, 0)},
				{VGet(60, 25, 0), VGet(-1, 0, 0)},

					{-1}
			},

		//å≈íËìGóp

			{
				{VGet(0, 0, 0), VGet(0, 0, 0)},
				{-1}
			},
			{
				{VGet(0, 0, 0), VGet(0, 0, 0)},
				{-1}
			},
			{
				{VGet(0, 0, 0), VGet(0, 0, 0)},
				{-1}
			},
			{
				{VGet(0, 0, 0), VGet(0, 0, 0)},
				{-1}
			},
			{
				{VGet(0, 0, 0), VGet(0, 0, 0)},
				{-1}
			},
			{
				{VGet(0, 0, 0), VGet(0, 0, 0)},
				{-1}
			},
			{
				{VGet(0, 0, 0), VGet(0, 0, 0)},
				{-1}
			},
			{
				{VGet(0, 0, 0), VGet(0, 0, 0)},
				{-1}
			},
			{
				{VGet(0, 0, 0), VGet(0, 0, 0)},
				{-1}
			},
			{
				{VGet(0, 0, 0), VGet(0, 0, 0)},
				{-1}
			},


		}


	};

	return enemyRoute[stageNo][enemyIndex][routeIndex];
}







// éÀåÇìG
// èâä˙ç¿ïW
ENEMY_DATA GetShootingData(int stageNo, int index)
{
	ENEMY_DATA shootingData[STAGE_MAX][ENEMY_SHOOTING_MAX + 1] = {
		// stage1
		{


			{SMALL, VGet(82, 22, 0)},

			{MEDIUM, VGet(91, 15, 0)},

			{SMALL, VGet(162, 16, 0)},

			{MEDIUM, VGet(219, 14, 0)},

			{MEDIUM, VGet(227, 14, 0)},

			{LARGE, VGet(212, 21, 0)},			

			{LARGE, VGet(323, 6, 0)},

			{MEDIUM, VGet(363, 3, 0)},

			{MEDIUM, VGet(363, 10, 0)},


			{ -1, VGet(-1, -1, -1) },
		},

		// stage2
		{


			{SMALL, VGet(52, 18, 0)},

			{SMALL, VGet(61, 18, 0)},

			{SMALL, VGet(74, 23, 0)},
			
			{MEDIUM, VGet(161, 51, 0)},
			
		    {MEDIUM, VGet(100, 61, 0)},

			{MEDIUM, VGet(108, 61, 0)},

			{LARGE, VGet(156, 30, 0)},
			

			{ -1, VGet(-1, -1, -1) },
		}
	};



	return shootingData[stageNo][index];
}




// ínè„ìG
// èâä˙ç¿ïW
ENEMY_DATA GetGroundData(int stageNo, int index)
{
	ENEMY_DATA groundData[STAGE_MAX][ENEMY_GROUND_MAX + 1] = {
		// stage1
		{

			
			{SMALL, VGet(60, 19, 0), ENGROUND_TYPE_NORMAL},

			{SMALL, VGet(104, 14, 0), ENGROUND_TYPE_NORMAL},

			{SMALL, VGet(204, 15, 0), ENGROUND_TYPE_NORMAL},

			{MEDIUM, VGet(261, 18, 0), ENGROUND_TYPE_NORMAL},

			{MEDIUM, VGet(280, 18, 0), ENGROUND_TYPE_NORMAL},

			{SMALL, VGet(240, 29, 0), ENGROUND_TYPE_NORMAL},

			{SMALL, VGet(236, 34, 0), ENGROUND_TYPE_NORMAL},

			{SMALL, VGet(244, 34, 0), ENGROUND_TYPE_NORMAL},

			{MEDIUM, VGet(351, 11, 0), ENGROUND_TYPE_NORMAL},

			{LARGE, VGet(380, 16, 0), ENGROUND_TYPE_NORMAL},


			{-1, VGet(-1, -1, -1)},
		},

		// stage2
		{
			

			{SMALL, VGet(41, 22, 0), ENGROUND_TYPE_METAL},

			{SMALL, VGet(57, 50, 0), ENGROUND_TYPE_NORMAL},

			{SMALL, VGet(57, 46, 0), ENGROUND_TYPE_METAL},

			{SMALL, VGet(57, 18, 0), ENGROUND_TYPE_METAL},

			{MEDIUM, VGet(97, 23, 0), ENGROUND_TYPE_METAL},

			{MEDIUM, VGet(101, 48, 0), ENGROUND_TYPE_NORMAL},

			{MEDIUM, VGet(91, 48, 0), ENGROUND_TYPE_NORMAL},

			{MEDIUM, VGet(81, 48, 0), ENGROUND_TYPE_NORMAL},
			
			{SMALL, VGet(33, 55, 0), ENGROUND_TYPE_METAL},

			{SMALL, VGet(33, 60, 0), ENGROUND_TYPE_METAL},

			{SMALL, VGet(42, 55, 0), ENGROUND_TYPE_METAL},

			{SMALL, VGet(42, 60, 0), ENGROUND_TYPE_METAL},

			{LARGE, VGet(113, 68, 0), ENGROUND_TYPE_NORMAL},


			{-1, VGet(-1, -1, -1)},
		}
	};



	return groundData[stageNo][index];
}




// èÇìG
// èâä˙ç¿ïW
ENEMY_DATA GetShieldData(int stageNo, int index)
{
	ENEMY_DATA shieldData[STAGE_MAX][ENEMY_SHIELD_MAX + 1] = {
		// stage1
		{
			{SMALL, VGet(122, 24, 0),ENSHIELD_TYPE_STAY},
			{SMALL, VGet(142, 24, 0),ENSHIELD_TYPE_STAY},
			{SMALL, VGet(152, 24, 0),ENSHIELD_TYPE_STAY},
			{SMALL, VGet(162, 24, 0),ENSHIELD_TYPE_STAY},
			{SMALL, VGet(349, 7, 0),ENSHIELD_TYPE_MOVE},
			{SMALL, VGet(376, 12, 0),ENSHIELD_TYPE_MOVE},
			{SMALL, VGet(376, 8, 0),ENSHIELD_TYPE_MOVE},


			{-1, VGet(-1, -1, -1), -1},
		},

		// stage2
		{
			{SMALL, VGet(104, 25, 0), ENSHIELD_TYPE_STAY},

			
			{-1, VGet(-1, -1, -1), -1},
		}
	};








	return shieldData[stageNo][index];
}














// èÇìG
// èÇèÓïÒ

// Ç±Ç±ÇÕèëÇ´ä∑Ç¶Ç»Ç≠ÇƒOK
SHIELD shieldInfo[ENEMY_SHIELD_MAX];



// í«è]ïÇóVìG
// èâä˙ç¿ïW
ENEMY_DATA GetTracerData(int stageNo, int index)
{

	ENEMY_DATA tracerData[STAGE_MAX][ENEMY_TRACER_MAX + 1] = {
		// stage1
		{
			{SMALL, VGet(219, 17, 0)},
			{SMALL, VGet(221, 17, 0)},
			{SMALL, VGet(219, 19, 0)},
			{SMALL, VGet(221, 19, 0)},

			{SMALL, VGet(226, 17, 0)},
			{SMALL, VGet(228, 17, 0)},
			{SMALL, VGet(226, 19, 0)},
			{SMALL, VGet(228, 19, 0)},

			
			{SMALL, VGet(339, 3, 0)},
			{SMALL, VGet(341, 3, 0)},
			{SMALL, VGet(339, 1, 0)},
			{SMALL, VGet(341, 1, 0)},

			{MEDIUM, VGet(360, 0, 0)},
			{MEDIUM, VGet(360, 3, 0)},
			{MEDIUM, VGet(360, 6, 0)},
			{MEDIUM, VGet(360, 9, 0)},
			{MEDIUM, VGet(360, 12, 0)},

			{ -1, VGet(-1, -1, -1)},
		},

		// stage2
		{
			{SMALL, VGet(103, 66, 0)},
			{SMALL, VGet(103, 68, 0)},
			{SMALL, VGet(105, 66, 0)},
			{SMALL, VGet(105, 68, 0)},

			{SMALL, VGet(143, 58, 0)},
			{SMALL, VGet(143, 60, 0)},
			{SMALL, VGet(145, 58, 0)},
			{SMALL, VGet(145, 60, 0)},

			{SMALL, VGet(145, 26, 0)},
			{SMALL, VGet(145, 28, 0)},
			{SMALL, VGet(147, 26, 0)},
			{SMALL, VGet(147, 28, 0)},

			{SMALL, VGet(160, 55, 0)},
			{SMALL, VGet(160, 57, 0)},
			{SMALL, VGet(162, 55, 0)},
			{SMALL, VGet(162, 57, 0)},

			{SMALL, VGet(144, 45, 0)},
			{SMALL, VGet(144, 47, 0)},
			{SMALL, VGet(146, 45, 0)},
			{SMALL, VGet(146, 47, 0)},

			
		
			{ -1, VGet(-1, -1, -1)},
		}
	};
	return tracerData[stageNo][index];
}




// äeìGÇÃÉTÉCÉYÅiè¨ÅEíÜÅEëÂÅjèÓïÒ

ENEMY_INFO GetEnemySizeInfo(int type, int size)
{
	ENEMY_INFO enemyInfo[ENEMY_TYPE_MAX][SIZE_MAX] = {

		// ïÇóVìG
		// ENEMY_TYPE_FLOAT
		{
			// hp, modelSize, difBodyRect, difSearchRect, difAttackRect
			{50, VGet(3.f, 3.f, 3.f), { -32, -32, 64, 64}, {-1, -1, -1, -1}, {-1, -1, -1, -1}}, // S
			//MÅELÉTÉCÉYÇÕégópÇµÇƒÇ¢Ç»Ç¢
			{80, VGet(6.f, 6.f, 6.f), { -32, -32, 64, 64}, {-1, -1, -1, -1}, {-1, -1, -1, -1}}, // M
			{300, VGet(9.f, 9.f, 9.f), { -32, -32, 64, 64}, {-1, -1, -1, -1}, {-1, -1, -1, -1}}, // L
		},

		// éÀåÇìG
		// ENEMY_TYPE_SHOOTING
		{
			// hp, modelSize, difBodyRect, difSearchRect, difAttackRect
			{100, VGet(6.f, 6.f, 6.f), { -64, -64, 128, 128}, { -960, -384, 1920,768}, { -32, -32, 64, 64 }}, // S
			{300, VGet(9.f, 9.f, 9.f), { -64, -64, 128, 128}, { -960, -384, 1920,768}, { -32, -32, 64, 64 }}, // M
			{1000, VGet(12.f, 12.f, 12.f), { -64, -64, 128, 128}, { -960, -384, 1920,768}, { -32, -32, 64, 64 }}, // L
		},

		// ínè„ìG
		// ENEMY_TYPE_GROUND
		{
			// hp, modelSize, difBodyRect, difSearchRect, difAttackRect
			{100, VGet(3.f, 3.f, 3.f), { -48, -191, 96, 191}, { -640, -760, 1280, 1160 }, { -384, -191, 768, 191 }}, // S
			{400, VGet(5.f, 5.f, 5.f), { -62, -319, 124, 319}, { -640, -760, 1280, 1160 }, { -384, -319, 768, 319 }}, // M
			{1500, VGet(9.f, 9.f, 9.f), { -144, -480, 144, 480}, { -640, -760, 1280, 1160 }, { -384, -480, 768, 480 }}, // L
		},

		// èÇìG
		// ENEMY_TYPE_SHIELD
		{
			// hp, modelSize, difBodyRect, difSearchRect, difAttackRect
			{10, VGet(6.f, 6.f, 6.f), { -64, -64, 128, 128 }, { -1000, -600, 2000, 1200}, {-1, -1, -1, -1}}, // S
			//MÅELÉTÉCÉYÇÕégópÇµÇƒÇ¢Ç»Ç¢
			{100, VGet(6.f, 6.f, 6.f), { -64, -64, 128, 128 }, { -1000, -600, 2000, 1200}, {-1, -1, -1, -1}}, // M
			{100, VGet(3.f, 6.f, 3.f), { -64, -64, 128, 128 }, { -1000, -600, 2000, 1200}, {-1, -1, -1, -1}}, // L
		},

		// í«è]ïÇóVìG
		// ENEMY_TYPE_TRACER
		{
			// hp, modelSize, difBodyRect, difSearchRect, difAttackRect
			{50, VGet(3.f, 3.f, 3.f), { -32, -32, 64, 64}, { -640, -540, 1280, 1080}, {-1, -1, -1, -1}}, // S
			{500, VGet(6.f, 6.f, 6.f), { -64, -64, 128, 128}, { -640, -540, 1280, 1080}, {-1, -1, -1, -1}}, // M
			//LÉTÉCÉYÇÕégópÇµÇƒÇ¢Ç»Ç¢
			{300, VGet(9.f, 9.f, 9.f), { -32, -32, 64, 64}, { -640, -540, 1280, 1080}, {-1, -1, -1, -1}}, // L
		},

		// çdâªìG
		// ENEMY_TYPE_METAL
		{
			// hp, modelSize, difBodyRect, difSearchRect, difAttackRect
			{100, VGet(3.f, 3.f, 3.f), { -48, -191, 96, 191}, { -640, -760, 1280, 1160 }, { -384, -191, 768, 191 }}, // S
			{400, VGet(5.f, 5.f, 5.f), { -62, -319, 124, 319}, { -640, -760, 1280, 1160 }, { -384, -319, 768, 319 }}, // M
			{1500, VGet(9.f, 9.f, 9.f), { -144, -480, 144, 480}, {  -640, -760, 1280, 1160 }, { -384, -480, 768, 480 }}, // L
		},
	};


	return enemyInfo[type][size];
}










