// COMP2811 Coursework 2: data model

#include "model.hpp"
// this is fine don't change
void WaterModel::updateFromFile(const QString &filename)
{
  beginResetModel();
  dataset.loadData(filename.toStdString());
  endResetModel();
}

// all the attributes (rows) from the dataset
QVariant WaterModel::data(const QModelIndex &index, int role) const
{
  if (!index.isValid())
  {
    return QVariant();
  }

  if (role == Qt::TextAlignmentRole)
  {
    return int(Qt::AlignRight | Qt::AlignVCenter);
  }
  else if (role == Qt::DisplayRole)
  {
    Water w = dataset[index.row()];
    switch (index.column())
    {
    case 0:
      return QVariant(w.getId().c_str());
    case 1:
      return QVariant(w.getResult());
    }
  }

  return QVariant();
}

// nees to show all the data
QVariant WaterModel::headerData(int section, Qt::Orientation orientation, int role) const
{
  if (role != Qt::DisplayRole)
  {
    return QVariant();
  }

  if (orientation == Qt::Vertical)
  {
    return QVariant(section + 1);
  }

  switch (section)
  {
  case 0:
    return QString("id");
  case 1:
    return QString("Latitude");
  default:
    return QVariant();
  }
}
