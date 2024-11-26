// COMP2811 Coursework 1 sample solution: QuakeDataset class

#pragma once

#include <vector>
#include "water.hpp"

class WaterDataset
{
  public:
    WaterDataset() {}
    WaterDataset(const std::string& filename) { loadData(filename); }
    void loadData(const std::string&);
    int size() const { return data.size(); }
    Water operator[](int index) const { return data.at(index); }

  private:
    std::vector<Water> data;
    void checkDataExists() const;
};
