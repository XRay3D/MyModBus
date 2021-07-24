#include "hldreg_model.h"
#include "EnumHelper.h"

namespace HoldingRegisters {

Model::Model(QObject* parent)
    : QAbstractTableModel(parent)
    , m_data {
        Data { Data::REG_01, uint16_t {}, Data::ReadWrite },
        Data { Data::REG_02, uint16_t {}, Data::ReadWrite },
        Data { Data::REG_03, uint16_t {}, Data::ReadWrite },
        Data { Data::REG_04, uint16_t {}, Data::ReadWrite },
        Data { Data::REG_05, uint16_t {}, Data::ReadWrite },
        Data { Data::REG_06, uint16_t {}, Data::ReadWrite },
        Data { Data::REG_07, uint16_t {}, Data::ReadWrite },
        Data { Data::REG_08, uint16_t {}, Data::ReadWrite },
        Data { Data::REG_09, uint16_t {}, Data::ReadWrite },
        Data { Data::REG_10, uint16_t {}, Data::ReadWrite },
        Data { Data::REG_11, uint16_t {}, Data::ReadWrite },
        Data { Data::REG_12, uint16_t {}, Data::ReadWrite },
        Data { Data::REG_13, uint16_t {}, Data::ReadWrite },
        Data { Data::REG_14, uint16_t {}, Data::ReadWrite },
        Data { Data::REG_15, uint16_t {}, Data::ReadWrite },
        Data { Data::REG_16, uint16_t {}, Data::ReadWrite },
        Data { Data::REG_17, uint16_t {}, Data::ReadWrite },
        Data { Data::REG_18, float {}, Data::ReadWrite },
        //        Data { Data::REG_19, float {}, Data::ReadWrite },
    } //
{
    auto typeName = []<size_t... Is>(std::index_sequence<Is...>)->QStringList
    {
        QStringList list;
        ((list << QString(typeid(std::get<Is>(Data::variant {})).name())), ...);
        return list << QString("All");
    };
    m_type = typeName(std::make_index_sequence<std::variant_size_v<HoldingRegisters::Data::variant>> {});
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
            return std::visit([](auto&& arg) { return typeid(std::decay_t<decltype(arg)>).name(); }, data.data);
        case Acces:
            return EnumHelper::toString(data.acces);
        case Value:
            return data.toVariant();
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
            return data.data.index();
        case Acces:
            return data.acces;
        case Value:
            return QVariant::fromValue(&data);
        case Error:
            return data.error;
        }
    } else if (role == Qt::EditRole && index.column() == Value)
        return std::visit([](auto&& arg) { return QVariant::fromValue(arg); }, data.data);
    else if (role == Qt::TextAlignmentRole && index.column())
        return Qt::AlignCenter;

    return {};
}

bool Model::setData(const QModelIndex& index, const QVariant& value, int role)
{
    auto& data = m_data[index.row()];
    if (role == Qt::EditRole && index.column() == Value) {
        data.fromVariant(value);
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

const Data::variant& Model::data(uint row) const { return m_data[row].data; }

Data::variant& Model::data(uint row) { return m_data[row].data; }

Data::ID Model::id(uint row) const { return m_data[row].id; }

const QStringList& Model::type() const { return m_type; }

const QStringList& Model::acces() const { return m_acces; }

} // namespace Reg
