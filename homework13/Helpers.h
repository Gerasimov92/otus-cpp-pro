#include <istream>
#include <vector>

namespace Helpers
{

bool readCoefs(std::istream &stream, std::vector<float> &coefs);
bool readFeatures(std::istream &stream, std::vector<float> &features, int &targetClass);

} // namespace Helpers
