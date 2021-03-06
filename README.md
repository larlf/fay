# Fay语言

一种完全解释执行的语言。

<http://www.faylang.com>

# 编译

## 依赖库

### Visual Leak Detector

<https://kinddragon.github.io/vld/>

### Iconv

### Jansson

# 语言规范

## 数据类型

| 编号 | 代码 | 长度(bytes) | 名称 | 备注 |
| --- | --- | --- | --- | --- |
| 0 | Void | * | 空值 |  |
| 1 | Bool | 1 | 布尔类型 |  |
| 2 | Byte | 1 | 字节 |  |
| 3 | Int | 4 | 整型 |  |
| 4 | Long | 8 | 长整型 |  |
| 5 | Float | 4 | 低精度浮点数 |  |
| 6 | Double | 8 | 高精度浮点数 |  |
| 7 | String | * | 字符串 |  |
| 8 | Object | * | 对象 |  |
| 9 | Function | * | 函数 |  |

\* 表示是一个引用对象，一般是64位的指针。

## 库的管理

### 要解决的问题
* 版本命名的方式不统一
* 引用版本之间混乱，特别是不同的库引用了同一个库不同版本的时候，有可能会有冲突

### 命名规范
```
com_mylib.3.5.2.lib
<lib_name>.<marjor>.<minjor>.<version>.lib
```

* lib_name : 库的名称，以C风格命名。
* marjor : 大版本。
大版本相当会认为是完全向上兼容的，编译时相同大版本会自动取最新的库。
大版本发生改变时，会认为是不同的库，如mylib.4.3和mylib.5.1在编译中，会分别在不同扣命名空间中。
* minjor : 小版本。只有小版本变化的库里必须做到向上兼容。
* version : 自定义的版本信息，可以没有。可以用非.的任何字符。

### 问题及解决

因为大版本的变化，会造成里面类的定义在命名空间上完全分离，可能会造成下面的情况：mylib.3.5和mylib.4.1里都一个类：com.lib.ClassA，但是实际上他们是完全不同的两个类：ClassA@mylib:3和ClassA@mylib:4，这样如果有一个接口需要向里传这个类型的参数时，两者无法匹配！

**解决方式**：需要传参数的地方，不要用第三方库的数据类型。

如果库里有需要初始化或控制全局状态的静态变量，同样存在不同大版本的时候，com.lib.Var1@mylib:3和com.lib.Var1@mylib:4也是完全不同的两个静态变量，需要分别初始化。

**解决方式**：如果有需要初始化的地方，自己在库里定义初始化的方法，在方法中调用第三方的库。不要依赖让用户自己去处理第三方库的初始化。

# 语法

## using

```
using com.lib.package1 as p1;
... ...
p1.ClassA.fun1();
```

using语句可以给命名空间指定别名。using不是必须的，实际上大部分时候都可以不用using语句，直接写类名就行。



### 任务
- [ ] 不使用using时的类文件的查找
- [ ] 使用using引入命名空间
- [ ] 使用using引入类








