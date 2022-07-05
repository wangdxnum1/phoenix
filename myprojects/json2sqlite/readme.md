### 1.功能说明
这个小程序的功能主要是把json数据转换成sqlite3数据库;  
使用说明：json2sqlite.exe --json ransomware.json --database virus.db  
这个功能不是主要的，主要想想尝试下chromium-base库在平时开发中的便利程度  
### 2.程序模块说明
1.  chromium-base,是chromium项目的基本组件，一个字，爽。
2.  extension,网易自己再次封装的chromium-base，感觉还是有必要的，如果自己要用，也要提供之间这一层。
3.  gflags,goole命令行解析
4.  nlohmann-json,json解析

### 3.线程模型
主要是想使用chromium-base的线程模型和消息循环机制，我开启了3个线程循环
1.  主线程循环，类似UI框架的UI线程；
2.  json解析线程
3.  sqlite3数据库线程

通过chromium-base和extension可以快速实现线程消息循环，以及线程之间的交互，异常的顺滑。
