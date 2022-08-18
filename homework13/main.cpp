#include <iostream>
#include <fstream>

#include "Helpers.h"
#include "LogregClassifier.h"

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        std::cerr << "Usage: fashion_mnist <input_csv> <logreg_coef>" << std::endl;
        return 1;
    }

    std::vector<LogregClassifier> classifiers;
    LogregClassifier::CoefsType coefs;

    std::ifstream coefInput(argv[2]);
    while (true)
    {
        if (!Helpers::readCoefs(coefInput, coefs))
            break;

        classifiers.emplace_back(LogregClassifier(coefs));
    }
    coefInput.close();

    std::ifstream dataInput(argv[1]);

    LogregClassifier::FeaturesType features;
    int targetClass;
    int totalCount = 0;
    int rightAnswersCount = 0;

    while (true)
    {
        if (!Helpers::readFeatures(dataInput, features, targetClass))
            break;

        totalCount++;

        float maxResult = -1;
        int maxResultClass = 0;

        for (size_t i = 0; i < classifiers.size(); i++)
        {
            auto result = classifiers[i].predictProbability(features);
            if (result > maxResult)
            {
                maxResult = result;
                maxResultClass = i;
            }
        }

        if (maxResultClass == targetClass)
            rightAnswersCount++;
    }

    float accuracy = 0;
    if (totalCount > 0)
    {
        accuracy = float(rightAnswersCount) / totalCount;
    }

    std::cout << accuracy << std::endl;

    return 0;
}
