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
     centerLayout->addWidget(chartView);




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
    std::vector<std::pair<std::string, double>> pollutantInfo;

    std::vector<std::string> allLocations = model->getData().getLocations(pollutant);
    for (const auto& location : allLocations) {
        // Correct usage of push_back
        auto pollutants = model->getData().getPollutants(pollutant, location);

        // Add each pair in pollutants to pollutantInfo
        for (const auto& pollutant : pollutants) {
            pollutantInfo.push_back(pollutant);
        }
    }

    std::sort(pollutantInfo.begin(), pollutantInfo.end(), compareByDate);

    QLineSeries* series = createPollutantChart(pollutantInfo);

    // Create a QChart object and add the series to it
    QChart* chart = new QChart();
    chart->addSeries(series);

    // Configure the X-axis (Time)
    QCategoryAxis* axisX = new QCategoryAxis();
    axisX->append("", 0); // Adds a blank tick for alignment
    axisX->setTitleText("Time");

    QFont axisFont;
    axisFont.setPointSize(14); // Adjust font size
    axisX->setTitleFont(axisFont);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    // Configure the Y-axis (µg/L)
    QValueAxis* axisY = new QValueAxis();
    axisY->setTitleText("µg/L");
    axisY->setLabelFormat("%.1f");
    axisY->setTitleFont(axisFont); // Apply the same larger font
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    chart->setTitle("Pollutant Levels Over Time");

    // Create a QChartView to display the chart
    QChartView* curChartView = new QChartView(chart);
    curChartView->setRenderHint(QPainter::Antialiasing);
    return curChartView;
}

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


    // Set the main layout as the central widget
    QWidget* centralWidget = new QWidget();
    centralWidget->setLayout(centerLayout);
    return centralWidget;
}
