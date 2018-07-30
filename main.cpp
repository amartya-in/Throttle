/* Main Program */

#include <iostream>
#include <unistd.h>
#include <log4cpp/Category.hh>
#include <log4cpp/FileAppender.hh>
#include <log4cpp/BasicLayout.hh>
#include <log4cpp/PatternLayout.hh>
#include <boost/date_time/posix_time/posix_time.hpp>

#include "Order.h"
#include "throttle.h"
using namespace std;


log4cpp::Category& createLogger(void);
void lightLoadTest(log4cpp::Category& logger);
void fakeLoad(const int a, const int b);

int main(int argc, char** argv)
{
    log4cpp::Category& logger = createLogger();
    lightLoadTest(logger);
    log4cpp::Category::shutdown();
    return EXIT_SUCCESS;
}


log4cpp::Category& createLogger(void)
{
    /*Setting up Appender, layout and Category*/
    log4cpp::Appender *appender = new log4cpp::FileAppender("FileAppender", "./throttle.log");
    log4cpp::Layout *myLayout = new log4cpp::PatternLayout();
    ((log4cpp::PatternLayout*) myLayout)->setConversionPattern("%d %-5p [[%c]] - %m%n");
    log4cpp::Category& category = log4cpp::Category::getInstance("Throttler");

    appender->setLayout(myLayout);
    category.setAppender(appender);
    category.setPriority(log4cpp::Priority::INFO);

    return category;
}

void lightLoadTest(log4cpp::Category& logger)
{
    using namespace boost;

    Util::Throttle myThrottler(20, &logger);
    logger.infoStream() << myThrottler << log4cpp::eol;

    logger.info("Starting lightLoadTest...");
    clock_t start = clock();
    posix_time::ptime pStart(posix_time::microsec_clock::universal_time());

    for(int i=0; i < 1000; ++i) { NewOrder myMsg ; myThrottler.waitTurn(myMsg); fakeLoad(1000, 100 ); }
    for(int i=0; i <1000; ++i) { CancelOrder myMsg ; myThrottler.waitTurn(myMsg); fakeLoad(1000, 100); }
    clock_t stop = clock();
    posix_time::ptime pStop(posix_time::microsec_clock::universal_time());
    logger.infoStream() << "Msg: [60]. Clock cycles: ["
                        << (stop - start) << "]"
                        << ". Duration: [" << (pStop - pStart) << "]"
                        << log4cpp::eol;
}

void fakeLoad(const int a, const int b)
{
    double r = 0;
    for ( int i = 0 ; i < a ; i++ ) {
        for ( int j = 0 ; j < b ; j++ ) {
            r = r + i * j;
            
        }
    }
}

