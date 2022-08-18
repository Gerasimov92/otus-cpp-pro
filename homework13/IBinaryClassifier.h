#pragma once

#include <vector>

class IBinaryClassifier
{
public:
    using FeaturesType = std::vector<float>;

    virtual ~IBinaryClassifier() {}

    virtual float predictProbability(const FeaturesType &) const = 0;
};
