Github链接：[Atopos-309/Compilation-principle (github.com)](https://github.com/Atopos-309/Compilation-principle)

测试方法：

cd到当前文件夹，编译：

```shell
gcc -o clang source.c
```

生成`clang.exe`

运行（要确保输入文件`demo.c`位于同一目录下，否则会报错）：

```shell
./clang
```

运行结束后，在同一目录下会生成`tokens.txt`

