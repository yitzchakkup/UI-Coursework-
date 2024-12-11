// COMP2811 Coursework 2: application entry point
// fred
#include <QtWidgets>
#include "window.hpp"
#include <QApplication>
#include <QTranslator>
#include <QLocale>

int main(int argc, char *argv[])
{
  QApplication app(argc, argv);

  QTranslator translator;
  if (translator.load(QLocale::system(), "myapp", "_", ":/i18n")) {
    app.installTranslator(&translator);
  }

  WaterWindow window;
  window.show();

  return app.exec();
}
