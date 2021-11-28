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


# 11.20
- 添加了huffman编码，速度降低，压缩率提高。
- 一个bug：int(char)时，这个char有符号，会进行符号位扩展。int(char(-125))会是一个负数。

|test_in4.txt|time myzip|time myunzip|test_out.txt|压缩率|
|---|---|---|---|---|
|523582020|18.572s |20.100s|124765778|23.83%|

- 唉，速度慢。压缩率低。到底还玩不玩了？？？？

# 11.21

> 今天过生日！屁事没干成
> 

- cmp -i a.txt b.txt 现实a、 b文件差异的字节位置和内容

## 11.22

|test_in4.txt|time myzip|time myunzip|test_out.txt|压缩率|
|---|---|---|---|---|
|523582020|17.141s |17.007s|124765778|23.83%|

## 11.25

ToThink
- Huffman编码结果中1多还是0多？（最小为左子树，次小为右子树，左0右1,则1的个数大于等于0的个数）

## gzip format


The gzip file format was standardized in Internet RFC 1952 [RFC1952](https://www.ietf.org/rfc/rfc1952.txt).
This section briefly describes the format and comments on some
implementation details.

The pkzip format imposes a lot of overhead in various headers, which
are useful for an archiver but not necessary when only one file is
compressed. gzip uses a much simpler structure. Numbers are in little
endian format, and bit 0 is the least significant bit.
A gzip file is a sequence of compressed members. Each member has the
following structure:

|||
|---|---|
|2 bytes | magic header  0x1f, 0x8b (\037 \213)|
|1 byte |  compression method (0..7 reserved, 8 = deflate)|
|1 byte |  flags|
|bit 0 set: |file probably ascii text|
|bit 1 set:| header CRC-16 present|
|bit 2 set:| extra field present|
|bit 3 set: |original file name present|
|bit 4 set: |file comment present|
|bit 5,6,7:| reserved|
|4 bytes  |file modification time in Unix format|
|1 byte  | extra flags (depend on compression method)|
|1 byte  | operating system on which compression took place|


|||
|---|---|
|2 bytes | optional part number (second part=1)|
|2 bytes | optional extra field length|
|? bytes | optional extra field|
|? bytes | optional original file name, zero terminated|
|? bytes | optional file comment, zero terminated|
|2 bytes | optional 16-bit header CRC|
|? bytes  |compressed data|
|4 bytes | crc32|
|4 bytes|  uncompressed input size modulo 2^32|

The format was designed to allow single pass compression without any
backwards seek, and without a priori knowledge of the uncompressed
input size or the available size on the output media. If input does
not come from a regular disk file, the file modification time is set
to the time at which compression started.

The **timestamp** is useful mainly when one gzip file is transferred over
a network. In this case it would not help to keep ownership
attributes. In the local case, the ownership attributes are preserved
by gzip when compressing/decompressing the file. A timestamp of zero
is ignored.

Bit 0 in the **flags** is only an optional indication, which can be set by
a small lookahead in the input data. In case of doubt, the flag is
cleared indicating binary data. For systems which have different
file formats for ascii text and binary data, the decompressor can
use the flag to choose the appropriate format.

The **extra field**, if present, must consist of one or more subfields,
each with the following format:

|||
|---|---|
|subfield id   | 2 bytes|
|subfield size | 2 bytes  (little-endian format)|
|subfield data||

The subfield id can consist of two letters with some mnemonic value.
Please send any such id to <gzip@gnu.org>. Ids with a zero second
byte are reserved for future use. The following ids are defined:

Ap (0x41, 0x70) : Apollo file type information

The subfield size is the size of the subfield data and does not
include the id and the size itself. The field 'extra field length' is
the total size of the extra field, including subfield ids and sizes.

It must be possible to detect the end of the compressed data with any
compression format, regardless of the actual size of the compressed
data. If the compressed data cannot fit in one file (in particular for
diskettes), each part starts with a header as described above, but
only the last part has the crc32 and uncompressed size. A decompressor
may prompt for additional data for multi-part compressed files. It is
desirable but not mandatory that multiple parts be extractable
independently so that partial data can be recovered if one of the
parts is damaged. This is possible only if no compression state is
kept from one part to the other. The compression-type dependent flags
can indicate this.

If the file being compressed is on a file system with case insensitive
names, the original name field must be forced to lower case. There is
no original file name if the data was compressed from standard input.

Compression is always performed, even if the compressed file is
slightly larger than the original. The worst case expansion is
a few bytes for the gzip file header, plus 5 bytes every 32K block,
or an expansion ratio of 0.015% for large files. Note that the actual
number of used disk blocks almost never increases.