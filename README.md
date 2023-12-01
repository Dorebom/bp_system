このプログラムはLGPLです．

# build and run

```
sudo apt-get update
sudo apt-get install cmake
mkdir build
cd build
cmake ..
make
./bp_system
```

# ノードの説明
フォルダ bp_system/_comon にある node.hpp および node.cpp にてthreading処理およびState Machine処理を記述している．これらの処理は継承するb_hub，b_node，p_hub，p_nodeで共通のふるまいとなる．
b_hub，b_nodeでは，共有メモリ処理を記述している．bp_systemを利用される人は，b_nodeを継承したクラスを書くことになる．

## State Machine
ステートマシンの状態は，表立って INITIALIZING, READY, STABLE, REPAIR, FORCE_STOPの5種類が存在する．これとは別にUNCONFIGURED, TRANSITINGの2つが存在する．UNCONFIGUREDは初期化すらされてない生まれたてであり，TRANSITINGは表立った5種類の状態が遷移中(図で示すと矢印の上)の状態である．

![StateMachineDiagram](https://github.com/Dorebom/bp_system/assets/13288579/6d4b8be5-bb13-4968-b0f4-9c36fa5edca8)

5種類の状態はそれぞれ，以下のの定周期処理をもつ．
```
/* node_state loop process */
virtual void initialize_processing() = 0;
virtual void ready_processing() = 0;
virtual void repair_processing() = 0;
virtual void stable_processing() = 0;
virtual void force_stop_processing() = 0;
```
また，状態を遷移する最初に1度だけ以下の処理を実行する．
```
/* node state change process */
// -> initialize
virtual bool any_to_initialize_processing() = 0;
// -> ready (reset process)
virtual bool any_to_ready_processing() = 0; // stable, repair and ready
// -> force stop
virtual bool any_to_force_stop_processing() = 0; // stable, repair and ready
// -> normal flow
virtual bool ready_to_repair_processing() = 0;
virtual bool ready_to_stable_processing() = 0;
virtual bool repair_to_stable_processing() = 0;
virtual bool stable_to_repair_processing() = 0;
```
ノード自体が起動する時に他のノードを呼び出していなければ，上記の処理でtrueを返すと直ちに状態が遷移する．一方で関連する他のノードが同じ状態になるのを待ってから状態を遷移させたい場合は返り値falseを返すようにする．
configフォルダ内の対応するjsonファイルの中で，transit_watch_dog_timeに0以外の自然数を記入した時間だけTRANSITING状態で他のノードの状態が遷移するのを待つ．待っても遷移できない場合は，遷移前のステートマシンの状態に戻る．

> [自分メモ]遷移に失敗して戻るとき，上記の1度だけ処理をリセットする必要がある．しかしながらまだ実装されていない．早く書け！忘れず書け！

# ノードの追加
クラス内で書くことは，以下の4点である．

- 各State Machineの状態でのふるまい
- 起動時にjsonファイルに書かれたデータを読み込む処理
- 外部から受け取る指令の処理
- 内部状態を外部に公開してる共有メモリにコピーする処理

ただし，命令を受け取らないノードであれば指令の処理は書かなくてよい．内部状態を公開しないノードであれば，外部公開のコピー処理は書かなくてよい．jsonファイルから読み込むノード固有のデータがなければ書かなくてよい．State Machineがない場合は，READY, STABLE, REPAIR, FORCE_STOPに同じ処理を記述してほしい．

## ノードの設計・実装
```
- ノードの設計を書く。
    - ノードの機能・役割を決める
    - ノードが公開する(他のノードが見る・使う)状態量を決める
    - ノードが公開する(他のノードから受取り実行する)指令コマンドと指令値を決める
    - ノードが各状態マシンでどんな振る舞いをするのか決める
    - ノードにとってのErrorとWarningを決める
- ノードのhppファイルをb_system/nodeに書く
- ノードのcppファイルをb_system/nodeに書く
- 状態量の構造体をb_system/b_node_stateに書く
- 指令値の構造体をb_system/b_node_cmdに書く
- 指令コマンドをb_system/b_node_cmd/b_node_cmd_list.hppに書く
- ノードの設定データをConfig/jsonに書く
```

## ノードを使うための周辺設定
```
- CMakeLists.txtにノードのcppを書き込む
- node_list.hppにあるenum class behavior_node_listにノードを追加する
- node_util.cppにあるstd::string get_b_node_nameにノードを追加する
    - デバッグコードでノードの名前を表示ときに使用
    - 書かなくても動作するけど、名前がunknownになる
- b_hub.hppでノードのhppを呼び出し、privateメンバーに追加する
    - [Notion]後述する処理でmapに再度食べさせてるので、将来的には書かなくても良くなる処理
- b_hub.cppのコンストラクタ部分でb_node_listっていうmapにノードメンバーを食べさせる
```

## ノードの例

### ノードAが起動時にノードBも呼び出す

```
- b_simple_node_a
- b_simple_node_b
```

### Pubノード・Subノード
ROSでのPub/Subは指令も状態量も同じPubとなるが，bp_systemの中では指令をpubすることは相手cmdの共有メモリに書き込むことであり，指令をsubすることは定周期処理で共有メモリに書かれた指令を読み込むことである．
同様に，状態をpubすることは自分のstateの共有メモリに書き込むことであり，状態をsubすることは相手のstateの共有メモリを読み込むことである．

したがって，以下の2つのノードでは，まず b_example_sub_servo を設計するときに 指令・状態が決定する．b_example_pub_control はただ使うだけなので後で設計する．

```
- b_example_pub_control
- b_example_sub_servo
```
