# Chapter1-绪论
## 1.1 数据结构的基本概念
### 1.数据（data）
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;主要包括数值型数据和非数值型数据。
### 2.数据元素（data element)
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;描述数据的基本单位。可以由多个数据项（data item)组成。<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;数据项是具有独立含义的最小标识单位。例如描述员工的数据元素可以由姓名、出生日期、职务等的数据项组成，其中出生日期又可以由年、月、日组成组合项。<br>
### 3.数据结构
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;由某一数据元素的结合及改集合中所有数据元素之间的关系组成。记作：
<p align="center">DataStructure = {D, S}</p>
其中，D是一个数据元素的有限集合， S是定义在D中的数据元素之间的关系的有限集合。

### 4.静态数据结构与动态数据结构
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`静态数据结构`采用的是连续的存储区域。<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;优点：可以直接访问各个数据，各个数据的地址可以根据首地址+偏移量的方式求得，访问复杂度为O(1)。<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;缺点：长度固定，新增或删除某一项数据需要大片的移动其他数据项，花费时间较多。<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`动态数据结构`采用的不是连续存储区域，而是每一个节点包含一个数据和下一个节点的指针。<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;优点：能够动态的进行扩容和删除。<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;缺点：不利于随即访问，每次访问特定元素时都需要花费较长时间，为O(n)。<br>
## 1.2 抽象数据类型及其实现
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;抽象数据类型可用 `(D, R, O)三元组`表示。其中D是数据对象，R是D上的关系集， O是对D的基本操作集。
## 1.3 算法与算法分析
-------------------------------------------------
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;1.`正确性`:首先不含语法错误，其次对于计组输入数据能够得出满足要求的结果，接下来对于苛刻数据依旧能够得出满足要求的结果，最后对于一切输入数据都能输出满足要求的结果。<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;2.`具体性`:一个算法必须有一系列具体操作组成，即所有操作必须可实现，所有操作都是可读可执行可在有限时间内完成。<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;3.`确定性`:算法中的所有操作都必须有确切的含义，不能产生歧义。<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;4.`有限性`:算法中的每个步骤都能在有限时间内完成。<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;5.`健壮性`:当输入无效数据时，算法能做适当的处理并及时反馈，而不应死机或输出异常结果。<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;6.`可读性`:顾名思义方便阅读理解。<br>

-------------------------------------------------
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;衡量算法的两个常用标准为`时间复杂度O(n)`和`空间复杂度S(n)`。
