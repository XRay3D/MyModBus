#include "coils_model.h"
#include "EnumHelper.h"
#include "interface.h"

namespace Coils {
Model::Model(QObject* parent)
    : QAbstractTableModel(parent)
    , m_data {
        { Data::COIL_00, false, Data::ReadWrite },
        { Data::COIL_01, false, Data::ReadWrite },
        { Data::COIL_02, false, Data::ReadWrite },
        { Data::COIL_03, false, Data::ReadWrite },
        { Data::COIL_04, false, Data::ReadWrite },
        { Data::COIL_05, false, Data::ReadWrite },
        { Data::COIL_06, false, Data::ReadWrite },
        { Data::COIL_07, false, Data::ReadWrite },
        { Data::COIL_08, false, Data::ReadWrite },
        { Data::COIL_09, false, Data::ReadWrite },
        { Data::COIL_10, false, Data::ReadWrite },
        //        { Data::COIL_11, false, Data::ReadWrite },
        //        { Data::COIL_12, false, Data::ReadWrite },
        //        { Data::COIL_13, false, Data::ReadWrite },
        //        { Data::COIL_14, false, Data::ReadWrite },
    }
{
}

QVariant Model::data(const QModelIndex& index, int role) const
{
    auto& data = m_data[index.row()];
    if (role == Qt::DisplayRole) {
        switch (index.column()) {
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
    } else if (role == Qt::UserRole) {
        switch (index.column()) {
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
    } else if (role == Qt::EditRole && index.column() == Value)
        return data.data; //        return std::visit([](auto&& arg) { return QVariant::fromValue(arg); }, data.data);
    else if (role == Qt::TextAlignmentRole && index.column())
        return Qt::AlignCenter;
    return {};
}

bool Model::setData(const QModelIndex& index, const QVariant& value, int role)
{
    auto& data = m_data[index.row()];
    qDebug() << sender();
    if (role == Qt::EditRole && index.column() == Value) {
        if (I::mymodbus()->writeSingleCoil(data.id, value.toUInt())) {
            data.data = value.toUInt();
            return true;
        } else {
            data.error = I::mymodbus()->errorString();
            return {};
        }
    }
    if (role == Qt::UserRole && index.column() == Value) {
        data.data = value.toUInt();
        return true;
    } else if (role == Qt::EditRole && index.column() == Error) {
        data.error = value.toString();
        return true;
    }
    return {};
}

QVariant Model::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
        return EnumHelper::toString(Columns(section));
    else if (role == Qt::TextAlignmentRole)
        return Qt::AlignCenter;
    return QAbstractTableModel::headerData(section, orientation, role);
}

Qt::ItemFlags Model::flags(const QModelIndex& index) const
{
    auto& data = m_data[index.row()];
    Qt::ItemFlags flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;
    if (index.column() == Value && data.acces == Data::ReadWrite)
        flags |= Qt::ItemIsEditable;
    return flags;
}
};
