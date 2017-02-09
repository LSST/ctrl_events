/*
 * LSST Data Management System
 *
 * This product includes software developed by the
 * LSST Project (http://www.lsst.org/).
 * See the COPYRIGHT file
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the LSST License Statement and
 * the GNU General Public License along with this program.  If not,
 * see <https://www.lsstcorp.org/LegalNotices/>.
 */
#include <memory>

#include "pybind11/pybind11.h"

#include "lsst/ctrl/events/Event.h"
#include "lsst/ctrl/events/EventBroker.h"
#include "lsst/ctrl/events/EventTransmitter.h"

namespace py = pybind11;
using namespace pybind11::literals;

namespace lsst {
namespace ctrl {
namespace events {

PYBIND11_PLUGIN(_eventTransmitter) {
    py::module::import("lsst.ctrl.events._transmitter");

    py::module mod("_eventTransmitter", "Python wrapper for EventTransmitter");

    py::class_<EventTransmitter, std::shared_ptr<EventTransmitter>, Transmitter> cls(mod, "EventTransmitter");

    cls.def(py::init<const std::string &, const std::string &, int>(),
            "hostName"_a, "destinationName"_a, "hostPort"_a=EventBroker::DEFAULTHOSTPORT);

    cls.def("getTopicName", &EventTransmitter::getTopicName);
    cls.def("getDestinationPropertyName", &EventTransmitter::getDestinationPropertyName);

    return mod.ptr();
}

}}} // lsst::ctrl::events
