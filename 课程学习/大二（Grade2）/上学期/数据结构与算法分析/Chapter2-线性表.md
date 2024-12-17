# Chapter2-线性表
## 2.1 线性表的定义
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;定义 n(≥0)个数据元素的有限序列,记作:
<p align="center">(a<sub>1</sub>, a<sub>2</sub>,…,a<sub>n</sub>)</p>
a<sub>1</sub> 是表中数据元素,n是表长度。a是线性表中数据元素,n是线性表长度,其中a,称为a<sub>i+1</sub>的直接前驱,简称为前驱，a<sub>i+1</sub>称为a<sub>i</sub>的直接后继,简称为后继。

## 2.2 线性表的特点
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;1.除第一个元素外，其他每个元素有且仅有一个直接前驱。<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;2.除最后一个元素外，其他每个元素有且仅有一个直接后继。

## 2.3 顺序链表
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;最具有代表性的就是数组。

## 2.4 链式链表
### 2.4.1 单链表
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;最基础的链式链表。每一个链表包含一个头指针`head`和一个指示当前节点的指针`current`。对于每一个节点，包含数据段和指针段。指针段用于存储下一个节点的位置。<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;判断结尾的方式是`current`是否为`NULL`。

### 2.4.2 双链表
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;在单链表的基础上增加了一个可以访问上一个节点指针，使得可以左右访问。<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;双链表可以用于寻找“中点”等操作。<br>

### 2.4.3 循环链表
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;1.循环链表是单链表的变形。<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;2.循环链表的最后一个节点的`link`指针不指向`NULL`，而是指向链表的头结点`head`。<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;3.为简化操作，往往回向循环链表中加入表头节点。<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;循环链表的最大特点：`只需要知道某一个节点的位置就可以搜索到其他所有节点的地址`。
<div align=center>
<img src="https://github.com/StrayerSQH/StrayerSQH_Learning/blob/main/%E5%85%B6%E4%BB%96%E6%94%AF%E6%8C%81%E6%96%87%E4%BB%B6/%E6%95%B0%E6%8D%AE%E7%BB%93%E6%9E%84%E4%B8%8E%E7%AE%97%E6%B3%95%E5%88%86%E6%9E%90/%E5%BE%AA%E7%8E%AF%E9%93%BE%E8%A1%A8.jpg" width="500" height="300">
</div>

### 2.4.4 双向循环链表
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;1.前驱和后继方向都能遍历的线性链表。<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;2.通常采用带镖头结点的循环链表形式.
<div align=center>
<img src="https://github.com/StrayerSQH/StrayerSQH_Learning/blob/main/%E5%85%B6%E4%BB%96%E6%94%AF%E6%8C%81%E6%96%87%E4%BB%B6/%E6%95%B0%E6%8D%AE%E7%BB%93%E6%9E%84%E4%B8%8E%E7%AE%97%E6%B3%95%E5%88%86%E6%9E%90/%E5%8F%8C%E5%90%91%E5%BE%AA%E7%8E%AF%E9%93%BE%E8%A1%A8.jpg" width="500" height="300">
</div>
