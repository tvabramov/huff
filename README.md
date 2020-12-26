# huff
huff is a yet another huffman (en/de)coding algorithm implementation.

## How to build it
Just clone the code and:
```sh
$ cd huff && make
```
Note: you need a c++14 compatible compiler and "make" utility.
If you want to run tests, just:
```sh
$ make test
```
But you will also need a python2 installed.

## Usage
huff is represented as two cli utilities - "encode" and "decode" for encoding and decoding files respectively. To encode "input.txt" to decoded "code.dat":
```sh
$ encode input.txt code.dat
```
If you want to decode it:
```sh
$ decode code.dat decoded.txt
```
Note: huff is operating by 8-bit chars only. So it may be effectivfe for compressing textual information, but not always for stuctured files such as images, video, etc.
