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
#include <vector>

#include "pybind11/pybind11.h"
#include "pybind11/stl.h"

#include "lsst/daf/base/PropertySet.h"
#include "lsst/ctrl/events/Event.h"

namespace py = pybind11;
using namespace pybind11::literals;

namespace lsst {
namespace ctrl {
namespace events {

PYBIND11_PLUGIN(_event) {
    py::module mod("_event", "Python wrapper for Event");

    py::class_<Event, std::shared_ptr<Event>> cls(mod, "Event");

    cls.attr("TYPE") = py::cast(Event::TYPE);
    cls.attr("EVENTTIME") = py::cast(Event::EVENTTIME);
    cls.attr("RUNID") = py::cast(Event::RUNID);
    cls.attr("STATUS") = py::cast(Event::STATUS);
    cls.attr("TOPIC") = py::cast(Event::TOPIC);
    cls.attr("QUEUE") = py::cast(Event::QUEUE);
    cls.attr("PUBTIME") = py::cast(Event::PUBTIME);
    cls.attr("UNINITIALIZED") = py::cast(Event::UNINITIALIZED);

    cls.def(py::init<>());
    cls.def(py::init<PropertySet const &>(), "properties"_a);
    cls.def(py::init<PropertySet const &, PropertySet const &>(), "properties"_a, "filterable"_a);
    cls.def(py::init<std::string const &, PropertySet const &>(), "runid"_a, "properties"_a);
    cls.def(py::init<std::string const &, PropertySet const &, PropertySet const &>(), "runid"_a,
            "properties"_a, "filterable"_a);
    // do not wrap the constructor that takes an activemqcpp cms::TextMessage
    // and activemqcpp is not wrapped

    cls.def("getPropertySet", &Event::getPropertySet);
    cls.def("getPubDate", &Event::getPubDate);
    cls.def("getPubTime", &Event::getPubTime);
    cls.def("setPubTime", &Event::setPubTime);
    cls.def("getEventTime", &Event::getEventTime);
    cls.def("setEventTime", &Event::setEventTime);
    cls.def("updateEventTime", &Event::updateEventTime);
    cls.def("getEventDate", &Event::getEventDate);
    cls.def("getRunId", &Event::getRunId);
    cls.def("setRunId", &Event::setRunId);
    cls.def("getType", &Event::getType);
    cls.def("getStatus", &Event::getStatus);
    cls.def("setStatus", &Event::setStatus);
    cls.def("getTopic", &Event::getTopic);
    cls.def("setTopic", &Event::setTopic);
    cls.def("getFilterablePropertyNames", &Event::getFilterablePropertyNames);
    cls.def("getCustomPropertyNames", &Event::getCustomPropertyNames);
    cls.def("getCustomPropertySet", &Event::getCustomPropertySet);
    // do not wrap `populateHeader` or `marshall` because both take an activemqcpp cms::TextMessage
    // and activemqcpp is not wrapped

    return mod.ptr();
}

}}} // lsst::ctrl::events
