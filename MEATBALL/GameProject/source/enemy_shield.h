#pragma once
#include "DxLib.h"
#include "gamemain.h"
#include "enemybase.h"
#include "enemy_data.h"


// モーション用定数
#define SHIELD_MOTION_MAX 2
			
#define SHIELD_MOTION_ENCOUNT 0
#define SHIELD_MOTION_NORMAL 1



// 盾をプレイヤーの方向に構えて、ミートボールでの攻撃をガードする敵
class EnemyShield : public EnemyBase
{
public:
	EnemyShield(int modelHandle, int size, int index, VECTOR startPos, int type);
	~EnemyShield();

	// 計算処理
	void Process(VECTOR playerPos) override;

	// 盾の中心座標と角度を計算する
	void UpdateShieldCenterPos();
	// 盾の3Dモデルを回転する
	void SetShieldModelRotation();

	// 描画位置の補正処理
	void AdjustDrawPos(MATRIX mView) override;
	// 描画処理
	void Draw() override;

	// 前面レイヤーに描画
	// 盾の描画
	void DrawFrontLayer(MATRIX mView) override;


	//
	// テクスチャ関連
	//
	// ※ 静的メンバ関数はクラスの継承先ごとに定義をする
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




	// あらかじめ盾のモデルを読み込んでおく
	// 盾敵のインスタンスを生成するときにそれをコピーして使用する
	static int _shieldModelBase;

protected:

	// 行動タイプ
	// 0 : stay, 1 : move
	int _moveType;


	// 盾のモデルハンドル
	int _shieldModelHandle;
	// 盾の3Dモデルの初期の回転値
	VECTOR _defaultShieldRot;

	// 盾の中心座標
	VECTOR _shieldCenterPos;
	// 盾の角度
	VECTOR _shieldDirection;

	// 描画用座標
	// スクリーン座標（判定用の座標にカメラの補正を掛けたもの）
	VECTOR _shieldScreenPos;
	// ワールド座標（_shieldScreenPosを3Dモデル描画用の座標に変換したもの）
	VECTOR _shieldWorldPos;

	// テクスチャ情報
	static std::vector<TEXTURE>	_texture;
	// マテリアル情報
	static std::vector<MATERIAL>	_material;



};



//
// 構造体「SHIELD」に対する処理
// 

// 盾情報を初期化する（全てリセットする）
void InitShieldInfo();
// 盾情報をセットする
void SetShieldInfo(int index, float r, int h);
// 盾情報を更新する
void UpdateShieldInfo(int index, int use, VECTOR pos = VGet(0, 0, 0), VECTOR dir = VGet(0, 0, 0));
// 盾情報を取得する
SHIELD GetShieldInfo(int index);