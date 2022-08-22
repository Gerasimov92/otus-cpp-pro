#pragma once

#include "IBinaryClassifier.h"

class LogregClassifier : public IBinaryClassifier
{
public:
    using CoefsType = FeaturesType;

    LogregClassifier(const CoefsType &coef);

    float predictProbability(const FeaturesType &feat) const override;

protected:
    CoefsType m_coef;
};

