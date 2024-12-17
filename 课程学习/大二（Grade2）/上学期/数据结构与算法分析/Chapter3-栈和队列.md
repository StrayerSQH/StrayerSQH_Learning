# Chapter3-栈和队列
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;1.栈和队列是两种常用的线性存储表。<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;2.都限定关于插入和删除元素的操作在表的端点进行。栈只能在栈顶进行操作，队列仅能在队首和队尾进行操作。
## 3.1 栈
### 3.1.1 栈的基本概念
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;1.只允许在一段插入和删除元素的线性表。<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;2.允许插入和删除元素的一段成为栈顶`top`，另一端成为栈底`bottom`.<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;3.遵循后进先出（LIFO）规则。
### 3.1.2 顺序栈——栈的数组表示
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;在实现过程中主要通过向数组中依次插入元素即可（前提是不溢出）。需要注意的是：**因为采用数组的时候必须指定一个特定的大小`size`，则需要一个技术器`count`来根据插入和删除元素的情况进行判断是否溢出。同时这也是循序栈的一个缺点。
### 3.1.3 链式栈
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;1.借助链表的方式解决了顺序栈空间不足的问题，可以根据实际需求进行动态扩容。<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;2.在实现过程中与单链表的实现方式类似。栈底的后继是`NULL`，在栈顶需要一个`top`指针指向栈顶元素。
<div align=center>
<img src="https://github.com/StrayerSQH/StrayerSQH_Learning/blob/main/%E5%85%B6%E4%BB%96%E6%94%AF%E6%8C%81%E6%96%87%E4%BB%B6/%E6%95%B0%E6%8D%AE%E7%BB%93%E6%9E%84%E4%B8%8E%E7%AE%97%E6%B3%95%E5%88%86%E6%9E%90/%E9%93%BE%E5%BC%8F%E6%A0%88.jpg" width="500" height="70">
</div>

## 3.2 队列
### 3.2.1 队列的基本概念
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;1.队列是只允许在一端**对头**`front`删除，一端插入**队尾**`rear`的线性表。<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;2.遵循先进先出（FIFO）规则。
### 3.2.2 链队列
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;链式队列在入队是无队满的问题，但有队空的问题。队空的条件为：
<p align = 'center'>front == rear</p>
<div align=center>
<img src="https://github.com/StrayerSQH/StrayerSQH_Learning/blob/main/%E5%85%B6%E4%BB%96%E6%94%AF%E6%8C%81%E6%96%87%E4%BB%B6/%E6%95%B0%E6%8D%AE%E7%BB%93%E6%9E%84%E4%B8%8E%E7%AE%97%E6%B3%95%E5%88%86%E6%9E%90/%E9%93%BE%E9%98%9F%E5%88%97.jpg" width="500" height="70">
</div>

### 3.2.3 循环队列——队列的顺序存储结构
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;相较于链队列，循环队列的入队和出队规则以及状态判定都要变得更为复杂一些：<br>

-------------------
**1.顺序队列的入队和出队原则**<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;（1）入队时将新元素按`rear`指示位置加入再将队尾`rear`自加一：`rear = rear + 1`<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;（2）出队时将下表为`front`的元素去除，再将对头`front`自加一：`front = front + 1`<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;（3）队满时再入队将溢出错误。<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;（4）队空时再出队将队空处理。<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;（5）解决假溢出方法：将队列元素存放数组首位相连，形成循环队列。

------------------
**2.循环队列的计算判断**<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;对头`front`和队尾`rear`自加一时从`maxSize - 1`直接进到0，可用mod运算实现：<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;（1）对头`front`自加一：`front = (front +1) % maxSize`.<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;（2）队尾`rear`自加一：`rear = (rear + 1) % maxSize`.<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;（3）队列初始化：`front == rear == 0`.<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;（4）队空条件：`count == 0`.<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;（5）队满条件：`count == maxSize`.
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
