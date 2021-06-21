#include "di_model.h"
#include "EnumHelper.h"
#include "interface.h"

#include <QColor>

namespace DiscreteInputs {

Model::Model(QObject* parent)
    : QAbstractTableModel(parent)
    , m_data {
          {Data::DI_00},
          {Data::DI_01},
          {Data::DI_02},
          {Data::DI_03},
          {Data::DI_04},
          {Data::DI_05},
          {Data::DI_06},
          {Data::DI_07},
          {Data::DI_08},
          {Data::DI_09},
          {Data::DI_10},
          //        { Data::DI_11 },
          //        { Data::DI_12 },
          //        { Data::DI_13 },
          //        { Data::DI_14 },
      } {
}

QVariant Model::data(const QModelIndex& index, int role) const {
    auto& data = m_data[index.row()];
    if(role == Qt::DisplayRole) {
        switch(index.column()) {
        case Name:
            return EnumHelper::toString(data.id);
        case Id:
            return "0x" + QString("%1").arg(data.id, 4, 16, QChar('0')).toUpper();
        case Type:
            return typeid(data.data).name();
        case Acces:
            return EnumHelper::toString(data.acces);
        case Value:
            return data.data; //            return data.toVariant();
        case Error:
            return data.error;
        }
    } else if(role == Qt::UserRole) {
        switch(index.column()) {
        case Name:
            return EnumHelper::toString(data.id);
        case Id:
            return data.id;
        case Type:
            return data.data; //            return data.data.index();
        case Acces:
            return data.acces;
        case Value:
            return QVariant::fromValue(&data);
        case Error:
            return data.error;
        }
    } else if(role == Qt::EditRole && index.column() == Value)
        return data.data; //        return std::visit([](auto&& arg) { return QVariant::fromValue(arg); }, data.data);
    else if(role == Qt::TextAlignmentRole && index.column())
        return Qt::AlignCenter;
    else if(role == Qt::BackgroundColorRole && index.column() == Value)
        return data.data ? QColor(0x7F, 0xFF, 0x7F)
                         : QColor(0xFF, 0x7F, 0x7F);
    return {};
}

bool Model::setData(const QModelIndex& index, const QVariant& value, int role) {
    auto& data = m_data[index.row()];
    qDebug() << sender();
    if(role == Qt::EditRole && index.column() == Value) {
        if(I::mymodbus()->writeSingleCoil(data.id, value.toUInt())) {
            data.data = value.toUInt();
            return true;
        } else {
            data.error = I::mymodbus()->errorString();
            return {};
        }
    }
    if(role == Qt::UserRole && index.column() == Value) {
        data.data = value.toUInt();
        return true;
    } else if(role == Qt::EditRole && index.column() == Error) {
        data.error = value.toString();
        return true;
    }
    return {};
}

QVariant Model::headerData(int section, Qt::Orientation orientation, int role) const {
    if(role == Qt::DisplayRole && orientation == Qt::Horizontal)
        return EnumHelper::toString(Columns(section));
    else if(role == Qt::TextAlignmentRole)
        return Qt::AlignCenter;
    return QAbstractTableModel::headerData(section, orientation, role);
}

Qt::ItemFlags Model::flags(const QModelIndex& index) const {
    auto& data = m_data[index.row()];
    Qt::ItemFlags flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;
    if(index.column() == Value && data.acces == Data::ReadWrite)
        flags |= Qt::ItemIsEditable;
    return flags;
}
};
