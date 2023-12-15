#include "hldreg_delegate.h"
#include "hldreg_model.h"
#include "interface.h"
#include <QDoubleSpinBox>
#include <QLineEdit>
#include <QSpinBox>

namespace HoldingRegisters {

ItemDelegate::ItemDelegate(QWidget* parent)
    : QStyledItemDelegate(parent) { }

ItemDelegate::~ItemDelegate() { }

QWidget* ItemDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& style, const QModelIndex& index) const {
    return index.data(Qt::UserRole).value<HoldingRegisters::Data*>()->createEditor(parent, style);
}

void ItemDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const {
    return index.data(Qt::UserRole).value<HoldingRegisters::Data*>()->setEditorData(editor);
}

void ItemDelegate::setModelData(QWidget* editor, QAbstractItemModel*, const QModelIndex& index) const {
    return index.data(Qt::UserRole).value<HoldingRegisters::Data*>()->setModelData(editor);
}

} // namespace HoldingRegisters
