#ifndef THROTTLE_H
#define THROTTLE_H

#include <deque>
#include <functional>
#include <queue>
#include <vector>
#include <iostream>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <log4cpp/Category.hh>
#include "Order.h"

namespace Util {
class Throttle {
private:
    friend std::ostream& operator<<(std::ostream&, const Throttle&);
    std::deque<Order> d_ticks;
    unsigned int d_rate;
    boost::posix_time::time_duration d_monitorWindow;
    log4cpp::Category* d_logger;

    Throttle(const Throttle& other);
    Throttle operator=(const Throttle&);

    void expireTicks(const boost::posix_time::ptime& now);

public:
    Throttle(log4cpp::Category* logger=NULL);
    explicit Throttle(const unsigned int& rate,
                      const boost::posix_time::time_duration& window,
                      log4cpp::Category* logger=NULL);
    explicit Throttle(const unsigned int& rate,
                      log4cpp::Category* logger=NULL);

    virtual ~Throttle(void);
    int waitTurn(Order& a);
    void setRate(const unsigned int& rate);
    unsigned int getRate(void) const;
};

} // end of Util

#endif

