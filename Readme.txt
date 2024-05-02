■タイトル


「MEATBALL」


■ジャンル


横スクロール成長アクション


■開発期間


約2カ月間
2023/7/24～2023/10/21


■開発環境


DXライブラリ
言語：c++
OS：Windows10
Maya2023
Photoshop2023
Substance 3D Painter
CLIP STUDIO
Motion Builder2023
ZBrush2022
After Effects
AsePrite
Premiere Pro
Tiled
Visual Studio 2022


■開発体制


・アミューズメントメディア総合学院
  企画、リードプランナー：
   宮田 倫彦（ゲームクリエイター学科１年）
  サブプランナー：
   河内 俊樹（ゲームクリエイター学科１年）
  プログラマー：
   諸角 寛哉（ゲームクリエイター学科１年）
   高橋 大助（ゲームクリエイター学科１年）
  グラフィック：
   見山 純（ゲーム・アニメ3DCG学科）
   中川 問登（ゲーム・アニメ3DCG学科）
   亀山 雄人（ゲーム・アニメ3DCG学科）
   野治 玲花（キャラクターデザイン学科）
   伊藤 安優香（キャラクターデザイン学科）
   永井 唯翔（キャラクターデザイン学科）
  声優：
   野上 隆世（声優学科）
   菊池 来幸（声優学科）


■必要動作環境


・Windows 10
・DirectX
・DirectInput対応コントローラー


■起動方法


・MEATBALL020Eフォルダ内のMEATBALL.exeを実行
（ダウンロードリンク https://drive.google.com/file/d/1G1-RrTD3QRcX3JyezcZpsDDoYipT9rD9/view?usp=drive_link）


■終了方法


・タイトル画面の終了を選択
・設定画面からゲームの終了を選択
・Alt＋F4キーを押す


■プログラム担当箇所


諸角寛哉
・キャラクター
    ・基底クラス (charabase.cpp, enemybase.cpp)
    ・プレイヤー (player.cpp)
    ・ミートボール (meatball.cpp)
    ・敵5種類 (enemy_float.cpp, enemy_ground.cpp, enemy_shield.cpp, enemy_shooting.cpp, enemy_tracer.cpp)
    ・ボス (boss.cpp)
・プレイヤーから飛び出すハートの処理 (heart.cpp)
・ギミック
    ・基底クラス (gimmick.cpp)    
    ・爆弾  (bomb.cpp)
    ・加速パネル (dashpanel.cpp)
    ・ジャンプ台 (jumppad.cpp)
    ・レバー (lever.cpp)
    ・とげ (spike.cpp)
・敵とギミックの配置データ管理 (enemy_data.cpp, gimmick_data.cpp)
　データはプランナーが記入
・ゲームメインシーンのUI描画以外の処理（scenegamemain.cpp）
・リスポーン処理 (respawn.cpp)
・カメラ (camera.cpp)
・エフェクト (effect.cpp)
・ムービーシーン (scenemovie.cpp)
・ゲームオーバーシーンのキャラクター関連の処理 (scenegameover.cpp)

高橋大助
・heart.cpp
    DrawFrontLayer()
・各ファイルのフェードイン/フェードアウト処理
・タイトルシーン（scenegametitle.cpp）
・ゲームオーバーシーン（scenegameover.cpp）
・会話シーン（B_scenario.cpp,C_scenario.cpp,talk_end.cpp,title_talk.cpp）
・ポーズシーン（pause.cpp）
・チュートリアルシーン（explanation.cpp）
・ゲームメインシーンUI描画（scenegamemain.cpp）
・エンディングシーン（scenebadending.cpp）
・各シーン間の遷移処理
・各素材の挿入(resource.cpp)


■操作説明


・左スティック
操作キャラクターの右左への移動
選択画面の操作
・キーＡ
ジャンプ
選択の決定
シナリオを次に進める
・キーＸ
肉塊を投げる（攻撃）
・(キーＸ＋長押し)＋左スティック   
 肉塊の軌道を指定して攻撃もしくは投擲中の肉塊の方向を指定し軌道を変更
・キーＢ        
操作説明から戻る
・キーＢ＋長押し
肉塊を手持ちに戻す
・キーＲＴ
ダッシュ移動


■概要説明


 このゲームは培養器に閉じ込められていた謎の少女「少女28」が肉塊の生き物「ミートボール」と一緒に巨大研究施設から脱出することを目的としたゲームです。
 プレイヤーは「少女28」を操作して各ステージに配置されている敵を倒したり、ギミックを解除したりしながらゴールを目指していきます。
 
 「少女28」はサイコキネシスを扱うことができ「ミートボール」を飛ばして敵を倒すことができます。また、投擲中の「ミートボール」は「少女28」のサイコキネシスゲージが最大だと追加入力で軌道を変えて攻撃できます。「ミートボール」はステージの壁や画面端に当たると反射するので、その性質を活かして効率よく倒していきましょう。
 
 敵を倒すと肉塊の大きさと攻撃力が上がり敵を倒しやすくなっていきます。肉塊がある程度の大きさになると肉塊レベルが上がり今まで壊せなかったブロックが壊せるようになっていくので敵を積極的に倒しに行きましょう。
 
 「少女28」はダメージを受けるとハートが身体から飛び出します。ハートにはカウントがありハートが飛び出している間はカウントが減っていき0になると消失します。「少女28」がハートのない状態でダメージを受けるとゲームオーバーとなるのでなるべく攻撃を受けないようにしましょう。


■その他


ゲームクリエイター甲子園に出展
（作品紹介ページ https://gameparade.creators-guild.com/works/1553）

