#include "account.h"
#include <iostream>
#include <cmath>

using namespace std;
double SavingsAccount::total = 0;

//SavingsAccount类相关成员函数的实现

SavingsAccount::SavingsAccount(const Date & date, const std::string & id, double rate): lastDate(date), id(id), rate(rate), accumulation(0){
	date.show();
	cout << "\t#" << id << "created" << endl;
}

//记录单笔账单
void SavingsAccount::record(const Date & date, double amount, const std::string &desc){
	accumulation = accumulate(date);
	lastDate = date;
	amount = floor(amount * 100 + 0.5) / 100;
	balance += amount;
	date.show();
	cout << "\t#" <<id << "\t" << amount << "\t" << balance << "\t" << desc << endl;

}

void SavingsAccount::error(const string &msg) const {
	cout << "Error(#" << id << "):" << msg <<endl;
}

void SavingsAccount::deposit(const Date & date, double amount, const std::string &desc){
	record(date, amount, desc);
}

//取出现金
void SavingsAccount::withdraw(const Date & date, double amount, const std::string &desc){
	if (amount > getBalance()){
		error ("not enough money");
	else
		record(date, -amount, desc);
	}
}

//结算利息，每年一月一日调用一次该函数
void SavingsAccount::settle(const Date & date){
	double interest = accumulate(date) * rate / date.distance(Date(date.getYear() - 1, 1, 1));
	if(interest != 0)
		record(date, interest, "interest");
	acculation = 0;
}
//显示账户信息
void SavingsAccount::show() const{
	cout << id << "\tBalance:" << balance;
}

	
