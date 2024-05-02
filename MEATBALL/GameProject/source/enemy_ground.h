#pragma once
#include "DxLib.h"
#include "gamemain.h"
#include "mymath.h"
#include "enemybase.h"

// 非索敵の左右の移動範囲
#define ENEMY_GROUND_WALK_RANGE 512

// 非索敵時の移動速度（通常状態）
#define ENEMY_GROUND_SPEED_WALK 1
// 追跡時の移動速度（通常状態）
#define ENEMY_GROUND_SPEED_RUN	3
// 攻撃時の移動速度（通常状態）
#define ENEMY_GROUND_SPEED_ATTACK 8

// 非索敵時の移動速度（メタル状態）
#define ENEMY_METAL_SPEED_WALK 1
// 追跡時の移動速度（メタル状態）
#define ENEMY_METAL_SPEED_RUN	2
// 攻撃時の移動速度（メタル状態）
#define ENEMY_METAL_SPEED_ATTACK 5


// 攻撃状態が持続する時間
#define ENEMY_GROUND_ATTACKTIME 3 * 60
// 攻撃後に移動を開始するまでの時間
#define ENEMY_GROUND_COOLTIME 1 * 60

// モーション用定数
#define GROUND_MOTION_MAX 3
				
#define GROUND_MOTION_RUN 0
#define GROUND_MOTION_TIRED 1
#define GROUND_MOTION_WALK 2



// 地上をうろつき、プレイヤーを見つけると追いかけてくる敵
// 通常状態とメタル状態がある
class EnemyGround : public EnemyBase
{
public:
	EnemyGround(int modelHandle, int size, int index, VECTOR startPos, int type);
	~EnemyGround();

	

	// 計算処理
	void Process(VECTOR playerPos) override;
	// モーションの計算処理
	void MotionProcess(int motionNum_old) override;
	// 描画処理
	void Draw() override;
	// 描画位置の補正処理
	void AdjustDrawPos(MATRIX mView) override;


	void SetAttack(int n) override;


	// メタル状態かどうかを取得する
	int GetIsMetal() override { return _isMetal; };
	// メタル状態から通常状態に戻す
	void ChangeToNormal() override { _isChange = 1; };


	//
	// テクスチャ関連
	//

	// テクスチャ画像の貼り替え
	// ダメージ受けたとき, 数フレームだけ真っ白な画像を貼る
	// その後, 元に戻す
	// 引数isDamaged によって貼るテクスチャを変える
	// 0 : 通常時, 1 : 被ダメージ時
	void ChangeTexture(int isDamaged) override;

	// 構造体「MATERIAL」の初期化
	static void MaterialInit(int modelHandle);

	// マテリアルのディフューズカラーの変更
	// 引数isDamagedによってディフューズカラーを変更する
	void ChangeMaterialDifColor(int isDamaged) override;







	// 機械パーツの描画位置の補正処理
	void MetalPartsAdjustDrawPos(MATRIX mView);



	// デバッグ情報の表示
	//void DrawDebugInfo(MATRIX mView, int fillFrag = 1) override;

	// テクスチャ画像ハンドル
	static int _cgTexEnGround[2];

protected:

	// メタル状態かどうか
	int _isMetal;
	// メタル状態から通常状態に移行するかどうか
	int _isChange;

	// 機械パーツのモデルハンドル
	int _metalModel;


	// 非索敵時にする左右移動の起点位置
	// この座標を中心にENEMY_GROUND_WALK_RANGEの範囲を移動する
	VECTOR _walkStartPos;

	// 体に刺さっている機械のモデルが落下するときに使う
	// 落下の開始位置
	VECTOR _fallStartPos;
	// 落下の終了位置
	VECTOR _fallEndPos;
	// 落下中のカウント
	int _fallCnt;


	// SEを再生可能かどうか
	// 攻撃状態になったタイミングで1度SEを鳴らす
	int _canPlaySe;

	// マテリアル情報
	static std::vector<MATERIAL> _material;
};