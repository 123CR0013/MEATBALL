#pragma once
#include "DxLib.h"
#include "gamemain.h"
#include "scenegamemain.h"

// 当たり判定矩形の種類

// 本体の矩形
#define COLLISION_TYPE_BODY	  1
// 索敵範囲
#define COLLISION_TYPE_SEARCH	  2
// 攻撃範囲
#define COLLISION_TYPE_ATTACK	  3


struct CHARA_ANIM
{
	int index = -1;
	int totalTime = -1;
};

// 残像情報
struct AFTERIMAGE
{
	// 使用中か
	int use = 0;

	// 残像を出現させるタイミング
	int frame = 0;
	// 残像を持続させるフレーム数
	int remainCnt = 0;

	// 残像の3Dモデル
	int modelHandle = -1;
	// 残像の座標
	VECTOR pos = VGet(0, 0, 0);

	// 残像のアニメーション
	// 残像発生時のplayTimeで固定
	CHARA_ANIM anim;

	// 残像の透明度
	float alpha = 0;
};

// テクスチャ情報
struct TEXTURE
{
	// テクスチャ番号
	int index = -1;
	// テクスチャ画像ハンドル
	int graphHandle = -1;
};

// マテリアル情報
struct MATERIAL 
{
	// マテリアル番号
	int index = -1;
	// ディフューズカラー（拡散光色）情報
	COLOR_F difColor = GetColorF(0.0f, 0.0f, 0.0f, 0.0f);
};

// キャラクターの基底クラス
class CharaBase {
public:
	CharaBase(int modelHandle, VECTOR startPos);
	virtual ~CharaBase();

	// 計算処理
	virtual void Process();
	// 描画処理
	virtual void Draw();

	// モーション情報の初期化処理
	void MotionInit(int motionMax);
	// モーションの計算処理
	virtual void MotionProcess(int motionNum_old);

	// 描画位置の補正処理
	virtual void AdjustDrawPos(MATRIX mView);


	int GetModelHandle() { return _modelHandle; };

	// 3Dモデルの回転処理
	void UpdateModelRotation();


	VECTOR GetPos() { return _pos; };

	// 3Dモデル表示用座標の更新
	virtual void UpdateWorldPos();



	VECTOR GetCenterPos() { return _centerPos; };



	void UpdateCenterPos();


	void SetDirection(VECTOR vDir) { _direction = vDir; };

	void SetCollision();
	MYRECT SetMyRect(MYRECT difRect, VECTOR pos);
	MYRECT GetMyRect(int collisionType);




	//void Attack(int i);
	void SetUse(int n) { _use = n; };
	int GetUse() { return _use; };


	// テクスチャ画像の貼り替え
	// ダメージ受けたとき, 数フレームだけ真っ白な画像を貼る
	// その後, 元に戻す
	// 引数isDamaged によって貼るテクスチャを変える
	// 0 : 通常時, 1 : 被ダメージ時
	// ※ 関数内で継承先のクラスの静的メンバ変数にアクセスするので, 継承先ごとに関数定義をする
	virtual void ChangeTexture(int isDamaged);

	// ChangeTextureの中で呼ぶ
	// _modelHandleのテクスチャ番号indexのテクスチャ画像をgraphHandleの画像に差し替える
	void SetTexture(int index, int graphHandle);

	// マテリアルのディフューズカラー（拡散光色）の変更
	// 引数isDamagedによってディフューズカラーを変更する
	// ※ 関数内で継承先のクラスの静的メンバ変数にアクセスするので, 継承先ごとに関数定義をする
	virtual void ChangeMaterialDifColor(int isDamaged);

	// ChangeTextureの中で呼ぶ
	// _modelHandleのマテリアル番号indexのディフューズカラー（拡散光色）を変更する
	void SetMaterialDifColor(int index, COLOR_F difColor);




	// 前面レイヤーに描画
	// 継承先の各クラスで定義
	virtual void DrawFrontLayer(MATRIX mView);


	// デバッグ情報の表示
	virtual void DrawDebugInfo(MATRIX mView, int fillFrag = 1);



	// 真っ白のテクスチャ画像
	// ダメージ受けたときに3Dモデルを白く点滅させる（テクスチャを白く貼り替える）
	static int _cgTexWhite;


protected:

	// このインスタンスが使用中かどうか
	int _use;

	// 攻撃中かどうか
	int _isAttack;

	// 重力処理を行うかどうか
	int _useGravity;
	// 重力値 （y軸の移動方向）
	// ジャンプ時は重力値にマイナスの数字を入れる
	float _gravity;
	// 床に立っているかどうか
	int _stand;

	// 移動速度
	float _speed;


	//
	// 座標関連
	//

	VECTOR _pos;			// 起点座標（当たり判定, 移動処理に使う） 
	VECTOR _screenPos;		// スクロール時の表示位置の補正に使う
	VECTOR _worldPos;		// 3Dモデルの表示位置

	VECTOR _centerPos;			// 中心座標
	float _difCenterY;			// 起点座標と中心座標の差分
	VECTOR _direction;			// 向いている方向



	// 体の当たり判定矩形
	// 他プレイヤーや風との当たり判定に使う
	MYRECT _difBodyRect;				// キャラ当たり判定(配置座標からの差分 x,y / 大きさ w,h)
	MYRECT _bodyRect;					// 当たり判定矩形(左上座標 x,y / 大きさ w,h) 


	//
	// 3Dモデル関連
	// 

	// モデルハンドル
	int _modelHandle;
	// 現在のモーション番号
	int _motionNum;
	// モーションの再生時間
	float _playTime;

	// アニメーション情報
	CHARA_ANIM _anim[5];
};