// COMP2811 Coursework 1 sample solution: Water class

#pragma once

#include <string>
#include <iostream>
#include <optional>


// SamplingPoint: Represents where the sample was collected
class SamplingPoint {
  private:
    std::string notation;
    float easting;
    float northing;
    std::string label;
  public:
    SamplingPoint(std::string notation, float easting, float northing, std::string label)
        : notation(notation), easting(easting), northing(northing), label(label) {}
    std::string getNotation() const { return notation; }
    float getEasting() const { return easting; }
    float getNorthing() const { return northing; }
    std::string getLabel() const { return label; }
};

// Determinand: Describes what is being measured
class Determinand {
  private:
    std::string label;
    std::string definition;
    std::string notation;
    std::string unit;
  public:
    Determinand(std::string label, std::string definition, std::string notation, std::string unit)
        : label(label), definition(definition), notation(notation), unit(unit) {}
    std::string getLabel() const { return label; }
    std::string getDefinition() const { return definition; }
    std::string getNotation() const { return notation; }
    std::string getUnit() const { return unit; }
};

// Sample: Represents a collected water sample
class Sample {
  private:
    SamplingPoint samplingPoint;
    std::string purpose;
    std::string materialType;
    std::string dateTime;
    bool isCompliance;

  public:
    Sample(SamplingPoint samplingPoint, std::string purpose, std::string materialType,
           std::string dateTime, bool isCompliance)
        : samplingPoint(samplingPoint), purpose(purpose), materialType(materialType),
          dateTime(dateTime), isCompliance(isCompliance) {}

    SamplingPoint getSamplingPoint() const { return samplingPoint; }
    std::string getPurpose() const { return purpose; }
    std::string getMaterialType() const { return materialType; }
    std::string getDateTime() const { return dateTime; }
    bool getIsCompliance() const { return isCompliance; }

};

// Water: Encapsulates all attributes of a water measurement
class Water {
  private:
      std::string id;
      float result;
      std::optional<std::string> resultQualifier; //optional entity
      std::optional<std::string> resultInterpretation; //optional entity 
      Sample sample;
      Determinand determinand;

  public:
      Water(const std::string &id, float result, std::optional<std::string> resultQualifier,
            std::optional<std::string> resultInterpretation, Sample sample, Determinand determinand);
      
      std::string getId() const { return id; }
      float getResult() const { return result; }
      std::optional<std::string> getResultQualifier() const { return resultQualifier; }
      std::optional<std::string> getResultInterpretation() const { return resultInterpretation; }
      Sample getSample() const { return sample; }
      Determinand getDeterminand() const { return determinand; }

    
};
