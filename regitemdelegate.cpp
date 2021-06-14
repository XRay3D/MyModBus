#include "regitemdelegate.h"
#include "interface.h"
#include "regmodel.h"
#include <QDoubleSpinBox>
#include <QLineEdit>
#include <QSpinBox>

namespace Reg {

ItemDelegate::ItemDelegate(QWidget* parent)
    : QStyledItemDelegate(parent) { }

ItemDelegate::~ItemDelegate() { }

QWidget* ItemDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& style, const QModelIndex& index) const {
    return index.data(Qt::UserRole).value<Reg::Data*>()->createEditor(parent, style);
}

void ItemDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const {
    return index.data(Qt::UserRole).value<Reg::Data*>()->setEditorData(editor);
}

void ItemDelegate::setModelData(QWidget* editor, QAbstractItemModel*, const QModelIndex& index) const {
    return index.data(Qt::UserRole).value<Reg::Data*>()->setModelData(editor);
}

} // namespace Reg
