  | 2016K8009908007-薛峰-Big_Bonus/
    |--big_bonus：该项目源码。
    |
    |--pktRxTx.c：修改过的pktRxTx程序源码。
    |
    |--hello：需要发送的小程序。


启动流程：
      1、mkfs
      2、touch hello
      3、exec 0
      4、exec 2
      5、发送小程序
      6、cat hello检查开头是不是ELF，若不是则接受数据包错误
      7、重启板子
      8、./hello，可看到第一行打印出“Hello World！”