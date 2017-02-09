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

#include "lsst/daf/base/PropertySet.h"
#include "lsst/ctrl/events/StatusEvent.h"
#include "lsst/ctrl/events/Event.h"

namespace py = pybind11;
using namespace pybind11::literals;

namespace lsst {
namespace ctrl {
namespace events {

PYBIND11_PLUGIN(_statusEvent) {
    py::module::import("lsst.ctrl.events._event");

    py::module mod("_statusEvent", "Python wrapper for StatusEvent");

    py::class_<StatusEvent, std::shared_ptr<StatusEvent>, Event> cls(mod, "StatusEvent");

    cls.attr("ORIG_HOSTNAME") = py::cast(StatusEvent::ORIG_HOSTNAME);
    cls.attr("ORIG_PROCESSID") = py::cast(StatusEvent::ORIG_PROCESSID);
    cls.attr("ORIG_LOCALID") = py::cast(StatusEvent::ORIG_LOCALID);

    cls.def(py::init<>());
    cls.def(py::init<LocationId const &, PropertySet const &>(), "originator"_a, "properties"_a);
    cls.def(py::init<LocationId const &, PropertySet const &, PropertySet const &>(), "originator"_a,
            "properties"_a, "filterable"_a);
    cls.def(py::init<std::string const &, LocationId const &, PropertySet const &>(), "runid"_a,
            "originator"_a, "properties"_a);
    cls.def(py::init<std::string const &, LocationId const &, PropertySet const &, PropertySet const &>(),
            "runid"_a, "originator"_a, "properties"_a, "filterable"_a);
    // do not wrap the constructor that takes an activemqcpp cms::TextMessage
    // and activemqcpp is not wrapped

    cls.def("getOriginator", &StatusEvent::getOriginator);
    // do not wrap `populateHeader` because it takes an activemqcpp cms::TextMessage
    // and activemqcpp is not wrapped

    return mod.ptr();
}

}  // events
}  // ctrl
}  // lsst
