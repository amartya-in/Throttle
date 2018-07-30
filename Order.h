#ifndef ORDER_H
#define ORDER_H
#include <iostream>
#include <functional>
#include <queue>
#include <boost/date_time/posix_time/posix_time.hpp>
using namespace boost;

class Order
{	
   public:
 	Order(){  }
	char id_[12];
	char OrderType;
        posix_time::ptime timeStamp;
};
class NewOrder : public Order
{
    public:
	NewOrder(){OrderType='N';}
      	char side_;
  	char symbol_[10];
  	int price_;
  	int size_;
};
class AmendOrder : public Order
{
	public: 
	AmendOrder(){ OrderType='A'; }
int price_;
  int size_;
}; 
class CancelOrder : public Order
{
	public:
	CancelOrder(){ OrderType='C';}

};

#endif

