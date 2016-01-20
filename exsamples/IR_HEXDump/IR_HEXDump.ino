/*

   @file IR_HEXDump.ino
   @brief 受信した赤外線データを16進数で表示するサンプルプログラム
          赤外線リモコン受信モジュール(Vout)に接続するArduino側のピン番号は2番です。
   @author Kei Takagi
   @date 2016.1.16

   Copyright (c) 2016 Kei Takagi
   Released under the MIT license
   http://opensource.org/licenses/mit-license.php

*/

#include <IRControlReceiver.h>

// 赤外線リモコン受信モジュール(Vout)に接続するArduino側のピン番号は2番
IRControlReceiver ir(2);

void setup() {
  Serial.begin(9600) ;  // シリアル通信設定：9600bps
}

void loop() {
  int i = 0;
  int ret = 0;

  // 赤外線リモコンデータの受信領域を宣言
  unsigned char ir_dat[IRControlReceiver::IR_DATA_MAX_BYTE];

  // 赤外線リモコンデータの受信
  ret = ir.receive(ir_dat);
  if (ret > 0) {
    //受信した赤外線リモコンデータを16進数で表示
    Serial.print("IR:HEX[");
    for ( i = 0; i < IRControlReceiver::IR_DATA_MAX_BYTE; i++) {
      if ( *(ir_dat + i) <= 0x0F )Serial.print('0');
      Serial.print(*(ir_dat + i), HEX);
    }
    Serial.println("]");
  }
}
