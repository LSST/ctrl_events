// -*- lsst-c++ -*-
/** \class EventSystem
  *
  * \ingroup events
  *
  * \brief Coordinate publishing and receiving events
  *
  * \author Stephen Pietrowicz, NCSA
  */

#ifndef LSST_EVENTS_EVENTSYSTEM_H_
#define LSST_EVENTS_EVENTSYSTEM_H_

#include <stdlib.h>
#include <iostream>
#include <vector>
#include <list>
#include <boost/shared_ptr.hpp>

#include "lsst/pex/policy/Policy.h"
#include "lsst/pex/logging/Component.h"
#include "lsst/utils/Utils.h"
#include "lsst/daf/base/PropertySet.h"
#include "lsst/pex/logging/LogRecord.h"
#include "lsst/ctrl/events/Events.h"

namespace dafBase = lsst::daf::base;
namespace pexPolicy = lsst::pex::policy;

using namespace std;

namespace lsst {
namespace ctrl {
namespace events {
class EventSystem {
public:
    EventSystem();

    ~EventSystem();

    static const EventSystem& getDefaultEventSystem();

    void createTransmitter(const std::string& hostName, 
                           const std::string& topicName);

    void createTransmitter(const pexPolicy::Policy& policy);

    void createLocalTransmitter(const std::string& topicName);

    void createReceiver(const std::string& hostName, 
                        const std::string& topicName);

    void createReceiver(const pexPolicy::Policy& policy);

    void createLocalReceiver(const std::string& topicName);

    void publish(const std::string& topicName, const dafBase::PropertySet::Ptr psp);

    void publish(const std::string& topicName, const pexLogging::LogRecord& rec);

    dafBase::PropertySet::Ptr receive(const std::string& topicName);

    dafBase::PropertySet::Ptr receive(const std::string& topicName,
                                  const long timeout);


    // TODO: all these need to be eliminated once the SWIG incantation for templates is figured out
    dafBase::PropertySet::Ptr matchingReceive(const std::string& topicName, const std::string& name, const int value);
    dafBase::PropertySet::Ptr matchingReceive(const std::string& topicName, const std::string& name, const long value);
    dafBase::PropertySet::Ptr matchingReceive(const std::string& topicName, const std::string& name, const float value);
    dafBase::PropertySet::Ptr matchingReceive(const std::string& topicName, const std::string& name, const double value);
    dafBase::PropertySet::Ptr matchingReceive(const std::string& topicName, const std::string& name, const long long value);
    dafBase::PropertySet::Ptr matchingReceive(const std::string& topicName, const std::string& name, const std::string& value);

    dafBase::PropertySet::Ptr matchingReceive(const std::string& topicName, const std::string& name, const int value, long timeout);
    dafBase::PropertySet::Ptr matchingReceive(const std::string& topicName, const std::string& name, const long value, long timeout);
    dafBase::PropertySet::Ptr matchingReceive(const std::string& topicName, const std::string& name, const float value, long timeout);
    dafBase::PropertySet::Ptr matchingReceive(const std::string& topicName, const std::string& name, const double value, long timeout);
    dafBase::PropertySet::Ptr matchingReceive(const std::string& topicName, const std::string& name, const long long value, long timeout);
    dafBase::PropertySet::Ptr matchingReceive(const std::string& topicName, const std::string& name, const std::string& value, long timeout);

private:
    template <typename T>
    dafBase::PropertySet::Ptr _matchingReceive(const std::string& topicName,
                                          const std::string& name,
                                          const T& value);

    template <typename T>
    dafBase::PropertySet::Ptr _matchingReceive(const std::string& topicName,
                                          const std::string& name,
                                          const T& value,
                                          long timeout);
    boost::shared_ptr<EventTransmitter> getTransmitter(const std::string& name);
    boost::shared_ptr<EventReceiver> getReceiver(const std::string& name);

protected:
    static EventSystem *defaultEventSystem;
    list<boost::shared_ptr<EventTransmitter> >_transmitters;
    list<boost::shared_ptr<EventReceiver> >_receivers;
};
}
}
}

#endif /*LSST_EVENTS_EVENTSYSTEM_H_*/

