#include<iostream>
#include<cmath>
#include<string>
#include<cstdlib> //exit函数，该函数的原型声明在cstdlib头文件中 
using namespace std;
//日期类
class Date 
{ 
	private:
		int year; //年 
		int month; //月 
		int day; //日 
		int totalDays; //该日期是从公元元年1月1日开始的第几天 
	public: 
		Date(int year, int month, int day);
		int getYear() const { return year; }
		int getMonth() const { return month; }
		int getDay() const { return day; }
		int getMaxDay() const;
		bool isLeapYear() const{
			return(year%4==0&&year%100!=0)||(year%400==0); 
		}
		void show() const;
		int distance(const Date& date) const{
			return totalDays - date.totalDays; 
		}
};
//日期类相关成员函数实现
namespace{ //namespace使下面的定义只在当前文件中有效
	//储存平年中某个月1日之前有多少天，为便于getMaxDay函数的实现，该数组多出一项 
	const int DAYS_BEFORE_MONTH[] = { 0,31,59,90,120,151,181,212,243,273,304,334,365 };
}
Date::Date(int year, int month, int day):year(year), month(month), day(day){ 
	if ((day <= 0)|| (day > getMaxDay())){ 
		cout << "Invalid date:"; show(); cout << endl; exit(1);
	}
	int years = year - 1; 
	totalDays = years * 365 + years / 4 - years / 100 + years / 400 + DAYS_BEFORE_MONTH[month - 1] + day; 
	if (isLeapYear() && month > 2){
		totalDays++;
	}	 
} 
int Date::getMaxDay() const{ 
	if (isLeapYear() && month == 2){
		return 29;
	}else{
		return DAYS_BEFORE_MONTH[month] - DAYS_BEFORE_MONTH[month - 1];
	} 
} 
void Date::show()const{ 
	cout << getYear() << "-" << getMonth() << "-" << getDay();
}
//储蓄账户类 
class SavingsAccount
{
	private:
		string id; //账号 
		double balance; //余额 
		double rate; //存款的年利率 
		Date lastDate; //上次变更余额的时期 
		double accumulation; //余额按日累加之和
		static double total; //所有账户的总金额 
		
		//记录一笔账，date为日期，amount为金额，desc为说明
		void record(const Date& date, double amount,const string &desc);
		void error(const string& msg) const;
		//获得指定日期为止的存款金额按日累加
		double accumulate(const Date& date) const { 
			return accumulation + balance * date.distance(lastDate); 
		}
	public:
		SavingsAccount(Date & date, string& id1, double rate); 
		const string& getId() { return id; } 
		const double getBalance() { return balance; } 
		const double getRate() { return rate; }
		static double getTotal() { return total; }
		//存入现金
		void deposit(const Date& date, double amount,const string&desc);
		//取出现金
		void withdraw(const Date& date, double amount,const string&desc); 
		//结算利息，每年一月一日调用一次该函数
		void settle(const Date& date); 
		//显示账户信息
		const void show(); 
};

double SavingsAccount::total = 0;

//SavingsAccount类相关成员函数实现
SavingsAccount::SavingsAccount(Date &date, string &id1, double rate)
	:id(id1), balance(0), rate(rate), lastDate(date), accumulation(0){ 
		date.show(); cout << "\t#"<< id << "is created" << endl; 
}

void SavingsAccount::record(const Date& date, double amount,const string& desc){ 
	accumulation = accumulate(date); 
	lastDate = date; 
	amount = floor(amount * 100 + 0.5) / 100; //保留小数点后两位
	balance += amount; 
	total += amount; 
	date.show(); 
	cout << "\t#" << id << "\t" << amount << "\t" << balance<<"\t"<<desc << endl; 
}
 
void SavingsAccount::error(const string& msg)const{ 
	cout << "Error(#" << id << "):" << msg << endl; 
} 

void SavingsAccount::deposit(const Date& date, double amount, const string&desc){ 
	record(date, amount,desc); 
}

void SavingsAccount::withdraw(const Date& date, double amount,const string&desc){ 
	record(date, -amount,desc); 
}

void SavingsAccount::settle(const Date& date){ 
	double interest = accumulate(date) * rate / date.distance(Date(date.getYear()-1,1,1)); 
	if (interest != 0){
		record(date, interest,"interest");
	} 
	accumulation = 0; 
}

const void SavingsAccount::show(){ 
	cout << "#" << id << "\tBalance: " << balance; 
}

int main()
{
	Date date(2008, 11, 1); //起始日期
	//建立几个账户
	string a = "S3755217"; 
	string b = "02342342"; 
	SavingsAccount accounts[] = { 
		SavingsAccount(date,a,0.015), SavingsAccount(date,b,0.015) 
	};
	
	const int n = sizeof(accounts) / sizeof(SavingsAccount);
	//几笔账户
	accounts[0].deposit(Date(2008, 11, 5), 5000, "salary"); 
	accounts[1].deposit(Date(2008, 11, 25), 10000, "sell stock 0323"); 
	accounts[0].deposit(Date(2008, 12, 5), 5500, "salary"); 
	accounts[1].withdraw(Date(2008, 12, 20), 4000, "buy a laptop"); 
	
	//开户后第90天到了银行的计息日， 结算所有帐户的年息
	cout << endl; for (int i = 0; i < n; i++){ 
		accounts[i].settle(Date(2009, 1, 1)); 
		accounts[i].show(); cout << endl; 
	}
	//输出各个帐户信息
	cout << "Total: " << SavingsAccount::getTotal() << endl;
	
	return 0;
}
