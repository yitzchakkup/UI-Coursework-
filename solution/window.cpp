// COMP2811 Coursework 2 sample solution: main window

#include <QtWidgets>
#include <stdexcept>
#include <iostream>
#include "window.hpp"
#include "stats.hpp"
#include "water.hpp"
#include "overwiewPage.hpp"



static const int MIN_WIDTH = 620;

WaterWindow::WaterWindow() : QMainWindow(), statsDialog(nullptr)
{
  createTest();
  createFileSelectors();
  createButtons();
  createToolBar();
  createStatusBar();
  addFileMenu();
  addHelpMenu();

  createPageBar();

  createPOPs();

  setMinimumWidth(MIN_WIDTH);
  setWindowTitle("Water Tool");
}


void WaterWindow::createTest()
{
  table = new QTableView();
  table->setModel(&model);

  QFont tableFont = QFontDatabase::systemFont(QFontDatabase::FixedFont);
  table->setFont(tableFont);

  setCentralWidget(table);
}

//todo my code is from here
void WaterWindow::searchBarReturnPressed()
{
  // Logic to be executed when Enter is pressed in the search bar
  QString searchTerm = searchBar->text().trimmed();
  std::cerr << "Search term: " << searchTerm.toStdString() << std::endl;
  createPOPs();
  // You can call your search or filtering function here
  // Example: model.filterData(searchTerm);
}

void WaterWindow::createSearchBar(QVBoxLayout* layout)
{
  QLineEdit* searchBar = new QLineEdit();
  searchBar->setPlaceholderText("Find pollutant...");
  layout->addWidget(searchBar);

  // Connect the returnPressed signal to the slot
  connect(searchBar, &QLineEdit::returnPressed, this, &WaterWindow::searchBarReturnPressed);
}


QWidget* WaterWindow::creatOverviewPage() {

  //todo nake helper function, there all taking info from ****model****
    QVBoxLayout* layout = new QVBoxLayout();

    // Search bar at the top
    createSearchBar(layout);
    // Center section: table and right panels
    QHBoxLayout* centerLayout = new QHBoxLayout();

    // Pollutants table
    QTableWidget* pollutantsTable = new QTableWidget();
    pollutantsTable->setRowCount(10); // Example: 10 rows
    pollutantsTable->setColumnCount(3); // Example: 3 columns
    pollutantsTable->setHorizontalHeaderLabels({"Pollutant", "Value", "Compliance"});
    centerLayout->addWidget(pollutantsTable);

    // Right-side layout for chart and data
    QVBoxLayout* rightPanelLayout = new QVBoxLayout();

    // Chart placeholder
    QLabel* chartPlaceholder = new QLabel("Pollutant Trend Over Time (Chart Placeholder)");
    chartPlaceholder->setAlignment(Qt::AlignCenter);
    chartPlaceholder->setStyleSheet("border: 1px solid gray; padding: 10px;");
    rightPanelLayout->addWidget(chartPlaceholder);

    // Advanced data group box
    QGroupBox* dataBox = new QGroupBox("Advanced Data");
    QVBoxLayout* dataBoxLayout = new QVBoxLayout();
    dataBoxLayout->addWidget(new QLabel("Risks: High"));
    dataBoxLayout->addWidget(new QLabel("Compliance: Within limits"));
    dataBoxLayout->addWidget(new QLabel("Safety thresholds: 50 µg/m³"));
    dataBox->setLayout(dataBoxLayout);
    rightPanelLayout->addWidget(dataBox);

    // Add the right panel layout to the center layout
    centerLayout->addLayout(rightPanelLayout);

    // Add the center layout to the main layout
    layout->addLayout(centerLayout);
  //}


  QWidget* centralWidget = new QWidget();//"boxing layout
  centralWidget->setLayout(layout);
  return centralWidget;
}


void WaterWindow::createPOPs() {
  //todo make singolton? for some reasen breaks the code
  //if(!singoltenOverviewPage) {
    singoltenOverviewPage = creatOverviewPage();
  //}
  //QVBoxLayout* layout = WaterOverwiewPage::createOverviewPage();
  setCentralWidget(singoltenOverviewPage);
}


void WaterWindow::createLitter(){}
void WaterWindow::createFlourinated(){}
void WaterWindow::createCompliance(){}



void WaterWindow::createPageBar()
{
  QToolBar *pageBar = new QToolBar();

  pageBar->addWidget(overviewButton);
  pageBar->addWidget(popsButton);
  pageBar->addWidget(litterButton);
  pageBar->addWidget(flourinatedButton);
  pageBar->addWidget(complianceButton);


  addToolBar(Qt::TopToolBarArea, pageBar);
}


// change this
void WaterWindow::createFileSelectors()
{
  QStringList pollutantOptions;
  pollutantOptions << "pollutants";
  pollutant = new QComboBox();
  pollutant->addItems(pollutantOptions);

  QStringList periodOptions;
  periodOptions << "hour" << "day" << "week" << "month";
  period = new QComboBox();
  period->addItems(periodOptions);
}

void WaterWindow::createButtons()
{
  loadButton = new QPushButton("Load");
  statsButton = new QPushButton("Stats");
  overviewButton = new QPushButton("Overview");
  popsButton = new QPushButton("POPs");
  litterButton = new QPushButton("Litter Indicators");
  flourinatedButton = new QPushButton("Flourinated compounds");
  complianceButton = new QPushButton("Compliance Dashboard");


  connect(loadButton, SIGNAL(clicked()), this, SLOT(openCSV()));
  connect(statsButton, SIGNAL(clicked()), this, SLOT(displayStats()));

  //todo im going to put my code in crate Pops evntully needs to be moved to createtest
  connect(overviewButton, SIGNAL(clicked()), this, SLOT(createTest()));
  connect(popsButton, SIGNAL(clicked()), this, SLOT(createPOPs()));
  connect(litterButton, SIGNAL(clicked()), this, SLOT(createLitter()));
  connect(flourinatedButton, SIGNAL(clicked()), this, SLOT(createFlourinated()));
  connect(complianceButton, SIGNAL(clicked()), this, SLOT(createCompliance()));
}

void WaterWindow::createToolBar()
{
  QToolBar *toolBar = new QToolBar();

  QLabel *pollutantLabel = new QLabel("Pollutant");
  pollutantLabel->setAlignment(Qt::AlignVCenter);
  toolBar->addWidget(pollutantLabel);
  toolBar->addWidget(pollutant);

  QLabel *periodLabel = new QLabel("Period");
  periodLabel->setAlignment(Qt::AlignVCenter);
  toolBar->addWidget(periodLabel);
  toolBar->addWidget(period);

  toolBar->addSeparator();

  toolBar->addWidget(loadButton);
  toolBar->addWidget(statsButton);

  addToolBar(Qt::LeftToolBarArea, toolBar);
}

void WaterWindow::createStatusBar()
{
  fileInfo = new QLabel("Current file: <none>");
  QStatusBar *status = statusBar();
  status->addWidget(fileInfo);
}

void WaterWindow::addFileMenu()
{
  QAction *locAction = new QAction("Set Data &Location", this);
  locAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_L));
  connect(locAction, SIGNAL(triggered()), this, SLOT(setDataLocation()));

  QAction *closeAction = new QAction("Quit", this);
  closeAction->setShortcut(QKeySequence::Close);
  connect(closeAction, SIGNAL(triggered()), this, SLOT(close()));

  QMenu *fileMenu = menuBar()->addMenu("&File");
  fileMenu->addAction(locAction);
  fileMenu->addAction(closeAction);
}

void WaterWindow::addHelpMenu()
{
  QAction *aboutAction = new QAction("&About", this);
  connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));

  QAction *aboutQtAction = new QAction("About &Qt", this);
  connect(aboutQtAction, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

  QMenu *helpMenu = menuBar()->addMenu("&Help");
  helpMenu->addAction(aboutAction);
  helpMenu->addAction(aboutQtAction);
}


void WaterWindow::setDataLocation()
{
  QString directory = QFileDialog::getExistingDirectory(
      this, "Data Location", ".",
      QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

  if (directory.length() > 0)
  {
    dataLocation = directory;
  }
}

void WaterWindow::openCSV()
{
  if (dataLocation == "")
  {
    QMessageBox::critical(this, "Data Location Error",
                          "Data location has not been set!\n\n"
                          "You can specify this via the File menu.");
    return;
  }

  auto filename = QString("Y-2024.csv");

  auto path = dataLocation + "/" + filename;
  std::cout << path.toStdString() << std::endl;
  try
  {
    model.updateFromFile(path);
  }
  catch (const std::exception &error)
  {
    QMessageBox::critical(this, "CSV File Error", error.what());
    return;
  }

  fileInfo->setText(QString("Current file: <kbd>%1</kbd>").arg(filename));
  table->resizeColumnsToContents();

  if (statsDialog != nullptr)
  {
    statsDialog->hide();
  }
}

// this is the function for the table view
// needs to be updated to view from water using stats dialouge

void WaterWindow::displayStats()
{
  if (model.hasData())
  {
    if (statsDialog == nullptr)
    {
      statsDialog = new StatsDialog(this);
    }

    // statsDialog->update(model.meanDepth(), model.meanMagnitude());

    statsDialog->show();
    statsDialog->raise();
    statsDialog->activateWindow();
  }
}


void WaterWindow::about()
{
  QMessageBox::about(this, "About Quake Tool",
                     "Quake Tool displays and analyzes earthquake data loaded from"
                     "a CSV file produced by the USGS Earthquake Hazards Program.\n\n"
                     "(c) 2024 Nick Efford");
}
