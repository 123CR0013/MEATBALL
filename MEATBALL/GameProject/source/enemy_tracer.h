#pragma once
#include "DxLib.h"
#include "gamemain.h"
#include "mymath.h"
#include "enemybase.h"


#define TRACER_MOTION_MAX 2

#define TRACER_MOTION_ENCOUNT 0
#define TRACER_MOTION_NORMAL 1

class EnemyTracer : public EnemyBase
{
public:
	EnemyTracer(int modelHandle, int size, int index, VECTOR startPos, VECTOR direction = VGet(0, 0, 0));
	~EnemyTracer();

	// 計算処理
	void Process(VECTOR playerPos) override;
	//void Draw() override;

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
	//void DrawDebugInfo(MATRIX mView, int fillFrag = 1) override;



protected:

	// SEを再生可能かどうか
	int _canPlaySe;

	// テクスチャ情報
	static std::vector<TEXTURE>	_texture;
	// マテリアル情報
	static std::vector<MATERIAL>	_material;




};