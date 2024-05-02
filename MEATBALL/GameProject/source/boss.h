#pragma once
#include "charabase.h"
#include "mymath.h"

#define BOSS_MOTION_MAX 16


#define BOSS_ATTACK_MAX 5

#define BOSS_BLOCK_MAX 20
#define BOSS_BULLET_MAX 30



#define BOSS_AFTERIMAGE_MAX 10


// 弾情報
struct BULLET
{
	int use = -1; // この配列要素を使用中かどうか
	int shotFrame; // 攻撃開始からこの弾を打ち始めるまでの経過フレーム数
	int speed; // 弾の速さ
	VECTOR pos = VGet(-1, -1, -1); // 弾の初期座標
	VECTOR direction = VGet(-1, -1, -1); // 弾の射出方向
};

// 攻撃情報
struct BOSS_ATTACK {
	int id = -1;

	// 次の攻撃へ移行するまでのフレーム数
	int frame = 0;
	// ボスの移動後の座標
	VECTOR pos = VGet(-1, -1, -1);
	// バリアブロックを出す座標
	VECTOR blockPos[BOSS_BLOCK_MAX] = {VGet(-1, -1, -1)};


	// 弾の挙動タイプ
	// 0:反射弾, 1: 設置弾
	int bulletType = -1;

	// 弾情報
	BULLET bullet[BOSS_BULLET_MAX];

};

class Boss : public CharaBase
{
public:
	Boss(int modelHandle, VECTOR startPos);
	~Boss();

	// 計算処理
	void Process(VECTOR playerPos);

	// 弾の初期化処理
	void BulletInit();
	// 弾の計算処理
	void BulletProcess();

	// 描画位置の補正処理
	void AdjustDrawPos(MATRIX mView) override;
	// 描画処理
	void Draw() override;
	// 弾の描画処理
	void DrawBullet(MATRIX mView);


	// 前面レイヤーに描画
	// 弾の描画
	void DrawFrontLayer(MATRIX mView) override;



	MYRECT GetBulletRect(int index);
	

	int GetBossBattle() { return _isBossBattle; };
	// ボスの攻撃を開始する
	void SetBossBattle();



	int GetHp() { return _hp; };
	void UpdateHp(int n) { _hp += n; };


	// ダメージを受けたという情報をセットする
	void SetIsHit(float damage);


	// 次の攻撃情報をセットする
	void SetAttackInfo(VECTOR playerPos);
	// 出現している弾を全て消す
	void DeleteAllBullet();
	// 攻撃パターンをランダムに決める
	void RandAttackPattern(VECTOR playerPos);
	// 攻撃情報を参照してバリアチップを出現させる
	void SetBarrier();


	// キャラクターの向いている向きによってモーションを変更する
	// 
	// Process処理内でモーションを指定するときは全て右向きのモーションを指定する
	// キャラクターが左を向いている場合は、左向きのモーションに変更する
	void SetMotionReverse();
	void MotionProcess(int motionNum_old) override;



	//
	// 残像処理
	//

	// 初期化処理
	void AfterImageInit(int modelHandle);
	// 計算処理
	void AfterImageProcess();
	// 残像をセットする
	void SetAfterImage();





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





	// ボス戦用BGMの読み込み
	static void LoadBossBGM();
	// ボス戦用BGMの破棄
	static void DeleteBossBGM();

	// ボス戦用BGMの再生・停止
	static void PlayBossBGM(int n);



	// デバッグ情報の表示
	void DrawDebugInfo(MATRIX mView, int fillFrag = 1) override;

private:
	// ボス戦が開始しているかどうか
	int _isBossBattle;

	// 体力
	int _hp;

	// 無敵時間フレーム数
	int _invincibleCnt;

	// アニメーション情報
	CHARA_ANIM _anim[BOSS_MOTION_MAX];

	// 次の攻撃に移行するかどうか
	int _changeAttack;

	// 現在の攻撃情報
	BOSS_ATTACK _attackInfo;
	// 現在の攻撃が開始されてからの経過フレーム数
	int _attackCnt;

	// 撃破されたかどうか
	int _isEliminated;

	// 攻撃中かどうか
	int _isAttack;
	// 攻撃が終了してから次の攻撃に移るまでのフレーム数
	int _attackCoolTime;

	// 次の攻撃地点まで移動する際の移動前の座標
	// 次の地点までの移動をイージングを用いて行うため、移動前の座標を保持する必要がある
	VECTOR _startPos;

	// 攻撃を受けたかどうか
	int _isHit;

	// オーラ描画用のアニメーションカウント
	int _auraAnimCnt;




	// 残像情報
	AFTERIMAGE _afterImage[BOSS_AFTERIMAGE_MAX];



	// テクスチャ情報
	static std::vector<TEXTURE>	_texture;
	// マテリアル情報
	static std::vector<MATERIAL> _material;


	// ボス戦用BGM
	static int	_bgmBossBattle;
};


extern Boss* boss;





