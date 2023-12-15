#pragma once

#include <QStyledItemDelegate>
namespace HoldingRegisters {

class ItemDelegate : public QStyledItemDelegate {
    // Q_OBJECT
public:
    ItemDelegate(QWidget* parent);
    ~ItemDelegate();

    void setModelData(QWidget* editor, QAbstractItemModel* /*model*/, const QModelIndex& index) const override;
    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& /*option*/, const QModelIndex& index) const override;
    void setEditorData(QWidget* editor, const QModelIndex& index) const override;
};

} // namespace HoldingRegisters
