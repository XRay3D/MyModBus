//#include "inreg_delegate.h"
//#include "inreg_model.h"
//#include "interface.h"
//#include <QDoubleSpinBox>
//#include <QLineEdit>
//#include <QSpinBox>

//namespace InputRegisters {

//ItemDelegate::ItemDelegate(QWidget* parent)
//    : QStyledItemDelegate(parent)
//{
//}

//ItemDelegate::~ItemDelegate() { }

//QWidget* ItemDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& style, const QModelIndex& index) const
//{
//    return index.data(Qt::UserRole).value<InputRegisters::Data*>()->createEditor(parent, style);
//}

//void ItemDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
//{
//    return index.data(Qt::UserRole).value<InputRegisters::Data*>()->setEditorData(editor);
//}

//void ItemDelegate::setModelData(QWidget* editor, QAbstractItemModel*, const QModelIndex& index) const
//{
//    return index.data(Qt::UserRole).value<InputRegisters::Data*>()->setModelData(editor);
//}

//} // namespace Reg
