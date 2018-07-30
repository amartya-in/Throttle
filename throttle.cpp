#include "throttle.h"
#include <unistd.h>
#include <boost/thread/thread.hpp>
#include <string.h>
using namespace std;
bool operator<(const Order& a, const  Order& b)
{
        if((a.OrderType != b.OrderType) && (a.OrderType == 'C') )
                return false;
        else
                return true;
}
namespace Util {
using namespace boost;

std::ostream& operator<<(std::ostream& os, const Throttle& obj)
{
    os << "Events in queue: [" << obj.d_ticks.size() << "]. "
       << "Monitor window: [" << obj.d_monitorWindow << "]. "
       << "Rate of events: [" << obj.d_rate << "].";
    return os;
}

Throttle::Throttle(log4cpp::Category* logger)
: d_rate(1)
, d_logger(logger)
, d_monitorWindow(posix_time::seconds(1))
{ }

Throttle::Throttle(const unsigned int& rate,
                   log4cpp::Category* logger)
: d_rate(rate)
, d_monitorWindow(posix_time::seconds(1))
, d_logger(logger)
{
    if(!d_rate) {
        std::string msg("The rate of the Throttle cannot be zero. Re-setting to 1.");
        if(d_logger) { d_logger->info(msg); }
        else { std::cerr << msg << std::endl; }
    }
}

Throttle::Throttle(const unsigned int& rate,
                   const boost::posix_time::time_duration& window,
                   log4cpp::Category* logger)
: d_rate(rate)
, d_monitorWindow(window)
, d_logger(logger)
{ }


Throttle::~Throttle(void) { }

void Throttle::setRate(const unsigned int& rate)
{
    if(!rate) {
        std::string msg("The rate of the Throttle cannot be zero.");
        if(d_logger) { d_logger->info(msg); }
        else { std::cerr << msg << std::endl; }
    } else {
        d_rate = rate;
    }
}

unsigned int Throttle::getRate(void) const
{
    return d_rate;
}

void Throttle::expireTicks(const boost::posix_time::ptime& now)
{
    while(d_ticks.size()) {
        if((now - d_ticks.front().timeStamp) >= d_monitorWindow) {
            if(d_logger) {
                d_logger->debugStream() << "Expiring tick: ["
                                        << d_ticks.front().timeStamp << "]"
                                        << log4cpp::eol;
            }
            d_ticks.pop_front();
        } else {
            break;
        }
    }
}

int Throttle::waitTurn(Order& a)
{
    posix_time::ptime now(posix_time::microsec_clock::universal_time());
    this->expireTicks(now);
    a.timeStamp=now;
    d_ticks.push_back(a);

    if(d_ticks.size() <= d_rate) {
        return 0;
    }
    const posix_time::ptime& last= d_ticks.front().timeStamp;
    this_thread::sleep(now-last);

    if(d_logger) {
        d_logger->infoStream() << "Waited ["
                               << (now - last) << "]"
                               << ",\t elements: [" << d_ticks.size() << "]"
                               << log4cpp::eol;
    }

    return (now - last).total_milliseconds();
}
} // end of Util
