#pragma once

#include "inreg_data.h"

#include <QAbstractTableModel>
template <size_t I>
struct key {
};

namespace InputRegisters {

class Model final : public QAbstractTableModel {
    Q_OBJECT

    std::vector<Data> m_data;

    Q_PROPERTY(QStringList acces READ acces CONSTANT)
    Q_PROPERTY(QStringList type READ type CONSTANT)

public:
    explicit Model(QObject* parent = nullptr);
    enum Columns {
        Name,
        Id,
        Type,
        Acces,
        Value,
        Error,
        ColumnCount
    };
    Q_ENUM(Columns);

    // QAbstractItemModel interface
    int rowCount(const QModelIndex& = {}) const override { return m_data.size(); }
    int columnCount(const QModelIndex& = {}) const override { return ColumnCount; }
    QVariant data(const QModelIndex& index, int role) const override;
    bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;

    Q_INVOKABLE QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    Qt::ItemFlags flags(const QModelIndex& index) const override;

    Data::variant& data(uint row);
    const Data::variant& data(uint row) const;
    Data::ID id(uint row) const;

    QStringList m_acces { "ReadOnly", "ReadWrite", "All" };
    QStringList m_type;
    const QStringList& acces() const;
    const QStringList& type() const;
};

inline Data::variant& toRVariant(const QModelIndex& index)
{
    return index.siblingAtColumn(Model::Value).data(Qt::UserRole).value<Data*>()->data;
}

} // namespace Reg
