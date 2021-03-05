# log

#### 介绍
C++实现的极简的日志记录 用法参考log_unit_test.cpp
支持文件流式输出 类似于 LOG(INFO) << "content1" << "content2"
#### 软件架构
软件架构说明


#### 安装教程

1. git clone && cd log
2. cmake -Bbuild -H. && cd build && make

注意事项，在使用时需要在main函数之前添加以下代码:
namespace bm
{
log_manger s_log_manger;
}
