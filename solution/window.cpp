// COMP2811 Coursework 2 sample solution: main window

#include <QtWidgets>
#include <stdexcept>
#include <iostream>
#include "window.hpp"
#include "stats.hpp"
#include "water.hpp"
#include <QtCharts>
#include <QLineSeries>
#include <QChart>
#include <QChartView>
#include <QToolBox>
#include <QProgressBar>
#include <QFrame>
#include "dataset.hpp"
#include <string>
#include <utility>
#include <vector>
#include <map>
#include "overviewPage.hpp"

static const int MIN_WIDTH = 620;

WaterWindow::WaterWindow() : QMainWindow(), statsDialog(nullptr)
{
  createFileSelectors();
  createButtons();
  createToolBar();
  createStatusBar();
  addFileMenu();
  addHelpMenu();
  createPageBar();
  setMinimumWidth(MIN_WIDTH);
  setWindowTitle("Water Tool");
}

void WaterWindow::createDashboard()
{
  setCentralWidget(0);

  table = new QTableView();
  table->setModel(&model);

  QFont tableFont = QFontDatabase::systemFont(QFontDatabase::FixedFont);
  table->setFont(tableFont);

  setCentralWidget(table);
}

void WaterWindow::showDataLoadedQuestion()
{
  //todo dhejhrfuws
  QMessageBox *msgBox = new QMessageBox(this);
  msgBox->setWindowTitle("Question");
  msgBox->setText("Do you want to see the loaded data?");
  msgBox->setStandardButtons(QMessageBox::Yes | QMessageBox::No);
  msgBox->setDefaultButton(QMessageBox::No);

  int ret = msgBox->exec();

  if (ret == QMessageBox::Yes)
  {
    // Handle Yes response
    createDashboard();
  }
}

void WaterWindow::createOverview()
{
  //todo make singolton? for some reasen breaks the code
  //if(!singoltenOverviewPage) {
  OverviewPage* page = new OverviewPage(this, &model);
  singoltenOverviewPage = page->createOverviewPage();
  //}
  //QVBoxLayout* layout = WaterOverviewPage::createOverviewPage();
  setCentralWidget(singoltenOverviewPage);
}

void WaterWindow::createPOPs()
{
  setCentralWidget(0);

  QStringList popsList = {"PCB Con 028", "PCB Con 105", "PCB Con 052", "PCB Con 101",
  "PCB Con 138", "PCB Con 156", "PCB Con 118", "PCB Con 153", "PCB Con 180"};

  updateFileSelector(pollutant, popsList);

  auto *popsChart = new QChart;
  auto *popSeries = new QBarSeries();
  popSeries->setName("POPs Overview");
  
  popsChart->addSeries(popSeries);
  popsChart->setTitle("POPs Overview");
  popsChart->createDefaultAxes();
  
  auto popsChartView = new QChartView(popsChart);

  auto popsLayout = new QVBoxLayout();
  popsLayout->addWidget(popsChartView);
  pops = new QWidget();
  pops->setLayout(popsLayout);
  //pops->addWidget(popsChart);
  setCentralWidget(pops);
}

void WaterWindow::createLitter()
{
  setCentralWidget(0);
  QStringList litterList = {"Bathing Water Profile : Other Litter (incl. plastics)",
  "Sewage debris", "Tarry residues"};
  QStringList locationList = {"BRIDLINGTON NORTH (08000)", "BRIDLINGTON SOUTH (08100)", "CAYTON BAY (07500)",
  "DANES DYKE - FLAMBOROUGH (07950)", "FILEY (07600)", "FLAMBOROUGH SOUTH LANDING (07900)", "FRAISTHORPE (08300)",
  "HORNSEA (08700)", "REIGHTON (07700)", "ROBIN HOODS BAY (07200)", "RUNSWICK BAY (06900)", "SANDSEND (07000)",
  "SCARBOROUGH NORTH BAY (07300)", "SCARBOROUGH SOUTH BAY (07400)", "WHITBY (07100)", "WILSTHORPE (08200)",
  "WITHERNSEA (08900)", "WHARFE AT WILDERNESS CAR PARK (08904)", "WHARFE AT CROMWHEEL - ILKLEY (08901)",
  "NIDD AT THE LIDO - KNARESBOROUGH (08903)"};

  updateFileSelector(pollutant, litterList);
  updateFileSelector(location, locationList);

  
  QBarSeries *litterSeries = new QBarSeries();
  QBarSet *set1 = new QBarSet("Bathing Water Profile : Other Litter (incl. plastics)");
  double set1List[] = {0.2, 0.625, 0, 0.26, 0.15, 0, 0.86, 0.16, 0.11, 0.17, 0.15, 0.5, 0.2, 0.37, 0.25, 0.48, 0.5, 1.73, 0.71, 0.65};
  for (int i = 0; i < 20; i++){
    set1->append(set1List[i]);
  }

  QBarSet *set2 = new QBarSet("Sewage debris");
  double set2List[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.05, 0, 0, 0, 0, 0};
  for (int i = 0; i < 20; i++){
    set2->append(set2List[i]);
  }

  QBarSet *set3 = new QBarSet("Tarry residues");
  double set3List[] = {0.14, 0.74, 0, 0, 0, 0.053, 0.4, 0.32, 0, 0, 0, 0.045, 0, 0, 0, 0.71, 0.053, 0, 0, 0};
  for (int i = 0; i < 20; i++){
    set3->append(set3List[i]);
  }

  litterSeries->append(set1);
  litterSeries->append(set2);
  litterSeries->append(set3);

  QChart *litterChart = new QChart;
  litterChart->addSeries(litterSeries);
  litterChart->setTitle("Average presence of litters in various locations");
  litterChart->setAnimationOptions(QChart::SeriesAnimations);

  QStringList locationName;
  for (int i = 0; i < 20; i++){
    locationName.append(locationList);
  }

  QLineSeries *redLine = new QLineSeries();
  redLine->append(0, 1.0);
  redLine->append(20, 1.0);
  QPen redPen(Qt::red);
  redPen.setWidth(3);
  redLine->setPen(redPen);
  litterChart->addSeries(redLine);
  
  QLineSeries *yellowLine = new QLineSeries();
  yellowLine->append(0, 0.5);
  yellowLine->append(20, 0.5);
  QPen yellowPen(Qt::yellow);
  yellowPen.setWidth(3);
  yellowLine->setPen(yellowPen);
  litterChart->addSeries(yellowLine);

  QLineSeries *greenLine = new QLineSeries();
  greenLine->append(0, 0.25);
  greenLine->append(20, 0.25);
  QPen greenPen(Qt::green);
  greenPen.setWidth(3);
  greenLine->setPen(greenPen);
  litterChart->addSeries(greenLine);

  QBarCategoryAxis *axisX = new QBarCategoryAxis();
  axisX->append(locationName);
  axisX->setTitleText("Locations");
  litterChart->addAxis(axisX, Qt::AlignBottom);
  litterSeries->attachAxis(axisX);

  QValueAxis *axisY = new QValueAxis();
  axisY->setRange(0, 2);
  axisY->setTitleText("garber c");
  litterChart->addAxis(axisY, Qt::AlignLeft);
  litterSeries->attachAxis(axisY);

  litterChart->legend()->setVisible(true);
  litterChart->legend()->setAlignment(Qt::AlignBottom);
  litterChart->legend()->markers(redLine)[0]->setLabel("1.0 (Exceeding safe levels)");
  litterChart->legend()->markers(yellowLine)[0]->setLabel("0.5 (Levels for concern)");
  litterChart->legend()->markers(greenLine)[0]->setLabel("0.25 (Safe levels)");
  litterChart->setVisible(true);

  redLine->attachAxis(axisX);
  redLine->attachAxis(axisY);

  yellowLine->attachAxis(axisX);
  yellowLine->attachAxis(axisY);
  
  greenLine->attachAxis(axisX);
  greenLine->attachAxis(axisY);
  
  
  litterSeries->setName("Types of litter");
  
  QChartView *litterChartView = new QChartView(litterChart);
  litterChartView->setRenderHint(QPainter::Antialiasing);
  litterChartView->setVisible(true);

  QVBoxLayout *litterLayout = new QVBoxLayout();
  litterLayout->addWidget(litterChartView);
  litter = new QWidget();
  litter->setLayout(litterLayout);
  setCentralWidget(litter);
}

void WaterWindow::createFlourinated()
{
  setCentralWidget(0);
  //  Create a container widget for this page
  QWidget *flourinatedWidget = new QWidget();
  QVBoxLayout *layout = new QVBoxLayout();

  QLabel *pfaLabel = new QLabel("label");
  QLabel *locationLabel = new QLabel("location");
  QFrame *complianceBar = new QFrame();

  complianceBar->setFrameShape(QFrame::HLine);
  complianceBar->setFixedHeight(10);

  layout->addWidget(pfaLabel);
  layout->addWidget(locationLabel);
  layout->addWidget(complianceBar);

  FlourineChart *fchart = new FlourineChart();
  if (model.hasData())
  {
    fchart->loadDataset(model.getData());
  }

  // Create file selectors specific to this window

  updateFileSelector(pollutant, fchart->getDeterminands());
  updateFileSelector(location, fchart->getLocations(pollutant->currentText().toStdString()));

  // Create a chart and chart view
  fchart->loadDataset(model.getData());
  QChart *chart = new QChart();
  fchart->initChart(chart);
  std::cout << "chart created" << std::endl;
  QChartView *chartView = new QChartView(chart);
  layout->addWidget(chartView);

  // Connect selectors to a slot for updating the chart
  connect(pollutant, &QComboBox::currentTextChanged, this, [=]()
          { fchart->updateChart(chart, pollutant->currentText().toStdString());
            updateFileSelector(location, fchart->getLocations(pollutant->currentText().toStdString()));
            fchart->updateCompliance(pfaLabel, locationLabel, complianceBar, pollutant->currentText().toStdString(), 
                              location->currentText().toStdString()); });
  connect(location, &QComboBox::currentTextChanged, this, [=]()
          { fchart->updateCompliance(pfaLabel, locationLabel, complianceBar, pollutant->currentText().toStdString(),
                                     location->currentText().toStdString()); });
  chartView->setChart(chart);
  // Set the layout and widget as the central widget
  flourinatedWidget->setLayout(layout);
  setCentralWidget(flourinatedWidget);
  
}

void WaterWindow::createCompliance()
{

}

void WaterWindow::createPageBar()
{
  QToolBar *pageBar = new QToolBar();
  pageBar->addWidget(dashboardButton);
  pageBar->addSeparator();
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

  QStringList locationOptions;
  locationOptions << "locations";
  location = new QComboBox();
  location->addItems(locationOptions);
}

void WaterWindow::updateFileSelector(QComboBox *selector, QStringList options)
{
  selector->clear();
  selector->addItems(options);
}

void WaterWindow::createButtons()
{
  loadButton = new QPushButton("Load");
  statsButton = new QPushButton("Stats");
  dashboardButton = new QPushButton("Dashboard");
  overviewButton = new QPushButton("Overview");
  popsButton = new QPushButton("POPs");
  litterButton = new QPushButton("Litter Indicators");
  flourinatedButton = new QPushButton("Flourinated compounds");
  complianceButton = new QPushButton("Compliance Dashboard");

  connect(loadButton, SIGNAL(clicked()), this, SLOT(openCSV()));
  connect(statsButton, SIGNAL(clicked()), this, SLOT(displayStats()));
  connect(dashboardButton, SIGNAL(clicked()), this, SLOT(createDashboard()));
  connect(overviewButton, SIGNAL(clicked()), this, SLOT(createOverview()));
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

  QLabel *locationLabel = new QLabel("location");
  locationLabel->setAlignment(Qt::AlignVCenter);
  toolBar->addWidget(locationLabel);
  toolBar->addWidget(location);

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
  showDataLoadedQuestion();
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
