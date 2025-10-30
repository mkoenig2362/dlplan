#ifndef DLPLAN_INCLUDE_DLPLAN_CORE_PARSERS_SYNTACTIC_PARSER_API_HPP_
#define DLPLAN_INCLUDE_DLPLAN_CORE_PARSERS_SYNTACTIC_PARSER_API_HPP_

#include "ast.hpp"

#include <boost/spirit/home/x3.hpp>


namespace dlplan::core
{
    namespace x3 = boost::spirit::x3;

    ///////////////////////////////////////////////////////////////////////////
    // Parser public interface
    ///////////////////////////////////////////////////////////////////////////
    namespace parser {
        struct ElementClass;
        struct BooleanClass;
        struct NumericalClass;
        struct ConceptClass;
        struct RoleClass;
        struct FrameUnaryClass;
        struct FrameBinaryClass;

        typedef x3::rule<ElementClass, ast::Element> element_type;
        typedef x3::rule<BooleanClass, ast::Boolean> boolean_type;
        typedef x3::rule<NumericalClass, ast::Numerical> numerical_type;
        typedef x3::rule<ConceptClass, ast::Concept> concept_type;
        typedef x3::rule<RoleClass, ast::Role> role_type;
        typedef x3::rule<FrameUnaryClass, ast::FrameUnary> frame_unary_type;
        typedef x3::rule<FrameBinaryClass, ast::FrameBinary> frame_binary_type;

        BOOST_SPIRIT_DECLARE(element_type, boolean_type, numerical_type, concept_type, role_type, frame_unary_type, frame_binary_type)
    }

    parser::element_type const& element();
    parser::boolean_type const& boolean();
    parser::numerical_type const& numerical();
    parser::concept_type const& concept_();
    parser::role_type const& role();
    parser::frame_unary_type const& frame_unary();
    parser::frame_binary_type const& frame_binary();
}

#endif