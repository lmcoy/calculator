//
//  Created by Lennart Oymanns on 14.07.17.
//

#ifndef ComplexNumber_hpp
#define ComplexNumber_hpp

#include <complex>
#include <memory>

#include "NumberRepr.hpp"

namespace Equation {

class Node;
typedef std::shared_ptr<Node> NodePtr;

std::pair<std::complex<NumberRepr>, bool> complex_number(NodePtr n);

NodePtr node_from_complex(std::complex<double> c);

NodePtr node_from_complex(std::complex<NumberRepr> c);

std::complex<NumberRepr> complex_mul(const std::complex<NumberRepr> &a,
                                     const std::complex<NumberRepr> &b);

} // namespace Equation

#endif /* ComplexNumber_hpp */
