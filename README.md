# parallelism-overhead 并行性开销对比小实验
## TODO
- [x] (ls baseline) 用pthread机制起多个线程，在0号线程用malloc分配一大块内存，同步后各线程并发读写这块内存上不相交的区域；使用`cpu shield`(或者`set_affinity`)将线程指定到`cpu group上`；
- [ ] (dby) 基于baseline，用pthread机制起多个线程，每个线程用malloc分配一小块内存，使得总的内存大小和baseline的内存大小相同，再并发读；线程亲核性设置和baseline完全一致；
- [ ] (wsh) 通过链接选项把malloc的实现更换成tcmalloc jemalloc(而不是用libc的)，看看考虑了local和gloabl并发的malloc的并发读写性能怎么样。

## 实验环境
### 硬件环境
本实验的硬件环境为实验室配置的64物理核服务器，大家可以使用`numactl --hardware`查看该服务器的cpu拓扑结构，如下：
```
available: 4 nodes (0-3)
node 0 cpus: 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 64 65 66 67 68 69 70 71 72 73 74 75 76 77 78 79
node 0 size: 46967 MB
node 0 free: 23058 MB
node 1 cpus: 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 80 81 82 83 84 85 86 87 88 89 90 91 92 93 94 95
node 1 size: 48356 MB
node 1 free: 12795 MB
node 2 cpus: 32 33 34 35 36 37 38 39 40 41 42 43 44 45 46 47 96 97 98 99 100 101 102 103 104 105 106 107 108 109 110 111
node 2 size: 48378 MB
node 2 free: 18985 MB
node 3 cpus: 48 49 50 51 52 53 54 55 56 57 58 59 60 61 62 63 112 113 114 115 116 117 118 119 120 121 122 123 124 125 126 127
node 3 size: 48376 MB
node 3 free: 24916 MB
node distances:
node   0   1   2   3 
  0:  10  21  21  21 
  1:  21  10  21  21 
  2:  21  21  10  21 
  3:  21  21  21  10 
```
这里，为了测量时更准确，首先设置将`baseline`起的线程跑在`node 1`上，具体实现请参考[脚本]()