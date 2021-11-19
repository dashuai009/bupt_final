## 2021.11.17

|test_in.txt|test_out.txt|test_out.txt.gz|
|---|---|---|
|580268|253703|85656|

> 第一步，将defaultConfig.txt里的模式串替换，将源文件题解减小一多半，再用gzip（第二步）可以比直接用gzip好一点

ToDo
- 速度太慢，std::string花费时间太多
- 不是流式压缩，大文件肯定无法直接读入内存。struct Buffer;
- 最后必须是一步完整压缩/解压
- 考虑将数字（非“”内的）单独压缩
- 考虑正则匹配引擎，`\{"x":-?[0-9]*,"y":-?[0-9]*,"z":-?[0-9]*\}`这样的可以用一个占位符和三个整数来替代。test_in.txt里有9485个。
- Log函数。

## 11.18

done
- Log
- Cmake
- move std::string
- Buffer

![](p1.png)


## 11.19
|test_in.txt|test_out.txt|test_in.txt.gz|time myzip test_in.txt| time gzip test_in.txt|
|---|---|---|---|---|
|990777330|50839839|177155091|9s|23s|


