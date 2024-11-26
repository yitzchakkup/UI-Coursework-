// COMP2811 Coursework 1 sample solution: Quake class

#include <stdexcept>
#include <sstream>
#include "water.hpp"

using namespace std;


Water::Water(const std::string &id, float result, std::optional<std::string> resultQualifier,
          std::optional<std::string> resultInterpretation, Sample sample, Determinand determinand)
        : id(id), result(result), resultQualifier(resultQualifier),
          resultInterpretation(resultInterpretation), sample(sample), determinand(determinand)
{
  ostringstream error;
}


ostream& operator<<(ostream& out, const Water& water)
{
  return out << "Water Sample ID: " << water.id << "\n"
           << "Result: " << water.result << " (" << (water.resultQualifier ? *water.resultQualifier : "") << ")\n"
           << "Interpretation: " << (water.resultInterpretation ? *water.resultInterpretation : "None") << "\n"
           << water.sample << "\n"
           << water.determinand << endl;
}
