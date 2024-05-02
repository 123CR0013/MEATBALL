#pragma once
#include "DxLib.h"
#include "gamemain.h"
#include "mymath.h"

#include "charabase.h"
#include "heart.h"


#define PLAYER_MOTION_MAX 18

#define PLAYER_AFTERIMAGE_MAX 10





class Player : public CharaBase {
public:
	Player(int modelHandle, VECTOR startPos);
	~Player();

	// 計算処理
	void Process();
	// 描画処理
	void Draw() override;
	// 描画位置の補正処理
	void AdjustDrawPos(MATRIX mView) override;

	// 3Dモデル表示用座標の更新
	void UpdateWorldPos() override;


	// キャラクターの向いている向きによってモーションを変更する
	// 
	// Process処理内でモーションを指定するときは全て右向きのモーションを指定する
	// キャラクターが左を向いている場合は、左向きのモーションに変更する
	void SetMotionReverse();
	void MotionProcess(int motionNum_old) override;


	void SetPos(VECTOR pos) { _pos = pos; };
	void SetGravity(int n) { _gravity = n; };

	void SetIsAttacked(int n) { _isAttacked = n; };

	int GetIsValidHit() { return _isValidHit; };


	void SetStand() { _stand = 1; _gravity = 0; };

	// ダッシュを途中でキャンセルする
	void StopDash();


	// ミートボールを呼び戻すまでのカウントを取得する
	// max 60になると呼び戻される
	int GetReturnCnt() { return _returnCnt; };

	// 被ダメージ処理
	void SetDamage();

	int GetGameOver() { return _isGameOver; };
	int GetGameOverMotion() { return _isPlayingGameOverMotion; };




	// アウトラインの設定
	void SetOutLine();


	// 残像処理
	void AfterImageInit(int modelHandle);
	// 残像の計算処理
	void AfterImageProcess();
	// 残像の描画処理
	void SetAfterImage();


	// SE関連

	// 再生中のSEを停止する
	// サイコキネシスSEと呼び戻しSEを停止する
	void StopSE();



	// 煙エフェクトのセット
	void SetSmokeEffect();




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





	// ハートのインスタンス
	Heart _heart;






protected:



	// 敵からの攻撃を受けたか
	int _isAttacked;
	// 敵や敵の攻撃との当たり判定が有効かどうか
	int _isValidHit;
	// 残りの無敵時間
	int _invincibleTime;
	// 被ダメージモーションを再生可能かどうか（1回のダメージで複数回モーションが再生されないようにするために使う）
	int _canPlayHitMotion;


	// ダッシュ可能かどうか
	int _canDash;
	// 残りダッシュ時間
	int _remainDashTime;
	// ダッシュのクールタイム
	int _dashCoolTime;


	// ミートボールを呼び戻すまでのカウント
	// カウントが一定以上になったら、ミートボールを呼び戻す（ワープさせる）
	int _returnCnt;

	// アニメーション情報
	CHARA_ANIM _anim[PLAYER_MOTION_MAX];






	// ゲームオーバーしたかどうか
	int _isGameOver;
	// ゲームオーバーモーション再生中かどうか
	int _isPlayingGameOverMotion;


	// オーラを描画するかどうか
	// 0: 描画しない, 1: ミートボール発射ホールド中, 2: ミートボール呼び戻し中
	int _isDrawAura;
	int _auraCnt;





	AFTERIMAGE _afterImage[PLAYER_AFTERIMAGE_MAX];

	// テクスチャ情報
	static std::vector<TEXTURE>	_texture;
	// マテリアル情報
	static std::vector<MATERIAL>	_material;

};