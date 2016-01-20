/*

   @file IRControlReceiver.h
   @brief 赤外線リモコン受信ライブラリ
          Infrared remote control receiver library
   @author Kei Takagi
   @date 2016.1.16

   Copyright (c) 2016 Kei Takagi
   Released under the MIT license
   http://opensource.org/licenses/mit-license.php

*/

#ifndef __IR_Control_Receiver_INCLUDE__
#define __IR_Control_Receiver_INCLUDE__

class IRControlReceiver
{
  public:
    //! LEDの接続されているピン番号を保存する変数
    static int _PinNo;
    //! 赤外線リモコン受信データの最大バイト
    static int IR_DATA_MAX_BYTE;

    /**
       @brief コンストラクタ
    */
    IRControlReceiver() ;

    /**
       @brief コンストラクタ
       @param (PinNo) 赤外線リモコン受信モジュール(Vout)に接続するArduino側のピン番号
    */
    IRControlReceiver(int inPinNo) ;

    /**
       @brief イニシャライズ
    */
    void init();

    /**
       @brief 赤外線受信データ取得関数
       @param (ir_dat) 受信した赤外線データを返す
       @return 受信データのバイト数 0バイトの時は無受信 0バイト以上の時は受信
    */
    int receive(unsigned char *ir_dat) ;
};
#endif

