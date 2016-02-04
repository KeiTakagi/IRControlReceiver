/*

   @file IRControlReceiver.cpp
   @brief 赤外線リモコン受信ライブラリ
          Infrared remote control receiver library
   @author Kei Takagi
   @date 2016.1.16

   Copyright (c) 2016 Kei Takagi
   Released under the MIT license
   http://opensource.org/licenses/mit-license.php

*/

#include "arduino.h"
#include "IRControlReceiver.h"

//! 赤外線リモコン受信データの最大バイト
int IRControlReceiver::IR_DATA_MAX_BYTE = IR_DATA_MAX_BYTE_SIZE;

//! デフォルトの赤外線リモコン受信モジュール(Vout)に接続するArduino側のピン番号
int IRControlReceiver::_PinNo = 2;

/**
   @brief コンストラクタ
*/
IRControlReceiver::IRControlReceiver()
{
  init();
}

/**
   @brief コンストラクタ
   @param (PinNo) 赤外線リモコン受信モジュール(Vout)に接続するArduino側のピン番号
*/
IRControlReceiver::IRControlReceiver(int PinNo)
{
  _PinNo = PinNo;
  init();
}

/**
   @brief 初期化
*/
void IRControlReceiver::init()
{
  pinMode(_PinNo, INPUT) ; // 指定されたピンを入力として使用
}

/**
   @brief 赤外線受信データ取得関数
   @param (*ir_data) 受信した赤外線データを返す
   @return 受信データのバイト数 0バイトの時は無受信 0バイト以上の時は受信
*/
int IRControlReceiver::receive(unsigned char *ir_data)
{
  unsigned long t = 0; // 起動からの時間(マイクロ秒)を記録。
  int i, dt  ;
  int ret = 0;

  for (i = 0; i < IR_DATA_MAX_BYTE; i++)*(ir_data + i) = 0x00; // 返却領域初期化

  // リーダ部(フレームの開始を示す同期パターン)のチェック
  if (digitalRead(_PinNo) == LOW) {
    t = micros() ; // 現在時刻(μs)取得
    while (digitalRead(_PinNo) == LOW) ; // リーダ部が来るのを待つ(HIGH(ON)になるまで待つ)
    t = micros() - t ; // リーダ部が来た時間(OFF→ONになった)を計測する。
  }

  i = 0 ;
  // リーダ部有り(3300μsより大きい時間にて判断する)
  // Panasonic(DIGA)が3400μs　古いPanasonic(不明)だと9000μs
  // 東芝(REGZA)だと9000μs
  if (t > 3300) {
    while (digitalRead(_PinNo) == HIGH) ; // リーダ部のHIGH(ON)時間が終わるまで待つ

    // データ部読込
    // NEC or 家電製品協会
    while (1) {
      while (digitalRead(_PinNo) == LOW) ; // LOW(OFF)の部分は無視
      t = micros() ;
      while (digitalRead(_PinNo) == HIGH) {// LOW(OFF)になるまで待つ
        if ( micros() > t + 10800) { // 10800μs(送信間隔)経過してもHIGH(ON)なら中止
          break ;
        }
      }
      t = micros() - t ;
      if (t > 10800) break ;// ストップ信号を受け取った場合 ループを抜ける
      dt = i / 8 ;
      // ON部分が長い場合は1。ON部分が短い場合0。
      *(ir_data + dt) = *(ir_data + dt) | ( t > 1000 ? 0x01 : 0x00) << (7 - i % 8);
      i++;
      ret = dt + 1;


      if (i >= (IR_DATA_MAX_BYTE * 8 - 1)) {
        ret = -1;  // 最大データ数の場合 ループを抜ける
        break;
      }
    }
  } else if (t > 50) {
    // リーダ部のHIGH(ON)までの時間が1000μs以下だとSONY製
    // ただし50μs以下はノイズの可能性あり
    while (digitalRead(_PinNo) == HIGH) ; // リーダ部のHIGH(ON)時間が終わるまで待つ
    // データ部読込
    // SONY
    while (1) {
      t = micros();
      while (digitalRead(_PinNo) == LOW) ;// LOW(OFF)の部分は無視
      t = micros() - t ;
      dt = i / 8 ;
      // ON部分が長い場合は1。ON部分が短い場合0。
      *(ir_data + dt) = *(ir_data + dt) | ( t > 1000 ? 0x01 : 0x00) << (7 - i % 8);
      i++;
      ret = dt + 1;

      t = micros() ;
      while (digitalRead(_PinNo) == HIGH) {// LOW(OFF)になるまで待つ
        if ( micros() > t + 4500) {// 4500μs(送信間隔)経過してもHIGH(ON)なら中止
          break ;
        }
      }

      t = micros() - t ;
      if (t > 4500) break ;// ストップ信号を受け取った場合 ループを抜ける

      if (i >= (IR_DATA_MAX_BYTE * 8 - 1)) {
        ret = -1;  // 最大データ数の場合 ループを抜ける
        break;
      }
    }
  }

  return ret;
}
