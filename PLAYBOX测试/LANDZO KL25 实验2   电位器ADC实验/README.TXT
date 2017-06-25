本实验将实现如下功能：本实验通过KL25开发板两种方式检测按键，当按键按下后将通过串口（UART1）打印出哪个键被按下。


注意：
1、  方式1：   #define Key_Interrupt     // 定义按键中断 检测按键时 两者屏蔽一个保留一个
             //#define Key_INPUT       // 定义按键输入 检测按键时 两者屏蔽一个保留一个
              如果定义了Key_Interrupt，则程序检测按键是通过按键外部输入中断进行的。

     方式2： //#define Key_Interrupt     // 定义按键中断 检测按键时 两者屏蔽一个保留一个
               #define Key_INPUT       // 定义按键输入 检测按键时 两者屏蔽一个保留一个
              如果定义了Key_INPUT，则程序检测按键是通过检测按键外部输入进行的。

2、各按键所对应的IO：
        UP_Key      PTA1  
        LEFT_Key    PTA12 
        DOWN_Key    PTA5   
        RIGHT_Key   PTA13 

        A_Key       PTA14
        B_Key       PTA16
        C_Key       PTA17
        D_Key       PTA15    




               	KL25Z系统版
               	2015-7-30
		芜湖蓝宙电子科技有限公司
                电话：
                淘宝店：http://landzo.taobao.com/
                公司网站：http://www.landzo.com/
                论坛 ：http://www.landzo.cn
              