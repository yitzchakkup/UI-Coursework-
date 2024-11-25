// COMP2811 Coursework 1 sample solution: Water class

#pragma once

#include <string>
#include <iostream>

class Water
{
private:
  class SamplingPoint
  {
  public:
    SamplingPoint(std::string notation, float easting, float northing, std::string label)
        : notation(notation), easting(easting), northing(northing), label(label) {}

    friend class Water;

  private:
    std::string notation;
    float easting;
    float northing;
    std::string label;
  };

  class Determinand
  {
  public:
    Determinand(std::string label, std::string definition, std::string notation, std::string unit)
        : label(label), definition(definition), notation(notation), unit(unit) {}

    friend class Water;

  private:
    std::string label;
    std::string definition;
    std::string notation;
    std::string unit;
  };

  class Sample
  {
  private:
    SamplingPoint samplingPoint;
    std::string purpose;
    std::string materialType;
    std::string dateTime;
    bool isCompliance;

  public:
    // Constructor
    Sample(SamplingPoint samplingPoint, std::string purpose, std::string materialType,
           std::string dateTime, bool isCompliance)
        : samplingPoint(samplingPoint), purpose(purpose), materialType(materialType),
          dateTime(dateTime), isCompliance(isCompliance) {}
  };

  std::string id;
  float result;
  std::string resultQualifier;
  std::string resultInterpretation;
  Sample sample;
  Determinand determinand;

public:
  Water(const std::string &, float, string, string, Sample, Determinand)
      : id(id), result(result), resultQualifier(resultQualifier), resultInterpretation(resultInterpretation),
        sample(sample), determinand(determinand) {}
};

std::ostream &operator<<(std::ostream &, const Water &);
// formating