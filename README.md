# everymsg.hpp
一个极其简单易用的C++数据结构序列化工具。整个代码库只有两个.hpp文件，不依赖任何第三方代码库。

优点：简单易用、源代码一看就懂、应用层代码可读性好、高性能

缺点：仅支持C++、版本控制需要应用层考虑


# 应用场景：
在C++语言编程过程中需要将结构化数据串行化存储或者经网络发送时，你就可以使用everymsg来实现。


# 实现原理：
通过宏函数侵入机制以及C++语言的友元和模板的特性的组合，实现C\C++任意数据结构的序列化和反序列化。
1. 宏函数侵入应用层锁定需要参与序列化的成员以及序列化的顺序。
2. 利用友元和模板的特性通过一个工具类（class EveryMsg;）来将数据排列在连续的buffer中，或从buffer中依次取出。


# 实现思路：
1. 数据结构序列化其实就是结构化的数据串行化的一个过程，反序列化就是将串行化后的数据还原为结构数据对象。
2. 从编程语言层面上讲数据只有两种，即整形和字符串。例如下图LoginRsp结构包含listCityInfo，然后CityInfo又包含listUserInfo，可以看出数据只有int和string，我们只需要把这些数据按照固定的顺序排列在一个连续的buffer里就可以实现序列化。

   ![image](https://user-images.githubusercontent.com/84183800/127149514-e73d3156-245d-4e9e-af11-dee455ecad1e.png)
   
3. 如果我们针对每一种数据结构都去用代码实现排列规则和顺序那么代码量会比较高，而且也很繁琐(google的protobuf工具就是用的这种方式)。其实我们可以通过宏函数入侵的方式由应用层去指定排列顺序，同时通过友元和模板的机制实现排列数据的规则，并提炼出一个泛型的工具类。
   
   sbuffer.hpp 一个智能的缓冲区处理，封装对内存数据的操作。
   
   everymsg.hpp 定义了序列化的框架规则以及标准stl容器的适配。
   
# Demo
  Demo可以通过vs2015编译通过
  
  ![image](https://user-images.githubusercontent.com/84183800/127163222-abcec2e6-4674-471f-9b72-76ad7bd9fb0b.png)
  
  
# 使用协议：what?
  鉴于在天朝神马协议都被无视，甚至很多大公司都不遵守，故索性采用了自定义协议~O(∩_∩)O~
  
  【Do what the fuck you want to public lincese】
  
  
# 使用案例
1. 2015年《深圳市聚橙网络技术有限公司》聚橙票务系统服务器间通信协议。
2. 2020年《深圳齐心好视通云计算有限公司》会议系统部分服务通信协议。

# 技术交流
  QQ：519896717
  
