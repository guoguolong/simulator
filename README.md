# Vending Machine Project - 数据持久化

## 系统要求

macOS、Linux 、Unix-Like System

## 安装gcc

```
brew install gcc
```

## 编译运行

```
cd $project_dir
sh ./build.sh
```

## 特性

1. CSV 文本文件存储商品数据；
2. 普通文本文件存储售卖机状态数据；
3. typedef 定义结构，使用起来更自然；
4. OOP 封装: ProductService 的用法；
5. 增加重置操作，使数据恢至复原始状态。