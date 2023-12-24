# b_example_sub_servo

簡素なサーボ機能を実装する。

このノードは以下の5つの機能をもつ。
- サーボを動かす：void _update_servo_state(int add_value);
- サーボをリセットする：void _reset_encoder_count();
- サーボ状態を表示する：void _display_servo_state();
- ステートマシンを変更する：(nodeクラスで実装済み)
- 命令を読む

そして、ステートマシンが以下の状態でそれぞれ実行される
- Stable
    - 命令を読む
    - サーボを動かす
    - サーボ状態を表示する
- Ready
    - 命令を読む
    - ステートマシンを変更する (Ready -> Stable)
    - サーボ状態を表示する
- Any -> initialize
    - サーボをリセットする

また、自身が受ける命令コマンドは `example_node/data_struct/cmd/cmd_example_servo.hpp` に記載されており、
自身が公開する状態量は `example_node/data_struct/state/state_example_servo.hpp` に記載されている。

# b_example_pub_control

b_example_sub_servoが公開する命令コマンドを使って、b_example_sub_servoの内部状態を0に収束させる。

このノードは以下の2つの機能をもつ。
- b_example_sub_servoのステートマシンがReadyの場合、Stableになるように命令する
    - b_example_sub_servoがStableになったら自身をStableにする
- b_example_sub_servoの`encoder_count`を読み取って0に向かう命令を送る。
    - servo_state1->encoder_count < 0の場合、ADD_COUNTを送る
    - servo_state1->encoder_count > 0の場合、SUB_COUNTを送る
    - servo_state1->encoder_count == 0の場合、何も送らない
