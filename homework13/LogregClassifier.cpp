#include "LogregClassifier.h"

#include <cassert>
#include <cmath>
#include <numeric>
#include <stdexcept>

namespace
{

template<typename T>
auto sigma(T x)
{
    return 1 / (1 + std::exp(-x));
}

}

LogregClassifier::LogregClassifier(const CoefsType &coef)
    : m_coef(coef)
{
    assert(!m_coef.empty());
}

float LogregClassifier::predictProbability(const FeaturesType &feat) const
{
    if (feat.size() + 1 != m_coef.size())
        throw std::runtime_error("Feature vector size mismatch");

    auto z = std::inner_product(feat.begin(), feat.end(), ++m_coef.begin(), m_coef.front());
    return sigma(z);
}
