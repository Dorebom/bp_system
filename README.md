







# ノードの追加

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

```
- b_example_pub_control
- b_example_sub_servo
```