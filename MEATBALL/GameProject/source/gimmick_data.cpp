#include "gimmick_data.h"

GIMMICK_DATA GetDoorData(int stageNo, int index)
{

	GIMMICK_DATA doorData[STAGE_MAX][DOOR_MAX + 1] = {
		// stage 1
		{
			// id, x, y
			{0, 36, 21},
			{0, 36, 22},
			{0, 36, 23},
			{0, 36, 20},
			{0, 36, 19},
			{0, 37, 21},
			{0, 37, 22},
			{0, 37, 23},
			{0, 37, 20},
			{0, 37, 19},


			{2, 130, 23},
			{2, 130, 24},
			{2, 130, 25},
			{2, 131, 23},
			{2, 131, 24},
			{2, 131, 25},


			{3, 230, 22},
			{3, 230, 23},
			{3, 230, 24},
			{3, 230, 25},
			{3, 231, 22},
			{3, 231, 23},
			{3, 231, 24},
			{3, 231, 25},
			

			{4, 259, 39},
			{4, 259, 40},
			{4, 259, 41},
			{4, 259, 42},
			{4, 260, 39},
			{4, 260, 40},
			{4, 260, 41},
			{4, 260, 42},

			{4, 261, 21},
			{4, 262, 21},
			{4, 263, 21},

			{4, 280, 21},
			{4, 281, 21},
			{4, 282, 21},

			{-1}
		},

		// stage 2
		{		
			// id, x, y
			{1, 48, 48},
			{1, 48, 49},
			{1, 48, 50},
			{1, 48, 51},
			{1, 49, 48},
			{1, 49, 49},
			{1, 49, 50},
			{1, 49, 51},


			{2, 76, 48},
			{2, 76, 49},
			{2, 76, 50},
			{2, 76, 51},
			{2, 77, 48},
			{2, 77, 49},
			{2, 77, 50},
			{2, 77, 51},


			{3, 86, 48},
			{3, 86, 49},
			{3, 86, 50},
			{3, 86, 51},
			{3, 87, 48},
			{3, 87, 49},
			{3, 87, 50},
			{3, 87, 51},


			{4, 96, 48},
			{4, 96, 49},
			{4, 96, 50},
			{4, 96, 51},
			{4, 97, 48},
			{4, 97, 49},
			{4, 97, 50},
			{4, 97, 51},


			{5, 106, 48},
			{5, 106, 49},
			{5, 106, 50},
			{5, 106, 51},
			{5, 107, 48},
			{5, 107, 49},
			{5, 107, 50},
			{5, 107, 51},


		{-1}
		
		}
	};



	return doorData[stageNo][index];
}


JUMPPAD_DATA GetJumpPadData(int stageNo, int index)
{
	JUMPPAD_DATA jumpPadData[STAGE_MAX][JUMPPAD_MAX + 1] = {
		// stage 1
		{
			//id, x, y(当たり判定矩形の左上のマップチップ座標), isAlwaysActive		
			 {0, 85, 33, 1},
			 {0, 99, 22, 1},
			 {0, 231, 34, 1},
			 {0, 257, 42, 1},
			 {0, 259, 34, 1},
			 {0, 271, 34, 1},
			 {0, 283, 34, 1},
			 {0, 326, 23, 1},
			 {0, 332, 17, 1},
			 {0, 362, 22, 1},


			//レバーあり			
			{1, 200, 25, 0},

			{-1}
		},

		// stage 2
		{
			//id, x, y(当たり判定矩形の左上のマップチップ座標), isAlwaysActive
	 		{0, 92, 33, 1},
			{0, 133, 69, 1},
			{0, 159, 62, 1},
			{0, 148, 53, 1},
			{0, 137, 44, 1},

	 		
			{-1}
		}
	};


	return jumpPadData[stageNo][index];
}


LEVER_DATA GetLeverData(int stageNo, int index)
{

	LEVER_DATA leverData[STAGE_MAX][LEVER_MAX + 1] = {
		// stage 1
		{
			// x, y,  gimmickType, gimmickId(上記のdoorData, jumpPadDataのidと対応付ける), returnInactive

			{31, 19, GIMMICK_TYPE_DOOR, 0, 0, GIMMICK_ROTATION_CEILING},
			
			{122, 24, GIMMICK_TYPE_DOOR, 2, 0},

			{223, 15, GIMMICK_TYPE_DOOR, 3, 0},

			{271, 37, GIMMICK_TYPE_DOOR, 4, 0, GIMMICK_ROTATION_CEILING},


			//ジャンプパッド用
			{209, 24, GIMMICK_TYPE_JUMPPAD, 1, 1},
			


			{-1}
		},

		// stage 2
		{
			// x, y,  gimmickType, gimmickId(上記のdoorData, jumpPadDataのidと対応付ける), returnInactive

			{56, 39, GIMMICK_TYPE_DOOR, 1, 0},

			{76, 40, GIMMICK_TYPE_DOOR, 2, 0},

			{86, 40, GIMMICK_TYPE_DOOR, 3, 0},

			{96, 40, GIMMICK_TYPE_DOOR, 4, 0},

			{106, 40, GIMMICK_TYPE_DOOR, 5, 0},
		
			

			{-1}

		}
	};

	return leverData[stageNo][index];


}

SPIKE_DATA GetSpikeData(int stageNo, int index)
{
	SPIKE_DATA spikeData[STAGE_MAX][SPIKE_MAX + 1] = {
		// stage 1
		{
			// x, y, rotation（デフォルトはGIMMICK_ROTATION_FLOOR）
			{43, 26, GIMMICK_ROTATION_CEILING},
			{44, 26, GIMMICK_ROTATION_CEILING},
			{45, 26, GIMMICK_ROTATION_CEILING},
			{45, 28,},
			{46, 28,},
			{46, 25, GIMMICK_ROTATION_LEFT_WALL},

			

			{155, 29},
			{156, 29},
			{157, 29},
			{158, 29},
			{159, 29},
			{160, 29},
			{161, 29},
			

			{165, 29},
			{166, 29},
			{167, 29},
			{168, 29},
			{169, 29},
			{170, 29},
			{171, 29},
			

			{264, 22, GIMMICK_ROTATION_CEILING},
			{265, 22, GIMMICK_ROTATION_CEILING},
			{266, 22, GIMMICK_ROTATION_CEILING},
			{267, 22, GIMMICK_ROTATION_CEILING},
			{268, 22, GIMMICK_ROTATION_CEILING},
			{269, 22, GIMMICK_ROTATION_CEILING},
			{270, 22, GIMMICK_ROTATION_CEILING},
			{271, 22, GIMMICK_ROTATION_CEILING},
			{272, 22, GIMMICK_ROTATION_CEILING},
			{273, 22, GIMMICK_ROTATION_CEILING},
			{274, 22, GIMMICK_ROTATION_CEILING},
			{275, 22, GIMMICK_ROTATION_CEILING},
			{276, 22, GIMMICK_ROTATION_CEILING},
			{277, 22, GIMMICK_ROTATION_CEILING},
			{278, 22, GIMMICK_ROTATION_CEILING},
			{279, 22, GIMMICK_ROTATION_CEILING},


			{265, 28},
			{266, 28},
			{277, 28},
			{278, 28},



			{-1}
		},

		// stage 2
		{
			// x, y, rotation（デフォルトはGIMMICK_ROTATION_FLOOR）
			{81, 34},
			{82, 34},
			{83, 34},

			{86, 34},
			{87, 34},
			{88, 34},
			{89, 34},
			{90, 34},
			{91, 34},

			{94, 34},
			{95, 34},
			{96, 34},
			{97, 34},
			{98, 34},
			{99, 34},

			{102, 34},
			{103, 34},
			{104, 34},
			{105, 34},
			{106, 34},
			{107, 34},


			{-1}

		}
	};

	return spikeData[stageNo][index];
}

GIMMICK_DATA GetBombData(int stageNo, int index)
{

	GIMMICK_DATA bombData[STAGE_MAX][BOMB_MAX + 1] = {
		// stage 1
		{
			// id, x, y
			
			{-1}
		},

		// stage 2
		{
			// id, x, y

			{0, 42, 25},
			{0, 42, 26},
			{0, 46, 26},

			{0, 57, 43},
			{0, 53, 43},
			{0, 61, 43},

			{0, 102, 44},

			{0, 92, 44},

			{0, 82, 44},

		

		

		


		

		{-1}

			
		}
	};
	return bombData[stageNo][index];
}

GIMMICK_DATA GetDashPanelData(int stageNo, int index)
{

	GIMMICK_DATA dashPanelData[STAGE_MAX][DASH_PANEL_MAX + 1] = {
		// stage 1
		{
			// id, x, y


			{-1}
		},

		// stage 2
		{
			// id, x, y
	

			{0, 52, 22},
			{0, 53, 22},
			{0, 54, 22},

			{0, 60, 22},
			{0, 61, 22},
			{0, 62, 22},

			{0, 64, 22},
			{0, 64, 23},
			{0, 64, 24},


			{0, 100, 28},
			{0, 101, 29},
			{0, 100, 30},
			

			{0, 116, 23},
			{0, 117, 23},

			{0, 102, 43},
			{0, 101, 44},
			{0, 102, 45},

			{0, 92, 43},
			{0, 91, 44},
			{0, 92, 45},

			{0, 36, 52},
			{0, 37, 52},
			{0, 38, 52},
			{0, 39, 52},

			{0, 33, 55},
			{0, 34, 55},

			{0, 41, 55},
			{0, 42, 55},

			{0, 33, 60},
			{0, 34, 60},

			{0, 41, 60},
			{0, 42, 60},

			{0, 48, 64},
			{0, 49, 64},
			{0, 50, 64},
			{0, 51, 64},

			{0, 84, 58},
			{0, 84, 59},
			
			

			{0, 59, 60},
			{0, 59, 61},
			{0, 71, 60},
			{0, 71, 61},

		{-1}

			
		}
	};
	return dashPanelData[stageNo][index];
}
