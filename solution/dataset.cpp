// COMP2811 Coursework 1 sample solution: QuakeDataset class

#include <stdexcept>
#include <algorithm>
#include <numeric>
#include "dataset.hpp"
#include "csv.hpp"
#include "water.hpp"

#include <iostream>
#include <string>

using namespace std;

// complete reading the csv into the data set
void WaterDataset::loadData(const string &filename)
{
  csv::CSVReader reader(filename);

  data.clear();
  int i = 0;
  for (const auto &row : reader)
  {
    std::cout << i << endl;
    i += 1;
    SamplingPoint samplingPoint{
        row["sample.samplingPoint.notation"].get<string>(),
        row["sample.samplingPoint.easting"].get<float>(),
        row["sample.samplingPoint.northing"].get<float>(),
        row["sample.samplingPoint.label"].get<string>(),
    };
    std::cout << "S" << endl;
    Determinand determinand{
        row["determinand.label"].get<string>(),
        row["determinand.definition"].get<string>(),
        row["determinand.notation"].get<string>(),
        row["determinand.unit.label"].get<string>(),
    };
    std::cout << "D" << endl;
    bool isCompliance = row["sample.isComplianceSample"].get<string>() == "TRUE";

    Sample sample{
        samplingPoint,
        row["sample.purpose.label"].get<string>(),
        row["sample.sampledMaterialType.label"].get<string>(),
        row["sample.sampleDateTime"].get<string>(),
        isCompliance,
    };
    std::cout << "Structures" << endl;
    Water water{
        row["@id"].get<string>(),
        row["result"].get<float>(),
        row["resultQualifier.notation"].get<string>(),
        row["codedResultInterpretation.interpretation"].get<string>(),
        sample,
        determinand,
    };
    std::cout << "push back" << endl;

    data.push_back(water);
  }
}

/*
Quake QuakeDataset::strongest() const
{
  checkDataExists();

  auto quake = max_element(data.begin(), data.end(),
    [](Quake a, Quake b) { return a.getMagnitude() < b.getMagnitude(); });

  return *quake;
}


Quake QuakeDataset::shallowest() const
{
  checkDataExists();

  auto quake = min_element(data.begin(), data.end(),
    [](Quake a, Quake b) { return a.getDepth() < b.getDepth(); });

  return *quake;
}


double QuakeDataset::meanDepth() const
{
  checkDataExists();

  auto sum = accumulate(data.begin(), data.end(), 0.0,
    [](double total, Quake q) { return total + q.getDepth(); });

  return sum / data.size();
}


double QuakeDataset::meanMagnitude() const
{
  checkDataExists();

  auto sum = accumulate(data.begin(), data.end(), 0.0,
    [](double total, Quake q) { return total + q.getMagnitude(); });

  return sum / data.size();
}

*/

void WaterDataset::checkDataExists() const
{
  if (size() == 0)
  {
    throw std::runtime_error("Dataset is empty!");
  }
}
