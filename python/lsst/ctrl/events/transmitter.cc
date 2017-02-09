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
#include "lsst/ctrl/events/Transmitter.h"

namespace py = pybind11;
using namespace pybind11::literals;

namespace lsst {
namespace ctrl {
namespace events {

PYBIND11_PLUGIN(_transmitter) {
    py::module::import("lsst.ctrl.events._event");

    py::module mod("_transmitter", "Python wrapper for _transmitter library");

    py::class_<Transmitter, std::shared_ptr<Transmitter>> cls(mod, "Transmitter");

    // do not wrap the constructor because class is pure virtual

    cls.def("publishEvent", &Transmitter::publishEvent);
    // do not wrap getDestinationPropertyName because it is pure virtual
    cls.def("getDestinationName", &Transmitter::getDestinationName);

    return mod.ptr();
}

}}} // lsst::ctrl::events
