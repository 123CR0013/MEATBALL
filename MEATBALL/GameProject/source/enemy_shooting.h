#pragma once
#include "DxLib.h"
#include "gamemain.h"
#include "enemybase.h"



// モーション用定数
#define SHOOTING_MOTION_MAX 2

#define SHOOTING_MOTION_ENCOUNT 0
#define SHOOTING_MOTION_NORMAL 1

// 弾の最大数
#define BULLET_MAX 1

// プレイヤーに向かって弾を撃つ敵
class EnemyShooting : public EnemyBase
{
public:
	EnemyShooting(int modelHandle, int size, int index, VECTOR startPos, VECTOR direction = VGet(0, 0, 0));
	~EnemyShooting();
	// 計算処理
	void Process(VECTOR playerPos) override;
	// 描画処理
	void Draw() override;
	
	

	// 描画位置の補正処理
	void AdjustDrawPos(MATRIX mView) override;

	// 弾を撃ちだす処理
	void ShotBullet(VECTOR playerPos);
	// 弾の計算処理
	void BulletProcess();

	// 弾破裂エフェクトのセット
	void SetBulletHitEffect();

	// 弾の描画処理
	void DrawBullet(MATRIX mView);

	// 前面レイヤーに描画
	// 弾の描画
	void DrawFrontLayer(MATRIX mView) override;



	//
	// テクスチャ関連
	//
	// ※ 関数内で継承先のクラスの静的メンバ変数にアクセスするので, 継承先ごとに関数定義をする


	// 構造体「TEXTURE」の初期化
	static void TextureInit(int modelHandle);

	// テクスチャ画像の貼り替え
	// ダメージ受けたとき, 数フレームだけ真っ白な画像を貼る
	// その後, 元に戻す
	// 引数isDamaged によって貼るテクスチャを変更する
	// 0 : 通常時, 1 : 被ダメージ時
	void ChangeTexture(int isDamaged) override;

	// 構造体「MATERIAL」の初期化
	static void MaterialInit(int modelHandle);

	// マテリアルのディフューズカラーの変更
	// 引数isDamagedによってディフューズカラーを変更する
	void ChangeMaterialDifColor(int isDamaged) override;





	// デバッグ情報の表示
	void DrawDebugInfo(MATRIX mView, int fillFrag = 1) override;


protected:
	// 弾の速度
	float _bulletSpeed;
	// 弾を発射してから、次の弾を発射するまでのフレーム数
	int _bulletCoolTime;

	// 弾情報
	struct BULLET {
		int use;
		VECTOR direction;
		VECTOR pos;

		MYRECT rect;

		int animCnt;
	};
	// 弾
	BULLET bullet[BULLET_MAX];
	// 弾のスクリーン座標
	VECTOR _bulletScreenPos;


	// 目のパーツを光らせる際の明るさ
	float _eyeEmiColorFade;


	// テクスチャ情報
	static std::vector<TEXTURE>	_texture;
	// マテリアル情報
	static std::vector<MATERIAL>	_material;
};