#ifndef OVERVIEWPAGE_HPP
#define OVERVIEWPAGE_HPP

#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QTableWidget>
#include <QLabel>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QString>
#include "model.hpp"  // Assuming the header for WaterModel exists
#include "window.hpp"
class WaterWindow;//forword declaration
class OverviewPage : public QWidget {
    Q_OBJECT

public:
    OverviewPage(WaterWindow* window, WaterModel* model, QWidget* parent = nullptr)
    : QWidget(parent), mainWindow(window), model(model) {}
    QWidget* createOverviewPage();

    private slots:
        void showOverviewPageExplanation();
    void searchBarReturnPressed();

private:
    QPushButton* createInfoButton();
    void createSearchBar(QVBoxLayout* layout);
    QTableWidget* createPollutantTabel(std::string pollutant);
    QLabel* createChart(std::string defaultPollutant);
    QGroupBox* createColordData(std::string defaultPollutant);


    WaterWindow* mainWindow;  // Reference to the main window
    QLineEdit* searchBar;
    WaterModel* model;  // Assuming WaterModel is part of the project
};

#endif // OVERVIEWPAGE_HPP 