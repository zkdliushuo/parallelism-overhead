# parallelism-overhead 并行性开销对比小实验
## TODO
[ ] (ls) 对比实验，对照是一个主线程，malloc分配一大块内存，再起多个子进程，并发读之前分配的这个内存，设置每个子进程读的区域互不重叠；在实验一个程序，直接起多个子进程，每个用malloc分配一小块(和对照读的区域的size应该是一样的才可以)，并发读;
[ ] (wsh) 通过链接选项把malloc的实现更换成tcmalloc jemalloc(而不是用libc的)，看看考虑了local和gloabl并发的malloc的并发读写性能怎么样。
