#pragma once

#include <QAbstractTableModel>

namespace DiscreteInputs {

struct Data {
    enum Acces {
        ReadOnly,
        ReadWrite
    };
    Q_ENUM(Acces)
    enum { Offset = 1 };
    enum ID : uint16_t {
        DI_00,
        DI_01,
        DI_02,
        DI_03,
        DI_04,
        DI_05,
        DI_06,
        DI_07,
        DI_08,
        DI_09,
        DI_10,
        DI_11,
        DI_12,
        DI_13,
        DI_14,
    };
    Q_ENUM(ID)

    ID id;
    bool data {};
    const Acces acces = ReadOnly;
    QString error {};

    //    QVariant toVariant() const;
    //    void fromVariant(const QVariant& value);
    //    void setModelData(QWidget* editor);
    //    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& style) const;
    //    void setEditorData(QWidget* editor) const;

    Q_GADGET
};

class Model final : public QAbstractTableModel {
    Q_OBJECT

    std::vector<Data> m_data;

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
    bool setData(const QModelIndex& index, const QVariant& value, int role) override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;
};
}

Q_DECLARE_METATYPE(DiscreteInputs::Data)
