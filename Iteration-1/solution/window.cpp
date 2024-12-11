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
#include <QMessageBox>

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
  setWindowTitle(tr("Water Tool"));
}

void WaterWindow::createDashboard()
{
  setCentralWidget(0);

  QWidget *dashboardWidget = new QWidget();
 
  QVBoxLayout *mainLayout = new QVBoxLayout(dashboardWidget);

  // Header
  QLabel *appTitle = new QLabel(tr("Water Quality Monitor"));
  appTitle->setAlignment(Qt::AlignCenter);
  appTitle->setStyleSheet("font-size: 24px; font-weight: bold;");

  // Pollutant Cards
  QGridLayout *cardLayout = new QGridLayout;

  QStringList categories = {tr("Persistant Organic Pollutants (POPs)"), tr("Flourinated Compounds")};
  QStringList summaries = {tr("Data on PCBs and other persistant organnic pollutants due to their long-lasting impact on the environment and health."), tr("Levels of PFAs and other fluorinated compounds, which are monitored for their environmental persistance.")};

  for (int i = 0; i < categories.size(); ++i) {
    QFrame *card = new QFrame;
    card->setFrameShape(QFrame::Box);
    card->setStyleSheet("border: 2px solid gray; border-radius: 5px;");

    QVBoxLayout *cardContent = new QVBoxLayout(card);

    QLabel *title = new QLabel(categories[i]);
    title->setStyleSheet("font-weight: bold; font-size: 18px;");

    QLabel *pageSummaries = new QLabel(summaries[i]);
    pageSummaries->setStyleSheet("font-size: 14px;");

    QPushButton *pageButton = new QPushButton(tr("View Page"));

    cardContent->addWidget(title);
    cardContent->addWidget(pageSummaries);
    cardContent->addWidget(pageButton);
    cardLayout->addWidget(card, i / 2, i % 2);

    if (categories[i] == "Persistant Organic Pollutants (POPs)") {
      connect(pageButton, SIGNAL(clicked()), this, SLOT(createPOPs()));
    } else if (categories[i] == "Flourinated Compounds") {
      connect(pageButton, SIGNAL(clicked()), this, SLOT(createFlourinated()));
    }
  }

    // Assemble Main Layout
    mainLayout->addWidget(appTitle);
    mainLayout->addLayout(cardLayout);

    setCentralWidget(dashboardWidget);

}

void WaterWindow::createTableAndModel()
{
  setCentralWidget(0);

  table = new QTableView();
  table->setModel(&model);

  QFont tableFont = QFontDatabase::systemFont(QFontDatabase::FixedFont);
  table->setFont(tableFont);

  setCentralWidget(table);
}

void WaterWindow::createPOPs()
{
  setCentralWidget(0);

  QWidget *popWidget = new QWidget();
  QVBoxLayout *layout = new QVBoxLayout();

  POPChart *pchart = new POPChart();
  if (model.hasData())
  {
    pchart->loadDataset(model.getData());
  }

  // Create file selectors specific to this window

  updateFileSelector(pollutant, pchart->getDeterminands());
  updateFileSelector(location, pchart->getLocations(pollutant->currentText().toStdString()));


  // Create a chart and chart view
  pchart->loadDataset(model.getData());
  QChart *chart = new QChart();
  pchart->initChart(chart);
  std::cout << "chart created" << std::endl;
  QChartView *chartView = new QChartView(chart);
  layout->addWidget(chartView);

  // Connect selectors to a slot for updating the chart
  connect(pollutant, &QComboBox::currentTextChanged, this, [=]()
          { pchart->updateChart(chart, pollutant->currentText().toStdString());
            updateFileSelector(location, pchart->getLocations(pollutant->currentText().toStdString())); });
  connect(location, &QComboBox::currentTextChanged, this, [=]()
          { std::cout << "location changed" << std::endl; });
  chartView->setChart(chart);
  // Set the layout and widget as the central widget
  popWidget->setLayout(layout);
  setCentralWidget(popWidget);
}

void WaterWindow::createFlourinated()
{
  setCentralWidget(0);
  //  Create a container widget for this page
  QWidget *flourinatedWidget = new QWidget();
  QVBoxLayout *layout = new QVBoxLayout();

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
            updateFileSelector(location, fchart->getLocations(pollutant->currentText().toStdString())); });
  connect(location, &QComboBox::currentTextChanged, this, [=]()
          { std::cout << "location changed" << std::endl; });
  chartView->setChart(chart);
  // Set the layout and widget as the central widget
  flourinatedWidget->setLayout(layout);
  setCentralWidget(flourinatedWidget);
  
}

void WaterWindow::createPageBar()
{
  QToolBar *pageBar = new QToolBar();
  pageBar->addWidget(dashboardButton);
  pageBar->addSeparator();
  pageBar->addWidget(popsButton);
  pageBar->addWidget(flourinatedButton);

  addToolBar(Qt::TopToolBarArea, pageBar);
}

// change this
void WaterWindow::createFileSelectors()
{
  QStringList pollutantOptions;
  pollutantOptions << tr("pollutants");
  pollutant = new QComboBox();
  pollutant->addItems(pollutantOptions);

  QStringList locationOptions;
  locationOptions << tr("locations");
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
  loadButton = new QPushButton(tr("Load"));
  statsButton = new QPushButton(tr("Stats"));
  dashboardButton = new QPushButton(tr("Dashboard"));
  popsButton = new QPushButton(tr("POPs"));
  flourinatedButton = new QPushButton(tr("Flourinated compounds"));

  connect(loadButton, SIGNAL(clicked()), this, SLOT(openCSV()));
  connect(statsButton, SIGNAL(clicked()), this, SLOT(displayStats()));
  connect(dashboardButton, SIGNAL(clicked()), this, SLOT(createDashboard()));
  connect(popsButton, SIGNAL(clicked()), this, SLOT(createPOPs()));
  connect(flourinatedButton, SIGNAL(clicked()), this, SLOT(createFlourinated()));
}

void WaterWindow::createToolBar()
{
  QToolBar *toolBar = new QToolBar();

  QLabel *pollutantLabel = new QLabel(tr("Pollutant"));
  pollutantLabel->setAlignment(Qt::AlignVCenter);
  toolBar->addWidget(pollutantLabel);
  toolBar->addWidget(pollutant);

  QLabel *locationLabel = new QLabel(tr("Location"));
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
  fileInfo = new QLabel(tr("Current file: <none>"));
  QStatusBar *status = statusBar();
  status->addWidget(fileInfo);
}

void WaterWindow::addFileMenu()
{
  QAction *locAction = new QAction(tr("Set Data &Location"), this);
  locAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_L));
  connect(locAction, SIGNAL(triggered()), this, SLOT(setDataLocation()));

  QAction *closeAction = new QAction(tr("Quit"), this);
  closeAction->setShortcut(QKeySequence::Close);
  connect(closeAction, SIGNAL(triggered()), this, SLOT(close()));

  QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
  fileMenu->addAction(locAction);
  fileMenu->addAction(closeAction);
}

void WaterWindow::addHelpMenu()
{
  QAction *aboutAction = new QAction(tr("&About"), this);
  connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));

  QAction *aboutQtAction = new QAction(tr("About &Qt"), this);
  connect(aboutQtAction, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

  QMenu *helpMenu = menuBar()->addMenu(tr("&Help"));
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
  createTableAndModel();
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
    QMessageBox::critical(this, tr("CSV File Error"), error.what());
    return;
  }

  fileInfo->setText(QString(tr("Current file: <kbd>%1</kbd>")).arg(filename));
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