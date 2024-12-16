#include<stdio.h>
#include<math.h>
#include<iostream>
#include<string.h>
#include<ctype.h>
#include<errno.h>
#include<float.h>
#include<limits.h>
#include<locale.h>
#include<signal.h>
#include<time.h>
#include<stddef.h>
#include<stdlib.h>
#include<stdarg.h>
#include <vector>
#include <fstream>
#include <string>

using namespace std;

// 函数声明
bool menuBook();
bool menuReader();
bool menuRecord();
void menu();
void addBook();
void showAllBook();
void deleteBook();
void updateBook();
void queryBookForBookName();
void queryBookForAutherName();
void addBorrower();
void showAllBorrower();
void deleteBorrower();
void updateBorrower();
void queryBorrowerForBorrowerId();
void queryBorrowerForBorrowerName();
size_t days(string date, string nowdate);
double pay(string pname, string bId, string nowdate);
string loginIn();
void showBorrowedBook(string s);
void showRecord(string s);
void borrowBook(string pid);
void returnBook(string pid);
void writeToFile();

void upDateFile();
vector<int> dateStringToInt(string date);
int findBookname();
template<typename T>
void save(vector<T>& tmp);


class Book {
private:
	string bId;        // 书籍编号
	string bookName;   // 书名
	string autherName; // 作者名称
	string pUnit;      // 出版单位
	string pTime;      // 出版时间
	string price;      // 价格
	string exFlag;     // 存在状态（1表示存在，0表示不存在）

public:
	Book() {};  // 默认构造函数

	// 友元函数，用于从输入流中读取书籍信息
	friend istream& operator>>(istream& s, Book& tmp) 
	{
		s >> tmp.bId >> tmp.bookName >> tmp.autherName >> tmp.pUnit >> tmp.pTime >> tmp.price >> tmp.exFlag;
		return s;
	}

	// 友元函数，用于将书籍信息写入输出流
	friend ostream& operator<<(ostream& s, Book& tmp) 
	{
		s << tmp.bId << "\t" << tmp.bookName << "\t" << tmp.autherName << "\t" << tmp.pUnit << "\t" << tmp.pTime << "\t" << tmp.price << "\t" << tmp.exFlag;
		return s;
	}

	// 比较操作符，用于比较两本书是否相同，基于书籍编号
	bool operator==(Book& tmp) 
	{
		return bId == tmp.getbId();
	}

	// 赋值操作符，用于将一个书籍对象的属性复制给另一个
	Book& operator=(Book& tmp) 
	{
		bId = tmp.getbId();
		bookName = tmp.getbookName();
		autherName = tmp.getautherName();
		pUnit = tmp.getpUnit();
		pTime = tmp.getpTime();
		price = tmp.getprice();
		exFlag = tmp.getexFlag();
		return *this;
	}

	// 比较操作符，用于比较两本书的编号大小
	bool operator<(Book& tmp) 
	{
		if (bId.compare(tmp.getbId()) == -1) {
			return true;
		}
		else {
			return false;
		}
	}

	// 设置和获取书籍编号
	void setbId(string s) { bId = s; }
	string getbId() { return bId; }

	// 设置和获取书名
	void setbookName(string s) { bookName = s; }
	string getbookName() { return bookName; }

	// 设置和获取作者名
	void setautherName(string s) { autherName = s; }
	string getautherName() { return autherName; }

	// 设置和获取出版单位
	void setpUnit(string s) { pUnit = s; }
	string getpUnit() { return pUnit; }

	// 设置和获取出版时间
	void setpTime(string s) { pTime = s; }
	string getpTime() { return pTime; }

	// 设置和获取价格
	void setprice(string s) { price = s; }
	string getprice() { return price; }

	// 设置和获取存在状态
	void setexFlag(string s) { exFlag = s; }
	string getexFlag() { return exFlag; }
};
class Borrower {
private:
	string pId;         // 借书者的学号
	string pName;       // 借书者的姓名
	string pGender;     // 借书者的性别
	string maxBorrBook; // 借书者目前还可再借的书籍数量

public:
	Borrower() {}; // 默认构造函数

	// 重载输入流操作符以从输入流中读取Borrower对象的数据
	friend istream& operator>>(istream& s, Borrower& tmp) 
	{
		s >> tmp.pId >> tmp.pName >> tmp.pGender >> tmp.maxBorrBook;
		return s;
	}

	// 重载输出流操作符以将Borrower对象的数据写入输出流
	friend ostream& operator<<(ostream& s, Borrower& tmp) 
	{
		s << tmp.pId << "\t" << tmp.pName << "\t" << tmp.pGender << "\t" << tmp.maxBorrBook;
		return s;
	}

	// 重载等于操作符，比较两个Borrower对象是否相等（基于学号）
	bool operator==(Borrower& tmp) 
	{
		return pId == tmp.getpId();
	}

	// 重载赋值操作符，用于将一个Borrower对象的数据赋给另一个
	Borrower& operator=(Borrower& tmp) 
	{
		pId = tmp.getpId();
		pName = tmp.getpName();
		pGender = tmp.getpGender();
		maxBorrBook = tmp.getmaxBorrBook();
		return *this;
	}

	// 重载小于操作符，比较两个Borrower对象的学号大小
	bool operator<(Borrower& tmp) 
	{
		return pId.compare(tmp.getpId()) == -1;
	}

	// 设置和获取学号
	void setpId(string s) { pId = s; }
	string getpId() { return pId; }

	// 设置和获取姓名
	void setpName(string s) { pName = s; }
	string getpName() { return pName; }

	// 设置和获取性别
	void setpGender(string s) { pGender = s; }
	string getpGender() { return pGender; }

	// 设置和获取目前还可再借书数
	void setmaxBorrBook(string s) { maxBorrBook = s; }
	string getmaxBorrBook() { return maxBorrBook; }
};

class record {
private:
	string pId;        // 借书人的标识（例如学号或用户ID）
	string bId;        // 借出的书籍的标识（例如书籍编号）
	string date;       // 借书日期
	string timeLimit;  // 借书期限

public:
	record() {}; // 默认构造函数，创建一个空的记录对象

	// 重载输入流操作符以从输入流（如cin）中读取record对象的数据
	friend istream& operator>>(istream& s, record& tmp) 
	{
		s >> tmp.pId >> tmp.bId >> tmp.date >> tmp.timeLimit;
		return s;
	}

	// 重载输出流操作符以将record对象的数据写入输出流（如cout）
	friend ostream& operator<<(ostream& s, record& tmp) 
	{
		s << tmp.pId << "\t" << tmp.bId << "\t" << tmp.date << "\t" << tmp.timeLimit;
		return s;
	}

	// 重载等于操作符，比较两个record对象是否相等（基于书籍标识bId）
	bool operator==(record& tmp) 
	{
		return bId == tmp.getbId();
	}

	// 重载赋值操作符，用于将一个record对象的数据赋给另一个
	record& operator=(record& tmp) 
	{
		pId = tmp.getbId();        // 这里应为tmp.getPId()，当前写法可能是一个错误
		bId = tmp.getpId();        // 同上，应为tmp.getBId()
		date = tmp.getdate();
		timeLimit = tmp.gettimeLimit();
		return *this;
	}

	// 重载小于操作符，比较两个record对象的书籍标识bId的字典序大小
	bool operator<(record& tmp) 
	{
		if (bId.compare(tmp.getbId()) == -1) 
		{
			return true;
		}
		else {
			return false;
		}
	}

	// 设置和获取借书期限
	void settimeLimit(string s) { timeLimit = s; }
	string gettimeLimit() { return timeLimit; }

	// 设置和获取借书人标识
	void setpId(string s) { pId = s; }
	string getpId() { return pId; }

	// 设置和获取书籍标识
	void setbId(string s) { bId = s; }
	string getbId() { return bId; }

	// 设置和获取借书日期
	void setdate(string s) { date = s; }
	string getdate() { return date; }
};

bool readFromFile(vector<Borrower>&);
bool readFromFile(vector<Book>&);
bool readFromFile(vector<record>&);
void deleteFromFile(record);
void deleteFromFile(Book);
void deleteFromFile(Borrower);
void upDateFile(record);
void upDateFile(Book);
void upDateFile(Borrower);
int findBookname(vector<Book>& tmp1, vector<Book>& tmp2, string bname);
int findAutherName(vector<Book>& tmp1, vector<Book>& tmp2, string aname);
int findper(vector<Borrower>& tmp1, vector<Borrower>& tmp2, string name);
int findper(vector<Borrower>&, string);
int findBid(vector<Book>& tmp1, Book& bk, string id);
void writeToFile(record tmp);


// 定义图书管理菜单函数
bool menuBook() 
{
	system("cls");  // 清屏

	int flag;  // 用户选择的菜单选项
	cout << "欢迎进入图书管理" << endl;  // 显示欢迎信息
	cout << "===========================" << endl;  // 菜单分隔线
	// 显示各菜单项
	cout << "新进图书添加:1" << endl;
	cout << "图书信息浏览:2" << endl;
	cout << "图书删除:3" << endl;
	cout << "图书信息修改:4" << endl;
	cout << "按书名查询:5" << endl;
	cout << "按作者名查询:6" << endl;
	cout << "退出图书管理:0" << endl;
	cout << "===========================" << endl;

	// 循环读取用户的输入直到合法
	do {
		cout << "请选择:(0~6):";
		cin >> flag;
	} while (flag < 0 || flag>6);

	// 根据用户输入执行相应操作
	switch (flag) {
	case 1:addBook(); return true; break;
	case 2:showAllBook(); return true; break;
	case 3:deleteBook(); return true; break;
	case 4:updateBook(); return true; break;
	case 5:queryBookForBookName(); return true; break;
	case 6:queryBookForAutherName(); return true; break;
	case 0:system("cls"); return false; break;  // 退出图书管理菜单
	default:break;
	}

	system("cls");  // 清屏
}

// 定义读者管理菜单函数
bool menuReader() 
{
	system("cls");  // 清屏

	int flag;  // 用户选择的菜单选项
	// 显示欢迎信息及菜单选项
	cout << "欢迎进入读者管理" << endl;
	cout << "===========================" << endl;
	cout << "注册用户:1" << endl;
	cout << "用户信息浏览:2" << endl;
	cout << "借书人信息的注销:3" << endl;
	cout << "借书人信息修改:4" << endl;
	cout << "按学号查询:5" << endl;
	cout << "按姓名查询:6" << endl;
	cout << "退出读者管理:0" << endl;
	cout << "===========================" << endl;

	// 循环读取用户的输入直到合法
	do {
		cout << "请选择:(0~6):";
		cin >> flag;
	} while (flag < 0 || flag>6);

	// 根据用户输入执行相应操作
	switch (flag) {
	case 1:addBorrower(); return true; break;
	case 2:showAllBorrower(); return true; break;
	case 3:deleteBorrower(); return true; break;
	case 4:updateBorrower(); return true; break;
	case 5:queryBorrowerForBorrowerId(); return true; break;
	case 6:queryBorrowerForBorrowerName(); return true; break;
	case 0:system("cls"); return false; break;  // 退出读者管理菜单
	default:break;
	}
}

// 定义借阅关系管理菜单函数
string loginusername;  // 当前登录用户的用户名
bool menuRecord() 
{
	system("cls");  // 清屏

	int flag;  // 用户选择的菜单选项
	// 显示欢迎信息及菜单选项
	cout << "欢迎进入借阅信息管理" << endl;
	cout << "===========================" << endl;
	cout << "读者用户登录(先登录才能进行其他操作):1" << endl;
	cout << "办理借书手续:2" << endl;
	cout << "办理还书手续:3" << endl;
	cout << "查询当前登录会员目前在借书籍:4" << endl;
	cout << "查询图书目前状态(按书名查询):5" << endl;
	cout << "查询图书目前状态(按作者名查):6" << endl;
	cout << "展示自己所有借阅关系:7" << endl;
	cout << "退出借阅关系管理:0" << endl;
	cout << "===========================" << endl;

	// 循环读取用户的输入直到合法
	do {
		cout << "请选择:(0~7):";
		cin >> flag;
	} while (flag < 0 || flag>7);

	// 根据用户输入执行相应操作
	switch (flag) {
	case 1:loginusername = loginIn(); return true; break;
	case 2:borrowBook(loginusername); return true; break;
	case 3:returnBook(loginusername); return true; break;
	case 4:showBorrowedBook(loginusername); return true; break;
	case 5:queryBookForBookName(); return true; break;
	case 6:queryBookForAutherName(); return true; break;
	case 7:showRecord(loginusername); return true; break;
	case 0:system("cls"); return false; break;  // 退出借阅关系管理菜单
	default:break;
	}
}

// 定义总菜单函数
void menu() 
{
	system("cls");  // 清屏

	int flag;  // 用户选择的菜单选项
	// 显示欢迎信息及菜单选项
	cout << "欢迎使用图书管理系统" << endl;
	cout << "===========================" << endl;
	cout << "进入图书管理:1" << endl;
	cout << "进入读者管理:2" << endl;
	cout << "进入借阅关系管理:3" << endl;
	cout << "退出程序:0" << endl;
	cout << "===========================" << endl;

	// 循环读取用户的输入直到合法
	do {
		cout << "请选择:(0~3):";
		cin >> flag;
	} while (flag < 0 || flag>3);

	// 根据用户输入执行相应操作
	switch (flag) 
	{
	case 1:
		// 循环显示图书管理菜单，直到用户选择退出
		while (true)
		{
			system("cls");
			bool flag = true;
			flag = menuBook();
			if (flag == false) {
				break;
			}
		}
		break;
	case 2:
		// 循环显示读者管理菜单，直到用户选择退出
		while (true)
		{
			system("cls");
			bool flag = true;
			flag = menuReader();
			if (flag == false) {
				break;
			}
		}
		break;
	case 3:
		// 循环显示借阅关系管理菜单，直到用户选择退出
		while (true)
		{
			system("cls");
			bool flag = true;
			flag = menuRecord();
			if (flag == false) {
				break;
			}
		}
		break;
	case 0:exit(0); break;  // 退出程序
	default:break;
	}
}

//添加图书
void addBook() 
{
	string me;  // 用于临时存储用户输入的字符串
	vector<Book> tmp;  // 存储Book对象的向量
	// 循环添加书籍信息
	while (true) 
	{
		Book tmpb;  // 创建一个新的Book对象
		// 以下是连续获取用户输入并设置Book对象的各项属性
		cout << "输入图书编号:";
		cin >> me;
		tmpb.setbId(me);  // 设置书籍编号
		me.clear();
		// 重复此过程以获取图书名、作者、出版社等信息
		cout << "输入图书名字:";
		cin >> me;
		tmpb.setbookName(me);  // 设置书名
		me.clear();
		cout << "输入图书作者:";
		cin >> me;
		tmpb.setautherName(me);
		me.clear();
		cout << "输入图书出版社:";
		cin >> me;
		tmpb.setpUnit(me);
		me.clear();
		cout << "输入图书出版时间:";
		cin >> me;
		tmpb.setpTime(me);
		me.clear();
		cout << "输入图书价格:";
		cin >> me;
		tmpb.setprice(me);
		me.clear();
		cout << "输入图书存在状态:";
		cin >> me;
		tmpb.setexFlag(me);
		me.clear();

		tmp.push_back(tmpb);  // 将填充好的Book对象添加到向量中
		save(tmp);  // 将图书信息保存到文件或数据库
		cout << "添加成功,是否继续添加:(Y/N)";
		cin >> me;
		// 根据用户输入判断是否继续添加图书
		if (me.compare("N") == 0 || me.compare("n") == 0) {
			return;  // 退出添加图书流程
		}
	}
}

//展示所有图书
void showAllBook() 
{
	vector<record> tmp;  // 存储Book对象的向量
	readFromFile(tmp);  // 从文件或数据库读取图书信息并填充到向量中
	// 输出表头
	cout << "编号\t" << "书名\t" << "作者名\t" << "出版单位\t" << "出版时间\t" << "价格\t" << "存在状态\t" << endl;
	// 遍历并打印所有图书信息
	for (size_t i = 0; i < tmp.size(); i++) {
		cout << tmp[i];  // 输出Book对象的信息，重载了ostream操作符
		cout << endl;
	}
	system("pause");  // 暂停，等待用户操作
	return;
}

//删除图书
void deleteBook() 
{
	Book tmp;  // 创建一个Book对象
	string s;  // 存储用户输入的字符串
	string reflag;  // 存储用户的继续或退出选择
	// 循环删除书籍
	while (true) {
		cout << "输入你想删除的书的编号:";
		cin >> s;
		tmp.setbId(s);  // 设置要删除的书籍编号
		deleteFromFile(tmp);  // 从文件或数据库中删除该书籍信息
		cout << "您还想继续删除吗？(Y/N):";
		cin >> reflag;
		// 根据用户输入判断是否继续删除图书
		if (reflag.compare("N") == 0 || reflag.compare("n") == 0) {
			return;  // 退出删除图书流程
		}
	}
}

//更新图书 
void updateBook() 
{
	Book tmp;  // 创建一个Book对象
	string s;  // 存储用户输入的图书编号
	string reflag;  // 存储用户的继续或退出选择
	// 循环更新书籍信息
	while (true) {
		cout << "输入你想修改的图书信息的图书编号:";
		cin >> s;
		tmp.setbId(s);  // 设置要修改的书籍编号
		vector<Book> tmpv;  // 存储Book对象的向量
		readFromFile(tmpv);  // 从文件或数据库中读取所有图书信息
		// 查找并获取要修改的图书信息
		for (size_t i = 0; i < tmpv.size(); i++) {
			if (tmpv[i].getbId().compare(tmp.getbId()) == 0) {
				tmp = tmpv[i];
				break;
			}
		}
		// 显示修改选项
		cout << "修改书名:1\t" << "    " << "修改作者名:2" << endl;
		cout << "修改出版单位:3\t" << "    " << "修改出版时间:4" << endl;
		cout << "修改价格:5\t" << "    " << "修改存在状态:6" << endl;
		cout << "退出修改:0" << endl;
		int flag;  // 用户选择的修改项
		bool brflag = false;  // 标记是否退出修改流程
		string me;  // 存储用户输入的修改信息
		// 循环获取用户选择并执行相应的修改
		while (!brflag) {
			cout << "输入你想修改的信息选项:";
			cin >> flag;
			if (flag == 0) {
				break;  // 用户选择退出修改
			}
			cout << "输入你想修改的信息:";
			cin >> me;
			// 根据用户选择修改对应的图书信息
			switch (flag) {
			case 1:tmp.setbookName(me); break;
			case 2:tmp.setautherName(me); break;
			case 3:tmp.setpUnit(me); break;
			case 4:tmp.setpTime(me); break;
			case 5:tmp.setprice(me); break;
			case 6:tmp.setexFlag(me); break;
			default:break;
			}
		}
		upDateFile(tmp);  // 更新文件或数据库中的图书信息
		cout << "修改成功,您还想继续修改其他书籍信息吗？(Y/N):";
		cin >> reflag;
		// 根据用户输入判断是否继续修改图书信息
		if (reflag.compare("N") == 0 || reflag.compare("n") == 0) {
			return;  // 退出更新图书流程
		}
	}
}

//按书名查询
void queryBookForBookName() 
{
	vector<Book> tmp;  // 存储Book对象的向量
	readFromFile(tmp);  // 从文件或数据库中读取所有图书信息
	string reflag;  // 存储用户的继续或退出选择
	int pflag;  // 标记查询结果状态
	string s;  // 存储用户输入的查询书名
	// 循环查询书籍
	while (true) {
		pflag = false;
		cout << "你想查询的书名:";
		cin >> s;
		vector<Book> book;  // 存储查询到的Book对象的向量
		pflag = findBookname(tmp, book, s);  // 执行查询操作
		// 根据查询结果显示信息
		if (pflag != -1) {
			for (size_t i = 0; i < book.size(); i++) {
				cout << book[i];  // 显示查询到的图书信息
				cout << endl;
			}
		}
		else {
			cout << "不好意思,没有您要的书" << endl;  // 未找到对应书籍
		}
		cout << "您想继续查询吗？(Y/N):";
		cin >> reflag;
		// 根据用户输入判断是否继续查询图书
		if (reflag.compare("N") == 0 || reflag.compare("n") == 0) {
			return;  // 退出按书名查询流程
		}
	}
}

//按作者名查询
void queryBookForAutherName() 
{
	vector<Book> tmp;  // 存储Book对象的向量
	readFromFile(tmp);  // 从文件或数据库中读取所有图书信息
	string reflag;  // 存储用户的继续或退出选择
	int pflag;  // 标记查询结果状态
	string s;  // 存储用户输入的查询作者名
	// 循环查询书籍
	while (true) {
		pflag = false;
		cout << "你想查询的作者名:";
		cin >> s;
		vector<Book> autherBook;  // 存储查询到的Book对象的向量
		pflag = findAutherName(tmp, autherBook, s);  // 执行查询操作
		// 根据查询结果显示信息
		if (pflag != -1) {
			for (size_t i = 0; i < autherBook.size(); i++) {
				cout << autherBook[i]; // 显示查询到的图书信息
				cout << endl;
			}
		}
		else {
			cout << "不好意思，没有您要的书" << endl; // 未找到对应作者的书籍
		}
		cout << "您想继续查询吗？(Y/N):";
		cin >> reflag;
		// 根据用户输入判断是否继续查询图书
		if (reflag.compare("N") == 0 || reflag.compare("n") == 0) {
			return; // 退出按作者名查询流程
		}
	}
}

//读者关系管理 
//添加读者 
void addBorrower() 
{
	string me;  // 用于临时存储用户输入
	vector<Borrower> tmp;  // 存储Borrower对象的向量
	// 循环添加读者信息
	while (true) {
		Borrower tmpb;  // 创建一个新的Borrower对象
		// 以下是连续获取用户输入并设置Borrower对象的各项属性
		cout << "输入读者学号:";
		cin >> me;
		tmpb.setpId(me);  // 设置读者学号
		// 重复此过程以获取读者名字、性别、可借书数等信息

		tmp.push_back(tmpb);  // 将填充好的Borrower对象添加到向量中
		save(tmp);  // 将读者信息保存到文件或数据库（具体实现未给出）
		cout << "添加成功,是否继续注册:(Y/N)";
		cin >> me;
		// 根据用户输入判断是否继续添加读者
		if (me.compare("N") == 0 || me.compare("n") == 0) {
			return;  // 退出添加读者流程
		}
		else {
			system("cls");  // 清屏，准备下一次添加
		}
	}
}

//展示所有读者 
void showAllBorrower() 
{
	vector<Borrower> tmp;  // 存储Borrower对象的向量
	readFromFile(tmp);  // 从文件或数据库读取读者信息并填充到向量中
	// 输出表头
	cout << "学号\t" << "姓名\t" << "性别\t" << "目前可借书数\t" << endl;
	// 遍历并打印所有读者信息
	for (size_t i = 0; i < tmp.size(); i++) {
		cout << tmp[i];  // 输出Borrower对象的信息，假设已重载ostream操作符
		cout << endl;
	}
	system("pause");  // 暂停，等待用户操作
}

//删除读者
void deleteBorrower() 
{
	Borrower tmp;  // 创建一个Borrower对象
	string s;  // 存储用户输入的读者学号
	string reflag;  // 存储用户的继续或退出选择
	// 循环删除读者
	while (true) {
		cout << "输入你想注销学生的编号:";
		cin >> s;
		tmp.setpId(s);  // 设置要删除的读者学号
		deleteFromFile(tmp);  // 从文件或数据库中删除该读者信息
		cout << "您还想继续删除吗？(Y/N):";
		cin >> reflag;
		// 根据用户输入判断是否继续删除读者
		if (reflag.compare("N") == 0 || reflag.compare("n") == 0) {
			return;  // 退出删除读者流程
		}
	}
}

//更新读者 
void updateBorrower() 
{
	Borrower tmp;  // 创建一个Borrower对象
	string s;  // 存储用户输入的读者学号
	string reflag;  // 存储用户的继续或退出选择
	// 循环更新读者信息
	while (true) {
		cout << "输入你想学生信息的学生学号:";
		cin >> s;
		tmp.setpId(s);  // 设置要修改的读者学号
		vector<Borrower> tmpv;  // 存储Borrower对象的向量
		readFromFile(tmpv);  // 从文件或数据库中读取所有读者信息
		// 查找并获取要修改的读者信息
		for (size_t i = 0; i < tmpv.size(); i++) {
			if (tmpv[i].getpId().compare(tmp.getpId()) == 0) {
				tmp = tmpv[i];
				break;
			}
		}
		// 显示修改选项
		cout << "修改学生姓名:1\t" << "    " << "修改性别:2" << endl;
		cout << "修改目前可借书数:3\t" << "    " << "退出修改:0" << endl;
		int flag;  // 用户选择的修改项
		bool brflag = false;  // 标记是否退出修改流程
		string me;  // 存储用户输入的修改信息
		// 循环获取用户选择并执行相应的修改
		while (!brflag) {
			cout << "输入你想修改的信息选项:";
			cin >> flag;
			if (flag == 0) {
				break;  // 用户选择退出修改
			}
			cout << "输入你想修改的信息:";
			cin >> me;
			// 根据用户选择修改对应的读者信息
			switch (flag) {
			case 1:tmp.setpName(me); break;
			case 2:tmp.setpGender(me); break;
			case 3:tmp.setmaxBorrBook(me); break;
			default:break;
			}
		}
		upDateFile(tmp);  // 更新文件或数据库中的读者信息
		cout << "修改成功,您还想继续修改其他学生信息吗？(Y/N):";
		cin >> reflag;
		// 根据用户输入判断是否继续修改读者信息
		if (reflag.compare("N") == 0 || reflag.compare("n") == 0) {
			return;  // 退出更新读者流程
		}
	}
}

//按学号查询
void queryBorrowerForBorrowerId() 
{
	vector<Borrower> tmp;  // 存储Borrower对象的向量
	readFromFile(tmp);  // 从文件或数据库中读取所有读者信息
	string reflag;  // 存储用户的继续或退出选择
	string s;  // 存储用户输入的查询学号
	// 循环查询读者
	while (true) {
		cout << "你想查询的读者学号:";
		cin >> s;
		size_t index = -1;
		// 执行查询操作
		index = findper(tmp, s);
		// 根据查询结果显示信息
		if (index != -1) {
			cout << tmp[index];  // 显示查询到的读者信息
			cout << endl;
		}
		else {
			cout << "不好意思,该读者未注册" << endl;  // 未找到对应学号的读者
		}
		cout << "您想继续查询吗？(Y/N):";
		cin >> reflag;
		// 根据用户输入判断是否继续查询读者
		if (reflag.compare("N") == 0 || reflag.compare("n") == 0) {
			return;  // 退出按学号查询流程
		}
	}
}

//按姓名查询
void queryBorrowerForBorrowerName() 
{
	vector<Borrower> tmp;  // 存储Borrower对象的向量
	readFromFile(tmp);  // 从文件或数据库中读取所有读者信息
	string reflag;  // 存储用户的继续或退出选择
	int pflag;  // 标记查询结果状态
	string s;  // 存储用户输入的查询姓名
	// 循环查询读者
	while (true) {
		pflag = false;
		cout << "你想查询的读者名字:";
		cin >> s;
		vector<Borrower> reader;  // 存储查询到的Borrower对象的向量

		pflag = findper(tmp, reader, s);  // 执行查询操作，假设findper函数根据姓名查找读者，并填充到reader向量中
		if (pflag != -1) {  // 判断是否找到对应的读者
			for (size_t i = 0; i < reader.size(); i++) 
			{
				cout << reader[i];  // 显示查询到的读者信息，假设Borrower对象已重载ostream操作符以便输出
				cout << endl;
			}
		}
		else {
			cout << "不好意思，您所找的读者未注册" << endl;  // 未找到对应姓名的读者
		}
		cout << "您想继续查询吗？(Y/N):";
		cin >> reflag;
		if (reflag.compare("N") == 0 || reflag.compare("n") == 0) 
		{
			return;  // 用户选择不再继续查询，退出查询流程
		}
		else {
			system("cls");
		}
	}
}

//借阅关系管理
//日期字符串转换为整数 
vector<int> dateStringToInt(string date) 
{
	vector<int> nyn;  // 用于存储年月日的向量
	size_t comma1 = 0;  // 第一个点的位置
	size_t comma2 = 0;  // 第二个点的位置

	// 查找第一个点并获取年份
	comma1 = date.find('.', 0);
	nyn.push_back(atoi(date.substr(0, comma1).c_str()));

	// 查找第二个点并获取月份
	comma2 = date.find('.', comma1 + 1);
	nyn.push_back(atoi(date.substr(comma1 + 1, comma2 - comma1 - 1).c_str()));

	// 获取日期
	comma1 = comma2;
	nyn.push_back(atoi(date.substr(comma1 + 1, date.length() - comma1 - 1).c_str()));

	return nyn;  // 返回年月日组成的向量
}

//计算两个日期间的天数
size_t days(string date, string nowdate) 
{
	vector<int> nyn1;
	nyn1 = dateStringToInt(date);  // 将第一个日期字符串转换为年月日
	vector<int> nyn2;
	nyn2 = dateStringToInt(nowdate);  // 将第二个日期字符串转换为年月日

	int yue[13] = { 0,31,28,31,30,31,30,31,31,30,31,30,31 };  // 每个月的天数
	// 检查第一个日期所在年是否为闰年
	if (nyn1[0] % 4 == 0 && nyn1[0] % 100 != 0 || nyn1[0] % 400 == 0) yue[2] = 29;
	else yue[2] = 28;

	size_t day, i, day1, ans;
	// 计算第一个日期距离零年的总天数
	day = nyn1[0] * 365 + (nyn1[0] - 1) / 4 - (nyn1[0] - 1) / 100 + (nyn1[0] - 1) / 400 + nyn1[2];
	for (i = 0; i < nyn1[1]; i++) 
	{
		day += yue[i];  // 加上月份的天数
	}
	// 对第二个日期做相同的计算
	if (nyn2[0] % 4 == 0 && nyn2[0] % 100 != 0 || nyn2[0] % 400 == 0) yue[2] = 29;
	else yue[2] = 28;
	day1 = nyn2[0] * 365 + (nyn2[0] - 1) / 4 - (nyn2[0] - 1) / 100 + (nyn2[0] - 1) / 400 + nyn2[2];
	for (i = 0; i < nyn2[1]; i++) 
	{
		day1 += yue[i];
	}
	ans = day1 - day;  // 计算两个日期之间的天数差

	return ans;  // 返回天数差
}

//读取特定读者的借阅记录
vector<record> readSelfRecord(string s) 
{
	vector<record> tmp;
	readFromFile(tmp);  // 从文件或数据库中读取所有借阅记录

	vector<record> tmppart;  // 用于存储特定读者的借阅记录
	for (size_t i = 0; i < tmp.size(); i++) 
	{
		if (tmp[i].getpId().compare(s) == 0) 
		{  // 检查每条记录的读者ID是否与指定的ID匹配
			tmppart.push_back(tmp[i]);  // 如果匹配，将该记录添加到tmppart向量中
		}
	}
	return tmppart;  // 返回包含特定读者借阅记录的向量
}

//计算逾期费用
double pay(string pname, string bId, string nowdate) 
{
	vector<record> tmp;
	tmp = readSelfRecord(pname);  // 读取特定读者的借阅记录
	record re;

	// 在读者的借阅记录中查找特定书籍的记录
	for (size_t i = 0; i < tmp.size(); i++) 
	{
		if (tmp[i].getbId().compare(bId) == 0) 
		{  // 检查书籍ID是否匹配
			re = tmp[i];  // 如果匹配，保存该借阅记录
			break;
		}
	}
	string date = re.getdate();  // 获取借书日期

	size_t day;
	day = days(date, nowdate);  // 计算从借书日期到当前日期的天数

	vector<Book> tmpb;
	readFromFile(tmpb);  // 读取所有书籍信息

	Book bk;
	findBid(tmpb, bk, bId);  // 查找特定ID的书籍
	double price = atof(bk.getprice().c_str());  // 获取书籍价格

	// 如果逾期，则计算逾期费用；否则返回0
	return day > (size_t)atoi(re.gettimeLimit().c_str()) ? (day - atoi(re.gettimeLimit().c_str())) * price : 0;
}

//用户登录
string loginIn() 
{
	vector<Borrower> tmp;
	readFromFile(tmp);  // 从文件或数据库中读取所有读者信息
	string me;
	string flag = "-1";  // 初始化标志为未找到
	Borrower tmpb;
	cout << "输入读者用户名(学号):";
	cin >> me;
	tmpb.setpId(me);  // 设置要查找的读者学号

	// 在所有读者信息中查找是否有匹配的学号
	for (size_t i = 0; i < tmp.size(); i++) 
	{
		if (tmp[i].getpId().compare(tmpb.getpId()) == 0) 
		{
			flag = me;  // 如果找到，设置标志为找到的学号
			break;
		}
	}
	if (flag.compare("-1") == 0) 
	{
		cout << "该读者未注册会员" << endl;  // 未找到匹配的读者
	}
	else 
	{
		cout << "登陆成功" << endl;  // 登录成功
	}
	system("pause");
	return flag;  // 返回登录结果
}

//获取已借阅的书籍
vector<Book> BorrowedBook(string s) 
{
	vector<record> tmp;
	tmp = readSelfRecord(s);  // 读取特定读者的借阅记录

	vector<Book> booktmp;
	readFromFile(booktmp);  // 从文件或数据库中读取所有书籍信息

	vector<Book> tmpb;  // 用于存储已借阅的书籍信息
	for (size_t i = 0; i < tmp.size(); i++) 
	{
		Book bk;
		findBid(booktmp, bk, tmp[i].getbId());  // 查找并获取特定书籍编号的书籍信息

		tmpb.push_back(bk);  // 将找到的书籍添加到已借阅书籍的列表中
	}
	return tmpb;  // 返回已借阅的书籍列表
}

//展示已借阅的书籍 
void showBorrowedBook(string s) 
{
	vector<Book> tmp;
	tmp = BorrowedBook(s);  // 获取特定读者已借阅的书籍列表

	// 打印表头
	cout << "编号\t" << "书名\t" << "作者名\t" << "出版单位\t" << "出版时间\t" << "价格\t" << "存在状态\t" << endl;

	// 遍历并打印每本已借阅的书籍信息
	for (size_t i = 0; i < tmp.size(); i++) 
	{
		cout << tmp[i];  // 假设Book类已重载ostream操作符以便输出
		cout << endl;
	}
	system("pause");  // 暂停，等待用户操作
}

//展示特定读者的借阅记录
void showRecord(string s) 
{
	vector<record> tmp;
	tmp = readSelfRecord(s);  // 读取特定读者的借阅记录

	// 打印表头
	cout << "读者编号\t" << "书籍编号\t" << "借书日期\t" << "借阅时限\t" << endl;
	// 遍历并打印每条借阅记录
	for (size_t i = 0; i < tmp.size(); i++) 
	{
		cout << tmp[i];  // 假设record类已重载ostream操作符以便输出
		cout << endl;
	}
	system("pause");  // 暂停，等待用户操作
}

//借书操作
void borrowBook(string pid) 
{
	vector<record> tmp;
	tmp = readSelfRecord(pid);  // 读取特定读者的借阅记录
	bool flag = true;
	string bid;
	string nowdate;

	// 检查读者是否还有借书次数
	vector<Borrower> borrtmp;
	readFromFile(borrtmp);  // 读取所有读者信息
	int index = findper(borrtmp, pid);  // 查找特定读者
	if (atoi(borrtmp[index].getmaxBorrBook().c_str()) <= 0) 
	{
		cout << "您的借书次数已用尽,请先还书！" << endl;
		system("pause");
		return;
	}

	cout << "输入当前时间:";
	cin >> nowdate;

	// 循环处理借书请求
	while (true) 
	{
		vector<Book> tmpb;
		readFromFile(tmpb);  // 读取所有书籍信息
		cout << "输入你想借的书的编号：";
		cin >> bid;

		Book bk;
		int index = findBid(tmpb, bk, bid);  // 查找并获取特定书籍编号的书籍信息
		for (size_t i = 0; i < tmp.size(); i++) 
		{
			if (pay(pid, tmp[i].getbId(), nowdate) > 0) 
			{
				flag = false;
				break;
			}
		}
		if (index == -1 || tmpb[index].getexFlag().compare("0") == 0) 
		{
			cout << "仓库中没有这本书" << endl;
			cout << "是否借其他书:(Y/N):";
		}
		else if (!flag) {
			cout << "您还有书逾期未还,请还完书再来借" << endl;
			return;
		}
		else {
			// 执行借书操作...

			cout << "借书成功,是否继续借书:(Y/N):";
		}
		cin >> bid;
		if (bid.compare("N") == 0 || bid.compare("n") == 0) {
			return;
		}
		else {
			bk.setexFlag("0");
			upDateFile(bk);


			/*修改学生可借书数*/
			Borrower tmper;
			bool fb = false;
			vector<Borrower> tmpv;
			readFromFile(tmpv);
			for (size_t i = 0; i < tmpv.size(); i++) {
				if (tmpv[i].getpId().compare(pid) == 0) {
					tmper = tmpv[i];
					fb = true;
					break;
				}
			}
			if (fb) {
				int num = atoi(tmper.getmaxBorrBook().c_str()) - 1;
				string me = to_string(num);
				tmper.setmaxBorrBook(me);
				upDateFile(tmper);
			}
			/*修改学生可借书数*/

			/*添加一个借阅关系*/
			record re;
			re.setpId(pid);
			re.setbId(bid);
			re.setdate(nowdate);
			re.settimeLimit("30");
			writeToFile(re);
			/*添加一个借阅关系*/
			bid.clear();

			cout << "借书成功,是否继续借书:(Y/N):";
		}
		cin >> bid;
		if (bid.compare("N") == 0 || bid.compare("n") == 0) 
		{
			return;
		}
		else 
		{
			bid.clear();
			system("cls");

		}
	}
}

//还书操作 
void returnBook(string pid) 
{
	string bid;

	// 循环处理还书请求
	while (true) 
	{
		vector<Book> tmpb;
		readFromFile(tmpb);  // 读取所有书籍信息
		cout << "输入你想还的书的编号：";
		cin >> bid;

		Book bk;
		findBid(tmpb, bk, bid);  // 查找并获取特定书籍编号的书籍信息
		bk.setexFlag("1");  // 更新书籍的存在状态为可借

		string nowdate;
		cout << "输入当前时间:";
		cin >> nowdate;

		double money = pay(pid, bid, nowdate);  // 计算逾期费用
		if (money > 0) 
		{
			cout << "您需要支付" << money << "元超期费用" << endl;
		}
		upDateFile(bk);  // 更新书籍信息

		/*修改学生可借书数*/
		Borrower tmper;
		bool fb = false;
		vector<Borrower> tmpv;
		readFromFile(tmpv);
		for (size_t i = 0; i < tmpv.size(); i++) 
		{
			if (tmpv[i].getpId().compare(pid) == 0) 
			{
				tmper = tmpv[i];
				fb = true;
				break;
			}
		}
		if (fb) 
		{
			int num = atoi(tmper.getmaxBorrBook().c_str()) + 1;
			string me = to_string(num);
			tmper.setmaxBorrBook(me);
			upDateFile(tmper);
		}

		/*修改学生可借书数*/

		/*添加一个借阅关系*/
		record re;
		re.setpId(pid);
		re.setbId(bid);
		re.setdate(nowdate);
		re.settimeLimit("30");
		deleteFromFile(re);
		/*添加一个借阅关系*/

		bid.clear();
		cout << "还书成功,是否继续还书:(Y/N)";
		cin >> bid;
		if (bid.compare("N") == 0 || bid.compare("n") == 0) 
		{
			return;
		}
		else {
			system("cls");
		}
	}
}

//一些工具 
//写入文件操作 writeToFile()
//对于Book类型
void writeToFile(Book tmp) 
{
	ofstream ofs;
	ofs.open("book.csv", ios::out | ios::app);  // 打开book.csv文件进行追加写入

	// 将Book对象的信息以逗号分隔的形式写入文件
	ofs << tmp.getbId() << "," << tmp.getbookName() << "," << tmp.getautherName() << "," << tmp.getpUnit() << ","
		<< tmp.getpTime() << "," << tmp.getprice() << "," << tmp.getexFlag() << ",";
	ofs << endl;  // 写入换行符

	ofs.close();  // 关闭文件
}

//对于Borrower类型
void writeToFile(Borrower tmp) 
{
	ofstream ofs;
	ofs.open("borrower.csv", ios::out | ios::app);  // 打开borrower.csv文件进行追加写入

	// 将Borrower对象的信息以逗号分隔的形式写入文件
	ofs << tmp.getpId() << "," << tmp.getpName() << "," << tmp.getpGender() << "," << tmp.getmaxBorrBook() << ",";
	ofs << endl;  // 写入换行符

	ofs.close();  // 关闭文件
}

//对于record类型
void writeToFile(record tmp) 
{
	ofstream ofs;
	ofs.open("record.csv", ios::out | ios::app);  // 打开record.csv文件进行追加写入

	// 将record对象的信息以逗号分隔的形式写入文件
	ofs << tmp.getpId() << "," << tmp.getbId() << "," << tmp.getdate() << "," << tmp.gettimeLimit() << ",";
	ofs << endl;  // 写入换行符

	ofs.close();  // 关闭文件
}

//从文件中读取内容 readFromFile()
//对于 Book 类型 
bool readFromFile(vector<Book>& tmp) 
{
	ifstream inf("book.csv", ios::in);  // 打开book.csv文件进行读取
	if (!inf.is_open()) 
	{
		return false;  // 如果文件打开失败，返回false
	}
	else 
	{
		const int cnt = 6;    // 一行中有7个字段，因此使用6个逗号来分割
		int j = 0;  // 用于追踪当前字段的索引
		size_t comma1 = 0;  // 用于存储当前找到的逗号位置
		size_t comma2 = 0;  // 用于存储下一个逗号位置
		string s1;  // 用于存储从文件中读取的每一行数据
		Book btmp;  // 用于暂时存储解析的书籍信息

		// 逐行读取文件内容
		while (getline(inf, s1)) 
		{
			// 寻找第一个逗号并获取书籍编号
			comma1 = s1.find(',', 0);
			btmp.setbId(s1.substr(0, comma1));

			// 循环处理剩余字段
			while (comma1 < s1.size() && j != cnt - 1) 
			{
				comma2 = s1.find(',', comma1 + 1);
				// 根据字段索引j解析并设置Book对象的其他属性
				if (j == 0) 
				{
					btmp.setbookName(s1.substr(comma1 + 1, comma2 - comma1 - 1));
				}
				else if (j == 1) 
				{
					btmp.setautherName(s1.substr(comma1 + 1, comma2 - comma1 - 1));
				}
				else if (j == 2) 
				{
					btmp.setpUnit(s1.substr(comma1 + 1, comma2 - comma1 - 1));
				}
				else if (j == 3) 
				{
					btmp.setpTime(s1.substr(comma1 + 1, comma2 - comma1 - 1));
				}
				else if (j == 4) 
				{
					btmp.setprice(s1.substr(comma1 + 1, comma2 - comma1 - 1));
					// 价格后是存在状态，因此需要再次更新逗号位置
					comma1 = comma2;
					comma2 = s1.find(',', comma1 + 1);
					btmp.setexFlag(s1.substr(comma1 + 1, comma2 - comma1 - 1));
				}
				++j;  // 更新字段索引
				comma1 = comma2;  // 更新逗号位置
			}
			j = 0;  // 重置字段索引
			tmp.push_back(btmp);  // 将解析好的Book对象添加到向量中
		}
		inf.close();  // 关闭文件
		return true;  // 返回成功标志
	}
}

//对于borrower类型
bool readFromFile(vector<Borrower>& tmp) 
{
	ifstream inf("borrower.csv", ios::in);  // 打开borrower.csv文件进行读取
	if (!inf.is_open()) 
	{
		return false;  // 如果文件打开失败，返回false
	}
	else 
	{
		const int cnt = 3;  // 一行中有4个字段，因此使用3个逗号来分割
		int j = 0;  // 用于追踪当前字段的索引
		size_t comma1 = 0;  // 用于存储当前找到的逗号位置
		size_t comma2 = 0;  // 用于存储下一个逗号位置
		string s1;  // 用于存储从文件中读取的每一行数据
		Borrower btmp;  // 用于暂时存储解析的读者信息

		// 逐行读取文件内容
		while (getline(inf, s1)) 
		{
			// 寻找第一个逗号并获取读者学号
			comma1 = s1.find(',', 0);
			btmp.setpId(s1.substr(0, comma1));

			// 循环处理剩余字段
			while (comma1 < s1.size() && j != cnt - 1) 
			{
				comma2 = s1.find(',', comma1 + 1);
				// 根据字段索引j解析并设置Borrower对象的其他属性
				if (j == 0) 
				{
					btmp.setpName(s1.substr(comma1 + 1, comma2 - comma1 - 1));
				}
				else if (j == 1) 
				{
					btmp.setpGender(s1.substr(comma1 + 1, comma2 - comma1 - 1));
					// 性别后是最大可借书数，因此需要再次更新逗号位置
					comma1 = comma2;
					comma2 = s1.find(',', comma1 + 1);
					btmp.setmaxBorrBook(s1.substr(comma1 + 1, comma2 - comma1 - 1));
				}
				++j;  // 更新字段索引
				comma1 = comma2;  // 更新逗号位置
			}
			j = 0;  // 重置字段索引
			tmp.push_back(btmp);  // 将解析好的Borrower对象添加到向量中
		}
		inf.close();  // 关闭文件
		return true;  // 返回成功标志
	}
}

//对于record类型
bool readFromFile(vector<record>& tmp) 
{
	ifstream inf("record.csv", ios::in);  // 打开record.csv文件进行读取
	if (!inf.is_open()) 
	{
		return false;  // 如果文件打开失败，返回false
	}
	else 
	{
		const int cnt = 3;  // 一行中有4个字段，因此使用3个逗号来分割
		int j = 0;  // 用于追踪当前字段的索引
		size_t comma1 = 0;  // 用于存储当前找到的逗号位置
		size_t comma2 = 0;  // 用于存储下一个逗号位置
		string s1;  // 用于存储从文件中读取的每一行数据
		record rtmp;  // 用于暂时存储解析的借阅记录

		// 逐行读取文件内容
		while (getline(inf, s1)) 
		{
			// 寻找第一个逗号并获取读者学号
			comma1 = s1.find(',', 0);
			rtmp.setpId(s1.substr(0, comma1));

			// 循环处理剩余字段
			while (comma1 < s1.size() && j != cnt - 1) 
			{
				comma2 = s1.find(',', comma1 + 1);
				// 根据字段索引j解析并设置record对象的其他属性
				if (j == 0) 
				{
					rtmp.setbId(s1.substr(comma1 + 1, comma2 - comma1 - 1));
				}
				else if (j == 1) 
				{
					rtmp.setdate(s1.substr(comma1 + 1, comma2 - comma1 - 1));
					// 日期后是借阅时限，因此需要再次更新逗号位置
					comma1 = comma2;
					comma2 = s1.find(',', comma1 + 1);
					rtmp.settimeLimit(s1.substr(comma1 + 1, comma2 - comma1 - 1));
				}
				++j;  // 更新字段索引
				comma1 = comma2;  // 更新逗号位置
			}
			j = 0;  // 重置字段索引
			tmp.push_back(rtmp);  // 将解析好的record对象添加到向量中
		}
		inf.close();  // 关闭文件
		return true;  // 返回成功标志
	}
}

//删除文件内容
//对于Book类型
void deleteFromFile(Book tmp) 
{
	ofstream ofs;
	vector<Book> tmp1;
	readFromFile(tmp1);  // 读取所有Book对象
	ofs.open("book.csv", ios::trunc);  // 打开book.csv文件，并使用ios::trunc模式清空文件内容
	ofs.close();  // 关闭文件

	// 遍历所有书籍，除了需要删除的书籍外，将其他书籍重新写入文件
	for (size_t i = 0; i < tmp1.size(); i++) 
	{
		if (tmp1[i].getbId().compare(tmp.getbId()) != 0) 
		{  // 检查书籍ID是否与要删除的书籍ID不同
			writeToFile(tmp1[i]);  // 将不需要删除的书籍重新写入文件
		}
	}
}

//对于Borrower类型
void deleteFromFile(Borrower tmp) 
{
	ofstream ofs;
	vector<Borrower> tmp1;
	readFromFile(tmp1);  // 读取所有Borrower对象
	ofs.open("borrower.csv", ios::trunc);  // 打开borrower.csv文件，并使用ios::trunc模式清空文件内容
	ofs.close();  // 关闭文件

	// 遍历所有读者，除了需要删除的读者外，将其他读者重新写入文件
	for (size_t i = 0; i < tmp1.size(); i++) 
	{
		if (tmp1[i].getpId().compare(tmp.getpId()) != 0) 
		{  // 检查读者ID是否与要删除的读者ID不同
			writeToFile(tmp1[i]);  // 将不需要删除的读者重新写入文件
		}
	}
}

//对于record类型
void deleteFromFile(record tmp) 
{
	ofstream ofs;
	vector<record> tmp1;
	readFromFile(tmp1);  // 读取所有record对象
	ofs.open("record.csv", ios::trunc);  // 打开record.csv文件，并使用ios::trunc模式清空文件内容
	ofs.close();  // 关闭文件

	// 遍历所有借阅记录，除了需要删除的记录外，将其他记录重新写入文件
	for (size_t i = 0; i < tmp1.size(); i++) 
	{
		if (tmp1[i].getbId().compare(tmp.getbId()) != 0) 
		{  // 检查记录的书籍ID是否与要删除的记录的书籍ID不同
			writeToFile(tmp1[i]);  // 将不需要删除的记录重新写入文件
		}
	}
}

//更新文件内容 upDateFile
//对于Book类型 
void upDateFile(Book tmp) 
{
	ofstream ofs;

	vector<Book> tmp1;
	readFromFile(tmp1);  // 读取所有Book对象
	ofs.open("book.csv", ios::trunc);  // 打开book.csv文件，并使用ios::trunc模式清空文件内容
	ofs.close();  // 关闭文件

	// 遍历所有书籍，更新需要更新的书籍信息
	for (size_t i = 0; i < tmp1.size(); i++) 
	{
		if (tmp1[i].getbId().compare(tmp.getbId()) == 0) 
		{  // 检查书籍ID是否与要更新的书籍ID相同
			tmp1[i] = tmp;  // 更新书籍信息
			break;
		}
	}

	// 将更新后的所有书籍重新写入文件
	for (size_t i = 0; i < tmp1.size(); i++) 
	{
		writeToFile(tmp1[i]);
	}
}

//对于Borrower类型
void upDateFile(Borrower tmp) 
{
	ofstream ofs;

	vector<Borrower> tmp1;
	readFromFile(tmp1);  // 读取所有Borrower对象
	ofs.open("borrower.csv", ios::trunc);  // 打开borrower.csv文件，并使用ios::trunc模式清空文件内容
	ofs.close();  // 关闭文件

	// 遍历所有读者，更新需要更新的读者信息
	for (size_t i = 0; i < tmp1.size(); i++) 
	{
		if (tmp1[i].getpId().compare(tmp.getpId()) == 0) 
		{  // 检查读者ID是否与要更新的读者ID相同
			tmp1[i] = tmp;  // 更新读者信息
			break;
		}
	}

	// 将更新后的所有读者重新写入文件
	for (size_t i = 0; i < tmp1.size(); i++) 
	{
		writeToFile(tmp1[i]);
	}
}

//对于record类型
void upDateFile(record tmp) 
{
	ofstream ofs;

	vector<record> tmp1;
	readFromFile(tmp1);  // 读取所有record对象
	ofs.open("record.csv", ios::trunc);  // 打开record.csv文件，并使用ios::trunc模式清空文件内容
	ofs.close();  // 关闭文件

	// 遍历所有借阅记录，更新需要更新的记录信息
	for (size_t i = 0; i < tmp1.size(); i++) 
	{
		if (tmp1[i].getbId().compare(tmp.getbId()) == 0) 
		{  // 检查记录的书籍ID是否与要更新的记录的书籍ID相同
			tmp1[i] = tmp;  // 更新记录信息
			break;
		}
	}

	// 将更新后的所有借阅记录重新写入文件
	for (size_t i = 0; i < tmp1.size(); i++) 
	{
		writeToFile(tmp1[i]);
	}
}

//找书 
//查找书名 findBookname()
int findBookname(vector<Book>& tmp1, vector<Book>& tmp2, string bname) 
{
	int pflag = -1;  // 初始化标志为未找到(-1)
	for (size_t i = 0; i < tmp1.size(); i++) 
	{  // 遍历所有书籍
		if (tmp1[i].getbookName().compare(bname) == 0) 
		{  // 如果书名与给定书名相匹配
			pflag = 1;  // 设置标志为找到(1)
			tmp2.push_back(tmp1[i]);  // 将找到的书籍添加到结果向量中
		}
	}
	return pflag;  // 返回查找结果标志
}

//查找作者名 findAutherName()
int findAutherName(vector<Book>& tmp1, vector<Book>& tmp2, string aname) 
{
	int pflag = -1;  // 初始化标志为未找到(-1)
	for (size_t i = 0; i < tmp1.size(); i++) 
	{  // 遍历所有书籍
		if (tmp1[i].getautherName().compare(aname) == 0) 
		{  // 如果作者名与给定作者名相匹配
			pflag = 1;  // 设置标志为找到(1)
			tmp2.push_back(tmp1[i]);  // 将找到的书籍添加到结果向量中
		}
	}
	return pflag;  // 返回查找结果标志
}

//查找特定ID的书籍 findBid()
int findBid(vector<Book>& tmp1, Book& bk, string id) 
{
	int pflag = -1;  // 初始化标志为未找到(-1)
	for (size_t i = 0; i < tmp1.size(); i++) 
	{  // 遍历所有书籍
		if (tmp1[i].getbId().compare(id) == 0) 
		{  // 如果书籍ID与给定ID相匹配
			bk = tmp1[i];  // 将找到的书籍赋值给bk
			pflag = i;  // 设置标志为找到的索引
			break;  // 结束循环
		}
	}
	return pflag;  // 返回查找结果标志
}

//查找特定ID的读者 findper()（按ID）
int findper(vector<Borrower>& tmp, string id) 
{
	int pflag = -1;  // 初始化标志为未找到(-1)
	for (size_t i = 0; i < tmp.size(); i++) 
	{  // 遍历所有读者
		if (tmp[i].getpId().compare(id) == 0) 
		{  // 如果读者ID与给定ID相匹配
			pflag = i;  // 设置标志为找到的索引
			break;  // 结束循环
		}
	}
	return pflag;  // 返回查找结果标志
}

//查找特定姓名的读者 findper()（按姓名）
int findper(vector<Borrower>& tmp1, vector<Borrower>& tmp2, string name) 
{
	int pflag = -1;  // 初始化标志为未找到(-1)
	for (size_t i = 0; i < tmp1.size(); i++) 
	{  // 遍历所有读者
		if (tmp1[i].getpName().compare(name) == 0) 
		{  // 如果读者姓名与给定姓名相匹配
			pflag = 1;  // 设置标志为找到(1)
			tmp2.push_back(tmp1[i]);  // 将找到的读者添加到结果向量中
		}
	}
	return pflag;  // 返回查找结果标志
}

//保存数据 save()
template<typename T>
void save(vector<T>& tmp) 
{
	for (size_t i = 0; i < tmp.size(); i++) 
	{  // 遍历给定的对象向量
		writeToFile(tmp[i]);  // 将每个对象写入对应的文件
	}
}

//主函数 
int main() 
{
	while (true) 
	{
		menu();
	}
	system("pause");
	return 0;
}


