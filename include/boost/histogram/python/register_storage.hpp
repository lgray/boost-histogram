// Copyright 2018-2019 Henry Schreiner and Hans Dembinski
//
// Distributed under the 3-Clause BSD License.  See accompanying
// file LICENSE or https://github.com/scikit-hep/boost-histogram for details.

#pragma once

#include <boost/histogram/python/pybind11.hpp>

#include <boost/histogram/python/serializion.hpp>
#include <boost/histogram/python/storage.hpp>
#include <pybind11/operators.h>
#include <type_traits>

/// Add helpers common to all storage types
template <class A>
py::class_<A> register_storage(py::module &m, const char *name, const char *desc) {
    using value_type = typename A::value_type;

    py::class_<A> storage(m, name, desc);

    storage.def(py::init<>())
        .def("__getitem__", [](A &self, size_t ind) { return self.at(ind); })
        .def("__setitem__", [](A &self, size_t ind, value_type val) { self.at(ind) = val; })
        .def("push_back", [](A &self, value_type val) { self.push_back(val); })
        .def(py::self == py::self)
        .def(py::self != py::self)
        .def(make_pickle<A>())
        .def("__copy__", [](const A &self) { return A(self); })
        .def("__deepcopy__", [](const A &self, py::object) { return A(self); });

    return storage;
}

/// Add helpers to the unlimited storage type
template <>
py::class_<storage::unlimited> register_storage(py::module &m, const char *name, const char *desc) {
    using A = storage::unlimited; // match code above

    using value_type = A::value_type;

    py::class_<A> storage(m, name, desc);

    storage.def(py::init<>())
        .def("__getitem__", [](A &self, size_t ind) { return self[ind]; })
        .def("__setitem__", [](A &self, size_t ind, value_type val) { self[ind] = val; })
        .def(py::self == py::self)
        .def("__ne__", [](const A &a, const A &b) { return !(a == b); })
        .def(make_pickle<A>())
        .def("__copy__", [](const A &self) { return A(self); })
        .def("__deepcopy__", [](const A &self, py::object) { return A(self); });

    return storage;
}
