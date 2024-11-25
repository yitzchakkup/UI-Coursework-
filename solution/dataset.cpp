// COMP2811 Coursework 1 sample solution: QuakeDataset class

#include <stdexcept>
#include <algorithm>
#include <numeric>
#include "dataset.hpp"
#include "csv.hpp"

using namespace std;

void WaterDataset::loadData(const string &filename)
{
  csv::CSVReader reader(filename);

  data.clear();

  for (const auto &row : reader)
  {
    Water water{
        row["id"].get<>(),
        row["result"].get<float>(),
    };
    data.push_back(quake);
  }
}

Quake QuakeDataset::strongest() const
{
  checkDataExists();

  auto quake = max_element(data.begin(), data.end(),
                           [](Quake a, Quake b)
                           { return a.getMagnitude() < b.getMagnitude(); });

  return *quake;
}

Quake QuakeDataset::shallowest() const
{
  checkDataExists();

  auto quake = min_element(data.begin(), data.end(),
                           [](Quake a, Quake b)
                           { return a.getDepth() < b.getDepth(); });

  return *quake;
}

double QuakeDataset::meanDepth() const
{
  checkDataExists();

  auto sum = accumulate(data.begin(), data.end(), 0.0,
                        [](double total, Quake q)
                        { return total + q.getDepth(); });

  return sum / data.size();
}

double QuakeDataset::meanMagnitude() const
{
  checkDataExists();

  auto sum = accumulate(data.begin(), data.end(), 0.0,
                        [](double total, Quake q)
                        { return total + q.getMagnitude(); });

  return sum / data.size();
}

void QuakeDataset::checkDataExists() const
{
  if (size() == 0)
  {
    throw std::runtime_error("Dataset is empty!");
  }
}
