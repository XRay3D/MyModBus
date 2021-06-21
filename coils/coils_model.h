#pragma once

#include <QAbstractTableModel>
namespace Coils {
struct Data {
    enum Acces {
        ReadOnly,
        ReadWrite
    };
    Q_ENUM(Acces)
    enum { Offset = 1 };
    enum ID : uint16_t {
        COIL_00,
        COIL_01,
        COIL_02,
        COIL_03,
        COIL_04,
        COIL_05,
        COIL_06,
        COIL_07,
        COIL_08,
        COIL_09,
        COIL_10,
        COIL_11,
        COIL_12,
        COIL_13,
        COIL_14,
    };
    Q_ENUM(ID)

    ID id;
    bool data;
    Acces acces;
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

Q_DECLARE_METATYPE(Coils::Data)
