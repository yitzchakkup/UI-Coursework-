#include "overviewPage.hpp"
#include <QMessageBox>
#include <iostream>
#include <string>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <sstream>
#include <ctime>

QPushButton* OverviewPage::createInfoButton()
{
    // Create the info button
    QPushButton* infoButton = new QPushButton(tr("Overview Info"), mainWindow);

    // Make the button small
    infoButton->setFixedSize(120, 40);  // You can adjust the size as needed

    // Style the button with a noticeable color (e.g., bright blue or green)
    infoButton->setStyleSheet("QPushButton {"
                              "background-color: #4CAF50;"  // Green background
                              "color: white;"               // White text color
                              "border-radius: 5px;"         // Rounded corners
                              "font-size: 14px;"            // Font size
                              "padding: 5px;"               // Padding around the text
                              "}"
                              "QPushButton:hover {"
                              "background-color: #45a049;"  // Darker green on hover
                              "}");

    // Connect the button to the show info slot
    connect(infoButton, SIGNAL(clicked()), this, SLOT(showOverviewPageExplanation()));

    return infoButton;
}

void OverviewPage::showOverviewPageExplanation()
{
    // Show the information pop-up when the button is clicked


    std::vector<std::string> pullutantNames = model->getData().getLabels();



    std::string print_info = "Overview Page Usage\n"
                             "Here you can find the overview of water data, including pollutants, "
                             "compliance, and trends. Use the controls to filter and analyze the data. "
                             "Search with the search bar to find any pollutant and get more info on it.\n";
    int index = 0;

    for (const auto& name : pullutantNames) {
        print_info += name + ", ";
        index++;
        if(index >10){
          break;
          }
    }

    // Remove trailing comma and space
    if (!pullutantNames.empty()) {
        print_info.pop_back();
        print_info.pop_back();

    }

    // Show the message box
    QMessageBox::information(
        mainWindow,
        tr("Overview Information"),
        QString::fromStdString(print_info)
    );
}


// Function to convert date string to tm struct
std::tm OverviewPage::TMstringToDate(const std::string& date) {
    std::tm tm = {};
    std::istringstream ss(date);
    ss >> std::get_time(&tm, "%Y-%m-%d");
    return tm;
}
QDate OverviewPage::stringToDate(const std::string& dateStr) {
    // Use QDate's fromString function to convert a date string to QDate
    return QDate::fromString(QString::fromStdString(dateStr), "yyyy-MM-dd");
}

// Comparison function for sorting by date
bool OverviewPage::compareByDate(const std::pair<std::string, double>& a, const std::pair<std::string, double>& b) {
    std::tm dateA = TMstringToDate(a.first);
    std::tm dateB = TMstringToDate(b.first);
    return std::mktime(&dateA) < std::mktime(&dateB);  // Compare by mktime (time_t)
}

QLineSeries* OverviewPage::createPollutantChart(const std::vector<std::pair<std::string, double>>& pollutantInfo) {
    // Create a new QLineSeries to hold the data
    QLineSeries* series = new QLineSeries();

    // Add data points to the series
    for (const auto& entry : pollutantInfo) {
        QDate date = stringToDate(entry.first); // Convert string date to QDate
        series->append(date.toJulianDay(), entry.second);  // Use Julian Day for X-axis
    }

    return series;  // Return the created QLineSeries
}
void OverviewPage::searchBarReturnPressed()
{
    centerLayout->removeWidget(chartView); // Detach it from the layout



    //currentChart = new QLabel();
    //rightPanelLayout->addWidget(currentChart);//todo change current chart

    // Logic to be executed when Enter is pressed in the search bar

    QString searchTerm = searchBar->text().trimmed();
    if (searchTerm.isEmpty()) {
        QMessageBox::warning(this, "Warning", "Search term cannot be empty.");
        centerLayout->addWidget(chartView);//unchanged
        return;
    }
    // Check if searchTerm exists in possiblePollutantNames
    std::vector<std::string> possiblePollutantNames = model->getData().getLabels();
    if (std::find(possiblePollutantNames.begin(), possiblePollutantNames.end(), searchTerm.toStdString()) == possiblePollutantNames.end()) {
        QMessageBox::warning(this, "Warning",
                        QString("Search a pollutant name under the given data.\n'%1' isn't a correct name.")
                        .arg(QString::fromStdString(searchTerm.toStdString())));

        centerLayout->addWidget(chartView); // unchanged
        return;
    }
    std::cout << "Search term: " << searchTerm.toStdString() << std::endl;


     chartView = createPollutantTabel(searchTerm.toStdString());
     centerLayout->addWidget(chartView);//todo deside what it gets




   // centerLayout->addWidget(pollutantsTable);

}

void OverviewPage::createSearchBar(QVBoxLayout* layout)
{
    searchBar = new QLineEdit();
    searchBar->setPlaceholderText(tr("Find pollutant..."));
    layout->addWidget(searchBar);

    // Connect the returnPressed signal to the slot
    connect(searchBar, SIGNAL(returnPressed()), this, SLOT(searchBarReturnPressed()));
}

QChartView* OverviewPage::createPollutantTabel(std::string pollutant)
{
    // Create a table for pollutant data
//    QTableWidget* curTable = new QTableWidget();
//    curTable->setRowCount(10);  // Placeholder, set based on actual data
//    curTable->setColumnCount(3); // Columns: Pollutant, Value, Compliance
//    curTable->setHorizontalHeaderLabels({tr("Pollutant"), tr("Value"), tr("Compliance")});
//    return curTable;





    std::vector<std::pair<std::string, double>> pollutantInfo;

    //    pollutantInfo = model->getData().getPollutants(searchTerm.toStdString(),
    //                                            mainWindow->curLocation()->currentText().toStdString());
    std::vector<std::string> allLocations =  model->getData().getLocations(pollutant);
    for (const auto& location : allLocations) {
        //        std::cout << location << std::endl;
        // Correct usage of push_back
        auto pollutants = model->getData().getPollutants(pollutant, location);

        // Add each pair in pollutants to pollutantInfo
        for (const auto& pollutant : pollutants) {
            pollutantInfo.push_back(pollutant);
        }
    }
    // Get the pollutant data based on the search term


    //todo continue writing this class
    // Sort the vector by date
    std::sort(pollutantInfo.begin(), pollutantInfo.end(), compareByDate);
//    std::cout << "Pollutant Info:" << std::endl;
//    for (const auto& entry : pollutantInfo) {
//        std::cout << "Pollutant: " << entry.first << ", Value: " << entry.second << std::endl;
//    }
    QLineSeries* line =createPollutantChart(pollutantInfo);


    QLineSeries* series = createPollutantChart(pollutantInfo);
    // Create a QChart object and add the series to it
    QChart* chart = new QChart();
    chart->addSeries(series);
    chart->createDefaultAxes();
    chart->setTitle("Pollutant Levels Over Time");

    // Create a QChartView to display the chart
    QChartView* curChartView = new QChartView(chart);
    curChartView->setRenderHint(QPainter::Antialiasing);
    return curChartView;
}

//QLabel* OverviewPage::createChart(std::string defaultPollutant)
//{
//    QLabel* chartPlaceholder = new QLabel(tr("Pollutant Trend Over Time (Chart Placeholder)"));
//    chartPlaceholder->setAlignment(Qt::AlignCenter);
//    chartPlaceholder->setStyleSheet("border: 1px solid gray; padding: 10px;");
//    return chartPlaceholder;
//}

//QGroupBox* OverviewPage::createColordData(std::string defaultPollutant)
//{
//    QGroupBox* dataBox = new QGroupBox(tr("Advanced Data"));
//    QVBoxLayout* dataBoxLayout = new QVBoxLayout();
//    dataBoxLayout->addWidget(new QLabel(tr("Risks: High")));
//    dataBoxLayout->addWidget(new QLabel(tr("Compliance: Within limits")));
//    dataBoxLayout->addWidget(new QLabel(tr("Safety thresholds: 50 µg/m³")));
//    dataBox->setLayout(dataBoxLayout);
//    return dataBox;
//}

QWidget* OverviewPage::createOverviewPage()
{

    centerLayout = new QVBoxLayout();
    // Main layout for the overview page
    //QVBoxLayout* layout = new QVBoxLayout();

    // Create a layout for the info button
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    QPushButton* infoButton = createInfoButton();  // Create the button
    buttonLayout->addWidget(infoButton, 0, Qt::AlignLeft);  // Align the button to the left
    centerLayout->addLayout(buttonLayout);  // Add button layout to main layout

    // Search bar at the top
    createSearchBar(centerLayout);

    // Center section: table and right panels


    // Default pollutant
    std::string defaultPollutant = "Chloroform";
    chartView = createPollutantTabel(defaultPollutant);
    centerLayout->addWidget(chartView);

    // Right-side layout for chart and data
    //rightPanelLayout = new QVBoxLayout();

    // Chart placeholder
    //currentChart = createChart(defaultPollutant);

    //rightPanelLayout->addWidget(currentChart);


    // Advanced data group box
    //rightPanelLayout->addWidget(createColordData(defaultPollutant));

    // Add the right panel layout to the center layout
    //centerLayout->addLayout(rightPanelLayout);

    // Add the center layout to the main layout
    //centerLayout->addLayout(centerLayout);

    // Set the main layout as the central widget
    QWidget* centralWidget = new QWidget();
    centralWidget->setLayout(centerLayout);
    return centralWidget;
}
