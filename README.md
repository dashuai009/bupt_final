# 毕设：专用压缩工具

## 摘要

智慧交通系统是近年来兴起的热门技术，其边缘计算设备上的雷达可以感知周围环
境，记录并保存了海量的数据。数据的高效存储成为了系统中重点解决的关键问题。针
对雷达感知数据的压缩可以带来存储成本的降低，加快数据传输速度。研究实现高效的
压缩工具还可以降低计算成本，为整个智慧交通系统带来很好的经济收益。

现在有很多通用的压缩工具，比如，gzip 的通用性和稳定性都非常高，但在超大规
模数据上没有明显优势。而 bzip2 的压缩率非常优秀，但需要消耗大量时间去计算，面
对超大规模数据，bzip2 的速度成为了致命缺点。本课题针对雷达感知数据的特殊性，设
计和实现了一个专用的压缩工具，能够对雷达感知数据进行压缩存储。此压缩工具综合
考虑 CPU 使用和压缩比，比一般的通用压缩工具（例如 gzip）有一定优势。

本课题提出的专用压缩工具主要通过多模匹配算法和匹配信息合并算法来压缩雷
达感知数据。首先，本课题提出了一个贪心算法，暴力寻找模式串集合。多模匹配使用
到了 AC 自动机，并对其代码实现进行调优，对比选择了更好的数据结构。在合并匹配
信息的过程中，本课题提出了另一个贪心的算法，以实现流式的压缩过程，并达到了较
好的压缩率。最终，本课题提出的压缩工具针对雷达感知数据，实现了 15.4% 的压缩率，
对比 gzip 为 18.1% 的压缩率；在实验环境下达到了 46.4MB/s 的压缩速度，对比 gzip 为
38.5MB/s。

## 使用方法

- 将数据放到data、目录下
- cmake编译
- 先使用findPattern寻找模式串，输出为配置文件
- 使用主程序压缩相关数据

## 测试

在test文件夹下有相关测试代码

## 性能对比

| 测试数据大小（字节） |gzip压缩率  |专用压缩工具压缩率| 
|------------|-----------|---------|
| 581079     | 0.181822  | 0.149124 |
| 5704583    | 0.180483  | 0.14949|
| 50839839   | 0.18165   |0.153751|
|  523582020 | 0.181186  | 0.154862|

| 测试数据大小（字节） | gzip压缩速度（MB/s） | 专用压缩工具压缩速度（MB/s） | gzip解压速度（MB/s） | 专用压缩工具解压速度（MB/s） |
|------------|----------------|------------------|-----|-----|
| 581079     | 32.926         | 46.0449          |130.874|219.972|
| 5704583    | 38.1797        | 46.7574          |220.813|274.115|
| 50839839   | 38.5277        | 46.3499          |239.47|272.697|
| 523582020  | 38.9205        | 46.3717          |241.273|274.121|