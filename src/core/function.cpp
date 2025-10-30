#include "../../include/dlplan/core.h"

#include <sstream>


namespace dlplan::core {

Function::Function(FunctionIndex index, const std::string& name, int arity, bool is_static)
    : Base<Function>(index), m_name(name), m_arity(arity), m_is_static(is_static) { }

Function::Function(const Function& other) = default;

Function& Function::operator=(const Function& other) = default;

Function::Function(Function&& other) = default;

Function& Function::operator=(Function&& other) = default;

Function::~Function() = default;

bool Function::are_equal_impl(const Function& other) const {
    // remove index comparison when we use factory.
    return (get_index() == other.get_index()) && (get_name() == other.get_name()) && (get_arity() == other.get_arity()) && (is_static() == other.is_static());
}

void Function::str_impl(std::stringstream& out) const {
    out << "Function("
       << "index=" << m_index << ", "
       << "name=" << m_name << ", "
       << "arity=" << m_arity << ", "
       << "is_static=" << m_is_static
       << ")";
}

size_t Function::hash_impl() const {
    return hash_combine(m_name, m_index, m_arity, m_is_static);
}

const std::string& Function::get_name() const {
    return m_name;
}

int Function::get_arity() const {
    return m_arity;
}

bool Function::is_static() const {
    return m_is_static;
}

}
