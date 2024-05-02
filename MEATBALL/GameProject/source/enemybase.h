#pragma once
#include "DxLib.h"
#include "gamemain.h"
#include "mymath.h"

#include "charabase.h"
#include "enemy_data.h"

#include <vector>


// 敵のタイプを識別するための定数
#define ENEMY_TYPE_FLOAT 0
#define ENEMY_TYPE_SHOOTING 1
#define ENEMY_TYPE_GROUND 2
#define ENEMY_TYPE_SHIELD 3
#define ENEMY_TYPE_TRACER 4
#define ENEMY_TYPE_METAL 5

// 敵キャラクターの基底クラス
class EnemyBase : public CharaBase
{
public:
	EnemyBase(int modelHandle, int index, VECTOR startPos, VECTOR direction = VGet(0, 0, 0));
	~EnemyBase();

	// 計算処理
	virtual void Process(VECTOR playerPos);
	// 描画処理
	virtual void Draw();

	// 敵の強さを初期設定する
	// Small, Medium, Large
	void SetStregth(int type, int size);


	// 当たり判定矩形を取得する
	MYRECT GetMyRect(int collisionType);

	int GetType() { return _type; };
	int GetIndex() { return _index; };


	void UpdateHp(int n) { _hp += n; };
	int GetHP() { return _hp; };



	void SetCollision();


	// 画面内にいるかを判定する
	void CheckIsInScreen();
	int GetIsInScreen() { return _isInScreen; };

	// 索敵状態（プレイヤーを見つけている状態）を設定する
	// 引数 n
	// 0: 非索敵状態にする, 1: 索敵状態にする
	void SetIsSpotPlayer(int n) { _isSpotPlayer = n; };

	virtual void SetAttack(int n) { _isAttack = n; };
	int GetAttack() { return _isAttack; };



	int GetCoolTime() { return _attackCoolTime; };


	void SetIsHit() { _isHitCnt = 3; };

	int  GetHitStop() { return _isHitStop; };
	void SetHitStop(VECTOR meatBallDir) { _isHitStop = 1; _hitStopCnt = 10; _hitStopDir = meatBallDir; };
	// ヒットストップ処理
	void HitStopProcess();



	// 3Dモデル表示用座標の更新
	void UpdateWorldPos() override;



	// デバッグ情報の表示
	void DrawDebugInfo(MATRIX mView, int fillFrag = 1) override;




	// 地上敵用（enemy_ground.cppで関数の中身を書く）
	// メタル状態かどうかを取得する
	virtual int GetIsMetal() { return 0; };
	// メタル状態から通常状態に戻す
	virtual void ChangeToNormal() {};




protected:
	// タイプ
	int _type;
	// インデックス番号（タイプごとに番号0から順番に設定する）
	int _index;

	// 体力
	int _hp;

	// 画面内にいるかどうか
	int _isInScreen;
	// 索敵状態かどうか（プレイヤーを発見しているか）
	int _isSpotPlayer;

	// 索敵範囲
	// この範囲内にプレイヤーが入ったら、索敵状態にする
	
	MYRECT _difSearchRect;	// 索敵範囲の起点位置からの差分(配置座標からの差分 x,y / 大きさ w,h)	
	MYRECT _searchRect;		// 索敵範囲矩形(左上座標 x,y / 大きさ w,h) 


	// 攻撃範囲
	// 各タイプの敵の弾などの攻撃判定に使う
	// （プレイヤーと敵の体との当たり判定は_bodyRectで行う）
	MYRECT _difAttackRect;	// 攻撃範囲の起点位置からの差分(配置座標からの差分 x,y / 大きさ w,h)
	MYRECT _attackRect;		// 攻撃範囲矩形(左上座標 x,y / 大きさ w,h) 


	// 残り攻撃時間
	int _attackRemainTime;
	// 攻撃が終わってから次の攻撃までのクールタイム
	int _attackCoolTime;


	
	// 攻撃されてからのカウント数
	// 攻撃されてから数フレームの間は、敵のテクスチャとマテリアルディフューズからを変更する
	int _isHitCnt;

	// ヒットストップ中かどうか
	int _isHitStop;
	// ヒットストップのカウント
	int _hitStopCnt;
	// ヒットストップ中に敵のモデルを揺らす方向
	VECTOR _hitStopDir;
};