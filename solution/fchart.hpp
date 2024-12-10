#include <QLineSeries>
#include <QChart>
#include <QtWidgets>
#include <QComboBox>
#include <QChartView>
#include <QLogValueAxis>
#include <QValueAxis>
#include <QFrame>
#include "dataset.hpp"
#include <QString>

class FlourineChart : public QLineSeries
{
public:
    FlourineChart(QObject *parent = nullptr) : QLineSeries(parent) {}
    void loadDataset(WaterDataset data) { dataset = data; };
    QStringList getDeterminands();
    QStringList getLocations(std::string pollutant);
    void createDataset(WaterDataset data, std::string pollutant);
    bool check(std::string label);
    void updateChart(QChart *chart, std::string pollutant);
    void initChart(QChart *chart);
    void updateCompliance(QLabel *pfaLabel, QLabel *locationLabel, QFrame *complianceBar,
                          std::string pollutant, std::string location);

    /*
    double meanDepth() const { return dataset.meanDepth(); }
    double meanMagnitude() const { return dataset.meanMagnitude(); }
    */

private:
    WaterDataset dataset;
    QLineSeries *series; // set these for now may move to window
    QChart *chart;       // set these for now may move to window
    QLogValueAxis *axisY;
    QValueAxis *axisX;
};
