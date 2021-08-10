# RemoteMemorymodule
Load the evilDLL from socket connection without touch disk



Inspired by @Rvn0xsy and the famous [MemoryModule](https://github.com/fancycode/MemoryModule) project



#### Server

​	Maked a simple socket server via c++ which is called `PigSender`(only work on Linux)

Responsible for processing the request sent by the client and transferring the DLL

#### Client

1. Added a simple anti-simulation method, and receive DLL file from remote Server

2. Finally, simply call MemoryModule

## Usage:

1. Put your DLL on the VPS and specify the file to be sent and the listening port

![image-20210810085812853](https://images-1258433570.cos.ap-beijing.myqcloud.com/images/20210810085814.png)

2. In the Client, just specify the address listened in the first step

![image-20210810085922601](https://images-1258433570.cos.ap-beijing.myqcloud.com/images/20210810085923.png)

**Thanks to this excellent "non-landing" technology, you can use this project to reduce the pain of evasion anti-virus in some temporary environments**

