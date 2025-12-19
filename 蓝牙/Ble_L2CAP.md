# L2CAP

![](C:\Users\user\AppData\Roaming\Typora\typora-user-images\image-20251110201337875.png)



## 1. 拆包

在上层中拆包

## 2. L2CAP_signal_packet_format

L2CAP分为固态的channel 和 动态的channel
在固态的channel中发送一些连接、配置信息，是在channel manager上，发送一些commands





**channel id = CID**
**CID = 0x0001 ACL-U**

**CID = 0x0005 LE-U**



发送一些信息是通过CID：0x0001 和 CID： 0x0005

commands 有三种通道，第一个是request，第二个是responses 第三个是indications



发送的请求叫做request，对于对端接收到的request叫做indication，回应叫做response，本地端叫做confirm

### Code(1 Byte)

### Identifier(1 Byte)

### DataLength



## Connection_请求响应



## Configuration_请求响应



## Disconnection_请求响应

## information_请求响应

