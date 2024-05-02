#pragma once
#include "DxLib.h"
#include "gamemain.h"
#include "mymath.h"
#include "charabase.h"

// 発射（再発射）時のスピード
#define SPEED_MAX 25
// スピードの最低値
#define SPEED_MIN 5
// 加速パネルに乗った時のスピード
#define SPEED_DASH 40

// 1フレームあたりの減速値
#define SPEED_DECREMENT_VALUE 0.1f
// 発射（加速）してから減速し始めるまでのフレーム数
#define LAUCH_COUNT_MAX 80

// 初期パワー
#define POWER_DEFAULT 50
// sizeが1増えるごとに増加するパワー
#define POWER_INCREMENT_VALUE 5

// 初期の半径サイズ
#define RADIUS_DEFAULT 16
// sizeが1増えるごとに増加する半径サイズ
#define RADIUS_INCREMENT_VALUE 4

// 初期の拡大率
#define MAGNIFICATION_DEFAULT 1.0f
// sizeが1増えるごとに増加する拡大率
#define MAGNIFICATION_INCREMENT_VALUE (4.0f / 16.0f)    // (RADIUS_INCREMENT_VALUE / RADIUS_DEFAULT)

// 再攻撃ができるまでの時間
#define MEATBALL_ATTACK_COOLTIME 2 * 30

#define MEATBALL_ROTAION_COOLTIME 3 * 60


// 呼び戻し操作をしている時間、または、ミートボールが画面外にいる時間が一定フレーム数経過するとミートボールがプレイヤーの元へ帰ってくる
#define MEATBALL_RETURN_CNT_MAX 30

// 残像の最大数
#define MEATBALL_AFTERIMAGE_MAX 10


// ミートボールにアタッチする機械パーツの最大数
#define MEATBALL_METAL_PARTS_MAX 8


class MeatBall : public CharaBase {
public:
	MeatBall(int modelHandle, VECTOR playerPos);
	~MeatBall();

	// ミートボールをプレイヤーのタンク内に配置するための初期化処理
	// 引数:
	// プレイヤーのモデルハンドル
	// タンクのフレーム番号
	void SetAttachToPlayer(int pHandle, int n);

	// 計算処理
	void Process(VECTOR playerPos);
	// 描画処理
	void Draw(MATRIX mview);
	// 描画位置の補正処理
	void AdjustDrawPos(MATRIX mView) override;

	// 前面レイヤーに描画
	// 方向線の描画
	void DrawFrontLayer(MATRIX mView);

	// 方向線の描画
	void DrawArrow(MATRIX mView);

	// 3Dモデル表示用座標の更新
	void UpdateWorldPos() override;

	// 攻撃状態を変更する
	// 引数 n
	// 0: 非攻撃状態に, 1: 攻撃状態に
	void SetAttack(VECTOR playerCenterPos = VGet(0, 0, 0), int n = 1);

	// プレイヤーの元へ戻す
	void ReturnToPlayer(VECTOR playerPos);

	// 敵・ボスと当たったときの反射処理
	void Reflect(MYRECT objRect);

	int GetAttackCollTime() { return _attackCoolTime; };


	float GetPower() { return _power; };
	void SpeedUp() { _speed = SPEED_DASH; _launchCnt = LAUCH_COUNT_MAX; };
	float GetSpeed() { return _speed; };



	VECTOR GetPos() { return _pos; };
	void	 SetPos(VECTOR tmpPos) { _pos = tmpPos; };



	VECTOR GetDirection() { return _direction; };
	void SetDirection(VECTOR dirTmp) { _direction = dirTmp; };
	float GetRadius() { return _r; };

	int GetIsAttack() { return _isAttack; };
	int GetCanReattack() { return _canReattack; };

	// マップチップとの反射用矩形の更新
	void UpdateReflectRect();

	MYRECT GetReflectRect() { return _reflectRect; };


	// 3Dモデル用座標の更新
	//void UpdateWorldPos();



	// サイズの更新処理
	void UpdateSize(int n);
	int  GetSize() { return _size; };
	void SetSize(int n) { _size = n; };

	void SetLevel(int n) { _level = n; };
	int  GetLevel() { return _level; };



	// ミートボールの回転を5つのパターンからランダムで決定する
	void SetRotType();

	// ミートボールが画面内にいるかを判定する
	// 返り値 0: 画面内にいない, 1: 画面内にいる
	int CheckIsInScreen();

	// ミートボールが画面外に出ている時間を取得する
	// 60になるとプレイヤーの元に戻る
	int GetOffScreenTime() { return _offScreenTime; };


	void SetHitStopCnt() { _hitStopCnt = 10; _beforHitStopPos = _pos; };

	int GetReturn() { return _isReturn; };

	
	void SetIsDrawFullArrow(int n) { _isDrawFullArrow = n; };


	//
	// スケーリング
	//

	// スケーリングの開始設定
	void SetScaling();
	// スケーリング処理
	void ScalingProcess(VECTOR vScale);


	//
	// 機械パーツ処理
	//

	// 機械パーツの初期化処理
	void MetalPartsInit(int metalPartsModel[]);
	// 機械パーツの計算処理
	// ミートボールの特定のリグに機械パーツを持たせる
	void MetalPartsProcess();
	// 機械パーツの描画処理
	void MetalPartsDraw();






	//
	// エフェクト
	//

	// 煙エフェクトのセット
	void SetSmokeEffect(HITCHIP hitChip);
	// チップ破壊エフェクトのセット
	void SetChipBreakEffect(HITCHIP hitChip);
	// オーラ描画の設定
	void SetIsDrawAura(int n);
	// 呼び戻しエフェクトのセット
	void SetReturnEffect();



	//
	// 残像関連の処理
	//

	// 残像の初期化処理
	void AfterImageInit(int modelHandle);
	// 残像の計算処理
	void AfterImageProcess();
	// 残像をセットする
	void SetAfterImage();
	// 残像の描画処理
	void DrawAfterImage();



	// デバッグ情報の表示
	void DrawDebugInfo(MATRIX mView, int fillFrag = 1) override;



	// 方向線の画像ハンドル
	static int _cgAim;


private:
	// 攻撃力
	float _power;
	// ミートボールを飛ばす方向
	// 左スティック操作
	VECTOR _arrow; 

	// ミートボールの半径
	float _r; 
	// 現在の移動速度
	float _speed;	

	// 発射してから再攻撃（方向転換）できるようになるまでのフレーム数
	int _attackCoolTime;
	// 再攻撃（方向転換）できるかどうか
	int _canReattack;

	// ミートボールがプレイヤーの手元にあるかどうか
	int _isReturn;	

	// 発射してから減速するまでのカウント
	int _launchCnt;

	// 画面内にイいるかどうか
	int _isInScreen;
	// 画面外に出ているカウント数
	// 一定時間画面外に出ていると、自動的にプレイヤーのもとに戻る
	int _offScreenTime;

	// マップチップとの当たり判定用
	// 反射するかの判定に使う
	// 円に内接する正方形
	MYRECT _reflectRect;		// 地形当たり判定(左上座標 x,y / 大きさ w,h)

	// 現在のミートボールサイズ
	int _size;
	// 現在のミートボールレベル
	int _level;
	

	//
	// 3Dモデル関連
	//

	// 3Dモデルの初期サイズ
	VECTOR _defaultSize;
	// 3Dモデルの回転値
	float _rad;
	// 回転のタイプ（5つのパターンからランダムに決める）
	int _rotType;
	// 回転タイプを変更してから、次の回転タイプに変更するまでのフレーム数
	int _rotCoolTime;


	// プレイヤーの3Dモデルハンドル
	// ボールをプレイヤーの培養ポッド内に配置する際に使う
	int _playerHandle;
	// ボールをアタッチするフレーム番号（プレイヤー3Dモデルのリグの番号）
	int _attachNum;






	// ヒットストップ開始前の座標
	VECTOR _beforHitStopPos;
	// ヒットストップのカウント
	int _hitStopCnt;



	// 方向線を最大の長さで描画するかどうか
	// 0のときは1/4の長さで描画する
	int _isDrawFullArrow;

	// オーラを描画するかどうか
	int _isDrawAura;
	// エフェクト画像のアニメーション用カウント
	int _effectCnt;


	// 残像の3Dモデル情報
	AFTERIMAGE _afterImage[MEATBALL_AFTERIMAGE_MAX];


	// 機械パーツ
	struct METAL
	{
		int modelHandle = -1;				// モデルハンドル
		int level = -1;						// 現在のミートボールのレベルがこの数値を超えていたら、このパーツを描画する
		int attachNum = -1;					// このパーツを配置するリグの名前
		VECTOR scale = VGet(0, 0, 0);		// 3Dモデルの拡大値
		VECTOR rotation = VGet(0, 0, 0);	// 3Dモデルの回転値
	};

	METAL _metal[MEATBALL_METAL_PARTS_MAX];




	// スケーリング
	int _isScaling;
	// スケーリングのカウント
	int _scalingCnt;


	// 押し出し方向を決定するための情報
	struct EXTRUSION
	{
		// 押し出す軸
		// 0 : x軸, 1: y軸 
		int axis;

		// 押し出す方向
		// -1 : マイナス方向, 1 : プラス方向
		int direction;

		// 当たり判定を取る座標
		VECTOR point;
	};
};



extern MeatBall* meatBall;