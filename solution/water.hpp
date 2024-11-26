// COMP2811 Coursework 1 sample solution: Water class

#pragma once

#include <string>
#include <iostream>
#include <optional>

// SamplingPoint: Represents where the sample was collected
struct SamplingPoint {
    std::string notation;
    float easting;
    float northing;
    std::string label;

    SamplingPoint(std::string notation, float easting, float northing, std::string label)
        : notation(notation), easting(easting), northing(northing), label(label) {}

    friend std::ostream &operator<<(std::ostream &os, const SamplingPoint &sp) {
        return os << "SamplingPoint: " << sp.label << " (" << sp.easting << ", " << sp.northing << ")";
    }
};

// Determinand: Describes what is being measured
struct Determinand {
    std::string label;
    std::string definition;
    std::string notation;
    std::string unit;

    Determinand(std::string label, std::string definition, std::string notation, std::string unit)
        : label(label), definition(definition), notation(notation), unit(unit) {}

    friend std::ostream &operator<<(std::ostream &os, const Determinand &det) {
        return os << "Determinand: " << det.label << " [" << det.unit << "]";
    }
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

    friend std::ostream &operator<<(std::ostream &os, const Sample &sample) {
        return os << "Sample taken at: " << sample.samplingPoint << " on " << sample.dateTime
                  << " for purpose: " << sample.purpose;
    }
};

// Water: Encapsulates all attributes of a water measurement
class Water {
private:
    std::string id;
    float result;
    std::optional<std::string> resultQualifier;
    std::optional<std::string> resultInterpretation;
    Sample sample;
    Determinand determinand;

public:
    Water(const std::string &id, float result, std::optional<std::string> resultQualifier,
          std::optional<std::string> resultInterpretation, Sample sample, Determinand determinand)
        : id(id), result(result), resultQualifier(resultQualifier),
          resultInterpretation(resultInterpretation), sample(sample), determinand(determinand) {}

    friend std::ostream &operator<<(std::ostream &os, const Water &water) {
        os << "Water Sample ID: " << water.id << "\n"
           << "Result: " << water.result << " (" << (water.resultQualifier ? *water.resultQualifier : "") << ")\n"
           << "Interpretation: " << (water.resultInterpretation ? *water.resultInterpretation : "None") << "\n"
           << water.sample << "\n"
           << water.determinand;
        return os;
    }
};