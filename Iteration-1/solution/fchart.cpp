#include "fchart.hpp"
#include "water.hpp"
#include <QLineSeries>
#include <QString>
#include <iostream>
#include <algorithm>

bool FlourineChart::check(std::string label)
{
    if (label.find("flu") != std::string::npos)
    {
        return true;
    }
    else
    {
        size_t pos = label.find("per");
        if (pos != std::string::npos && (pos == 0 || !isalpha(label[pos - 1])))
        {
            return true;
        }
    }
    return false;
}

QStringList FlourineChart::getDeterminands()
{
    // creating a map to see how many occurences and if it is a plottable PFAS or florinated compound
    std::unordered_map<std::string, int> determinandMap;
    QStringList determinands;
    for (int i = 0; i < dataset.size(); i++)
    {
        Water w = dataset[i];
        std::string label = w.getDeterminand().getLabel();
        std::string definition = w.getDeterminand().getDefinition();
        // filter for flourinated compounds
        if (check(label) || check(definition))
        {

            if (determinands.indexOf(QString::fromStdString(label)) == -1)
            {
                if (w.getResult() > 0.001 && w.getResult() < 1)
                {
                    if (determinandMap.find(label) == determinandMap.end())
                    {
                        determinandMap[label] = 1;
                    }
                    else
                    {
                        if (determinandMap[label] == 10)
                        {
                            determinands << QString::fromStdString(label);
                        }
                        determinandMap[label]++;
                    }
                }
            }
        }
    }
    return determinands;
}

QStringList FlourineChart::getLocations(std::string pollutant)
{
    std::cout << "location pollutant" << std::endl;
    std::cout << pollutant << std::endl;

    QStringList locations;
    if (pollutant == "")
    {
        return locations;
    }
    else
    {
        for (int i = 0; i < dataset.size(); i++)
        {
            Water w = dataset[i];
            std::string location = w.getSample().getSamplingPoint().getLabel();
            std::string label = w.getDeterminand().getLabel();
            if (label == pollutant)
            {
                if (locations.indexOf(QString::fromStdString(location)) == -1)
                {
                    locations << QString::fromStdString(location);
                }
            }
        }
    }
    return locations;
}

void FlourineChart::createDataset(WaterDataset data, std::string pollutant)
{
    std::vector<std::pair<double, double>> plots;

    for (int i = 0; i < dataset.size(); i++)
    {
        Water w = data[i];
        // for each each data point for that pollutant
        if (w.getDeterminand().getLabel() == pollutant && w.getResult() > 0.001 && w.getResult() < 1)
        {
            double result = w.getResult();
            std::string time = w.getSample().getDateTime();

            std::string time_y = time.substr(0, time.find("T"));
            time_y.erase(4, 1);
            time_y.erase(6, 1);
            std::string time_d = time.substr(time.find("T") + 1, 18);
            time_d.erase(2, 1);
            time_d.erase(4, 1);
            std::string combined_time = time_y + time_d;
            std::cout << combined_time << std::endl;
            std::cout << result << std::endl;
            plots.emplace_back(std::stod(combined_time), result);

            std::cout << "appended" << std::endl;
        }
    }
    std::sort(plots.begin(), plots.end(), [](const std::pair<double, double> &a, const std::pair<double, double> &b)
              { return a.first < b.first; });

    for (const auto &plot : plots)
    {
        series->append(plot.first, plot.second);
    }
}

void FlourineChart::updateChart(QChart *chart, std::string pollutant)
{
    series->clear();
    chart->removeSeries(series);
    series = new QLineSeries();
    createDataset(dataset, pollutant);

    // Attach axes to the chart and series
    chart->addSeries(series);
    chart->setTitle(QString::fromStdString(pollutant));
}

void FlourineChart::initChart(QChart *chart)
{
    series = new QLineSeries();
    chart->legend()->hide();
    chart->addSeries(series);
    std::cout << "creating axis " << std::endl;

    // logarithmic Y axis
    axisY = new QLogValueAxis();
    axisY->setTitleText("Concentration (Log Scale)");
    axisY->setBase(10);
    axisY->setRange(0.001, 1);
    axisY->setLabelFormat("%.3f");
    axisY->setMinorTickCount(-1);

    // Create a linear X-axis
    axisX = new QValueAxis();
    axisX->setTitleText("Time");
    axisX->setLabelFormat("%.0f");
    axisX->setTickCount(10);

    std::cout << "init chart" << std::endl;
    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);

    series->attachAxis(axisX);
    series->attachAxis(axisY);

    chart->setTitle("Flourinated Compounds");
}