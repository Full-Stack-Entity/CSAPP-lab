# CSAPP Labs

`lab/` 汇总了本仓库中的 CS:APP 实验实现与复查记录，覆盖 bit-level expression、assembly、pipeline、cache、process control、heap allocator 和 network proxy 等主题。  

这组实验的价值不只在于逐个完成课程任务，更在于把一条底层系统链路完整串起来：表示层的约束如何影响实现，控制流和 calling convention 如何决定程序行为，pipeline 和 cache 如何影响性能，进程、内存和网络代码又如何进入长期运行、并发和资源管理的问题域。

## 总览


| Lab                                | 主题                       | 当前保留的交付物                                           | 实验结果/成绩                           |
| ---------------------------------- | ------------------------ | -------------------------------------------------- | --------------------------------- |
| [lab1-datalab](./lab1-datalab)     | 位级运算与 IEEE754            | `bits.c`                                           | `62/62`                           |
| [lab2-bomblab](./lab2-bomblab)     | 逆向分析与控制流重建               | `answer.txt`、`bomb_asm.txt`                        | 六关 + secret phase 本地通过            |
| [lab3-attacklab](./lab3-attacklab) | 栈破坏、code injection、ROP   | `ans/`、`ctarget_asm.txt`、`rtarget_asm.txt`         | 五个 payload 本地通过                   |
| [lab4-archlab](./lab4-archlab)     | ISA 扩展与 pipeline 性能优化    | `seq-full.hcl`、`pipe-full.hcl`、`sim/pipe/ncopy.ys` | Part B 测试全过，Part C `59.3/60`      |
| [lab5-perflab](./lab5-perflab)     | cache-aware C 优化         | `kernels.c`                                        | best `Rotate 33.6`，`Smooth 117.9` |
| [lab6-cachelab](./lab6-cachelab)   | cache simulator 与矩阵转置优化  | `csim.c`、`trans.c`                                 | `csim 27/27`，Part B `53/53`       |
| [lab7-shlab](./lab7-shlab)         | shell、job control、signal | `tsh.c`                                            | `trace01-16` 全通过                  |
| [lab8-malloclab](./lab8-malloclab) | allocator 设计与性能权衡        | `mm.c`                                             | `69/100`                          |
| [lab9-proxylab](./lab9-proxylab)   | 并发网络代理与缓存                | `proxy.c`                                          | `70/70`                           |


## 实验解法和个人收获

这组 lab 如果按课程章节拆开，可以分成数据表示、逆向、体系结构、性能优化、进程、内存和网络几个部分；如果从实现角度看，它们更像一条连续的系统训练链。`datalab` 强调表示与约束，`bomblab/attacklab` 强调控制流与栈语义，`archlab/perflab/cachelab` 强调对机器行为的具体回应，`shlab/malloclab/proxylab` 则把问题推进到状态管理、错误路径、资源释放和并发控制。

从个人收获看，这组实验最重要的不是“覆盖了哪些知识点”，而是建立了一套稳定的方法：先找约束和不变量，再选实现；先做可验证的闭环，再考虑优化；优化必须和硬件行为、性能指标或 trace 结果对应，而不是只停留在写法层面。这也是后续继续看系统和具身相关问题时最常复用的一套分析框架。

## Lab 1: Data Lab

对应文件：`[lab1-datalab/bits.c](./lab1-datalab/bits.c)`

这一部分的核心是，在极少操作符和严格规则下表达完整逻辑。当前实现里，`conditional` 使用全 `1` / 全 `0` mask 模拟三目运算，`logicalNeg` 利用 `x | (-x)` 的符号位特征判断是否为零，`howManyBits` 则采用逐级缩小范围的 bit-level binary search。浮点题部分按 `sign / exp / frac` 拆分，再分别处理 denorm、normal 和 overflow。

这一关最大的训练价值在于“表达式是否真正贴合底层语义”。常见坑主要有两类：一类是 `isLessOrEqual` 这类题中的 overflow，如果不先区分同号和异号，直接看减法结果很容易出错；另一类是浮点题在 denorm 和指数边界上漏分类。

实验结果/成绩：`./driver.pl` 返回 `62/62 [36/36 Corr + 26/26 Perf]`。

## Lab 2: Bomb Lab

对应文件：`[lab2-bomblab/answer.txt](./lab2-bomblab/answer.txt)`、`[lab2-bomblab/bomb_asm.txt](./lab2-bomblab/bomb_asm.txt)`

这一部分保留的是最终答案和反汇编。解法路径基本是标准逆向流程：`phase_1` 是字符串比较，`phase_2` 是倍增序列，`phase_3` 对应 jump table / switch，`phase_4` 需要理解 `fun4` 的递归二分与返回值编码，`phase_5` 依赖字符低 4 bit 映射，`phase_6` 本质上是链表重排与单调性检查，secret phase 则回到树路径编码。

这一关的关键收获，是把“逐条翻译汇编”推进到“重建控制逻辑和数据结构”。典型坑则是过早陷入局部细节，例如在 `phase_6` 里只盯地址运算而忽略链表重连，或者在 `phase_4` 里没有先识别出递归路径编码这一整体结构。

实验结果/成绩：按 `answer.txt` 中的真实输入顺序重放后，六关和 secret phase 全部通过。

## Lab 3: Attack Lab

对应文件：`[lab3-attacklab/ans](./lab3-attacklab/ans)`、`[lab3-attacklab/ctarget_asm.txt](./lab3-attacklab/ctarget_asm.txt)`、`[lab3-attacklab/rtarget_asm.txt](./lab3-attacklab/rtarget_asm.txt)`

Attack Lab 的阶段划分很清楚：前半部分在 `ctarget` 上做 code injection，后半部分在 `rtarget` 上转向 ROP。现有交付物里，`ctarget1_ans.txt` 是直接覆盖返回地址，`ctarget2_inject.s` 已经体现出 `push touch2 + mov cookie -> %rdi + retq` 的基本模式，`ctarget3_inject.s` 进一步处理了字符串参数；到了 `rtarget4_ans.txt` 和 `rtarget5_ans.txt`，重点就变成了在 gadget farm 中拼接寄存器传递和控制转移。

这一关的训练价值主要在于把 calling convention、栈布局、返回地址和寄存器约定真正连起来。最典型的坑是阶段切换后的思路惯性：如果进入 `rtarget` 后仍沿用 code injection 视角，就会直接撞上 NX 约束，必须改成“现有 gadget 能完成什么状态转移”的分析方式。

实验结果/成绩：`ctarget` 1-3 关和 `rtarget` 4-5 关 payload 均已本地复现通过。

## Lab 4: Architecture Lab

对应文件：`[lab4-archlab/sim/seq/seq-full.hcl](./lab4-archlab/sim/seq/seq-full.hcl)`、`[lab4-archlab/sim/pipe/pipe-full.hcl](./lab4-archlab/sim/pipe/pipe-full.hcl)`、`[lab4-archlab/sim/pipe/ncopy.ys](./lab4-archlab/sim/pipe/ncopy.ys)`

这一部分分成两层。Part B 在 `SEQ` 和 `PIPE` 两套控制逻辑里补齐 `iaddq`，重点不是单独增加一条指令，而是把它完整接入 `need_regids`、`need_valC`、`srcB / dstE`、`aluA / aluB`、`set_cc` 以及 forwarding / stall / bubble 等整条路径。这里最直接的收获，是把“ISA 是一套跨 stage 契约”这件事真正具体化。

Part C 是当前仓库里最有代表性的性能优化实现之一。`ncopy.ys` 的主循环按 `8` 个 word 展开，并且不是简单复制 baseline loop，而是有意识地交错安排 `mrmovq / rmmovq / andq / jle / iaddq`，尽量把 load-use hazard 藏到后续指令里。尾部处理也没有回退到统一小循环，而是先按 `0-3` 与 `4-7` 拆段，再继续手动分支展开到具体剩余长度。这种写法牺牲了一些整洁性，但直接换来了 control overhead 和流水线停顿的下降。

这里最典型的坑有两个：一是“展开”本身并不等于优化，如果 hazard 和分支路径不处理，CPE 不会明显下降；二是尾部处理如果重新退回高开销小循环，很容易把主循环省下来的周期重新丢掉。

实验结果/成绩：

- `seq-full.hcl` / `pipe-full.hcl` 对 `iaddq` 的支持测试全部通过
- `ncopy.ys` 通过 `68/68` correctness test
- `benchmark.pl` 返回 `Average CPE 7.54`，`Score 59.3/60.0`

## Lab 5: Performance Lab

对应文件：`[lab5-perflab/kernels.c](./lab5-perflab/kernels.c)`

这一部分继续围绕性能，但实现层面已经完全回到 C。`rotate` 上保留了明显的迭代轨迹：先做 `8x8` 手工展开，再继续削减地址计算；`smooth` 则保留了多版思路，从内联 `avg`，到边界与内部拆开处理，再到 `smooth3` 里显式维护列和的 sliding-window 写法。

这一关最直接的收获，是把“重复工作”拆成不同来源去看：有些来自算术，有些来自访存，有些来自边界逻辑。做对这种拆分之后，优化就不再是盲目展开，而是针对具体瓶颈做响应。典型坑则是过度追求手工展开，导致代码只剩体力活，却没有真正解决热点。

实验结果/成绩：

- `Rotate` 最优版本：`33.6`
- `Smooth` 最优版本：`117.9`

## Lab 6: Cache Lab

对应文件：`[lab6-cachelab/csim.c](./lab6-cachelab/csim.c)`、`[lab6-cachelab/trans.c](./lab6-cachelab/trans.c)`

Part A 是一套直接的 LRU cache simulator，使用 `LRU_time` 维护替换顺序；Part B 的重点则完全落在“为什么 miss”上。`32x32` 版本采用标准 `8x8 block + register temporary`，`64x64` 单独做了冲突规避：先把上半块搬进 `B` 的缓冲区域，再重排后半块和缓冲数据，尽量压低 direct-mapped cache 上最规整的冲突 miss；`61x67` 则使用 `16x16` blocked transpose 和边界处理来换取稳定表现。

这一关的关键收获，是把数据布局、访问顺序和临时缓冲位置都当成核心逻辑，而不是附属细节。典型坑在于把 `32x32` 上的写法直接平移到 `64x64`，忽略后者在 direct-mapped cache 上更强的规律性冲突。

实验结果/成绩：

- `test-csim` 返回 `27/27`
- `test-trans` miss 数：
  - `32x32: 288`
  - `64x64: 1148`
  - `61x67: 1993`
- 按本地 `driver.py` 的阈值折算，Part B 为满分，因此总分是 `53/53`

## Lab 7: Shell Lab

对应文件：`[lab7-shlab/tsh.c](./lab7-shlab/tsh.c)`

这一部分把进程组、前后台、信号转发和作业列表真正接成了一个可交互系统。当前 `tsh.c` 中，`eval` 负责 fork/exec 与前后台判定，`do_bgfg` 处理 job id / pid 两类入口，`sigchld_handler` 负责回收和状态更新，`sigint_handler`、`sigtstp_handler` 则把终端信号转发给前台作业组。

这一关的主要收获，是把 shell 看成一个持续维护状态的长生命周期程序，而不只是“读命令并执行”的循环。相应的典型坑也都落在状态变化上：`bg/fg` 参数解析和错误输出、signal handler 中对 job list 的更新时机、以及 `waitfg` 等待的到底是“进程退出”还是“它不再是 foreground job”。

当前仓库中的 `tsh.c` 是在已有 shell 逻辑基础上迁入 lab scaffold 并对齐 `tshref` 输出格式后形成的版本。

实验结果/成绩：`trace01-16` 全部返回 `0`。

## Lab 8: Malloc Lab

对应文件：`[lab8-malloclab/mm.c](./lab8-malloclab/mm.c)`

这一部分保留的是一版 textbook-style baseline allocator：boundary tag、implicit free list、first-fit、split、immediate coalescing，以及最直接的 `malloc-copy-free` 版 `realloc`。它不是高性能版本，但结构清楚，足够适合作为 allocator 设计的基线。

这一关最大的训练价值，在于把 block layout、不变量和 heap consistency check 变成 allocator 的第一语言。header/footer 是否匹配、相邻 free block 是否被合并、heap 头尾是否保持约定状态，都直接决定 allocator 是否正确。与此同时，这一版实现也很直观地暴露出 implicit free list 和 first-fit 的吞吐上限，因此“正确”和“快”的边界会看得很清楚。

这里最典型的坑，一是 `coalesce` 的四种情况如果分类不清，很容易把 header/footer 写坏；二是 `realloc` 如果过早追求局部修补，会迅速把边界问题放大。

实验结果/成绩：`./mdriver -V` 返回 `Perf index = 78/100`。

## Lab 9: Proxy Lab

对应文件：`[lab9-proxylab/proxy.c](./lab9-proxylab/proxy.c)`

这是整组实验里最接近完整系统程序的一项。当前 `proxy.c` 采用单文件实现，把顺序代理、线程化连接处理、HTTP/1.0 请求重写以及并发可读的内存 cache 收在一起。实现重点主要有三块：解析 `host / port / path` 并按题目要求重写关键请求头；用 detached thread 处理并发连接，避免阻塞请求拖住整个代理；用 per-entry `pthread_rwlock_t` 和全局元数据锁实现近似 LRU cache，让多线程读 cache 成为可能。

这一关最大的收获，是把并发、I/O 和状态缓存放到同一个程序里以后，很多看似独立的小问题会立刻耦合起来，例如 binary-safe I/O、client 提前断开、object size 限制和锁粒度选择。这里的典型坑也正集中在这些耦合点上：大锁实现虽然简单，但题目明确不接受；只按文本 I/O 思路写会在图片和二进制文件上出错；请求头处理过于随意又会带来兼容性问题。

实验结果/成绩：

- 官方 `driver.sh`：`70/70`
- 手工 `curl --proxy http://localhost:<proxy_port> http://localhost:<tiny_port>/home.html` 返回 `HTTP/1.0 200 OK`
- 关闭 `tiny` 后再次请求同一对象，cache 命中成功

## 方法总结

这组实验最后沉淀下来的方法可以概括成一段话：系统题最先要找的是约束和不变量，而不是代码形状；`datalab` 的表达式规则、`malloclab` 的 block layout、`shlab` 的 job state 和 `proxylab` 的 HTTP / cache 一致性都在反复强化这一点。另一方面，优化必须直接回应机器行为，`archlab`、`perflab` 和 `cachelab` 的经验都说明，真正有效的优化一定和 load-use hazard、branch overhead、address calculation 或 cache conflict 这类具体机制绑定，而不是停留在“代码写得更复杂了”这一层。
