#include "hldreg_data.h"
#include "interface.h"
#include <QStyleOptionViewItem>
#include <QtWidgets>
#include <ranges>

namespace rng = std::ranges;

class SLCModel : public QAbstractTableModel {
    HoldingRegisters::SetLinaCoef& slc;

    template <class T, size_t N>
    constexpr size_t size(T (&)[N]) const { return N; }

public:
    SLCModel(HoldingRegisters::SetLinaCoef& slc, QWidget* parent)
        : QAbstractTableModel(parent)
        , slc(slc) {
    }
    ~SLCModel() { }

    // QAbstractItemModel interface
    int rowCount(const QModelIndex&) const override { return size(slc.coef); }
    int columnCount(const QModelIndex&) const override { return size(slc.coef[0].f); }
    Qt::ItemFlags flags(const QModelIndex&) const override { return Qt::ItemIsEditable | Qt::ItemIsSelectable | Qt::ItemIsEnabled; }

    QVariant data(const QModelIndex& index, int role) const override {
        if (role == Qt::DisplayRole || role == Qt::EditRole)
            return slc.coef[index.row()].f[index.column()];
        if (role == Qt::TextAlignmentRole)
            return Qt::AlignCenter;
        return {};
    }

    bool setData(const QModelIndex& index, const QVariant& value, int role) override {
        if (role == Qt::EditRole) {
            slc.coef[index.row()].f[index.column()] = value.toFloat();
            return true;
        }
        return {};
    }
};

namespace HoldingRegisters {

QVariant Data::toVariant() const {
    return std::visit([](auto&& arg) {
        using T = std::decay_t<decltype(arg)>;
        /*  */ if constexpr (std::is_same_v<T, Text>) {
            return QVariant::fromValue(QString::fromLocal8Bit(arg.data, sizeof(T)));
        } else if constexpr (std::is_same_v<T, SetLinaCoef>) {
            QString str;
            for (auto& lc: arg) {
                str += "{";
                for (auto& f: lc)
                    str += QString::number(f) + ",";
                str += "},";
            }
            return QVariant::fromValue(str);
        } else {
            return QVariant::fromValue(arg);
        }
    },
        data);
}

void Data::fromVariant(const QVariant& value) {
    std::visit([&value](auto&& arg) {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, Text>) {
            arg = value.value<T>();
        } else if constexpr (std::is_same_v<T, SetLinaCoef>) {
            ////
        } else {
            arg = value.value<T>();
        }
    },
        data);
}

void Data::setModelData(QWidget* editor) {
    qDebug(__FUNCTION__);
    return std::visit([this, editor](auto&& arg) {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, Text>) {
            ////
        } else if constexpr (std::is_same_v<T, SetLinaCoef>) {
            ////
        } else {
            auto sbx = qobject_cast<QDoubleSpinBox*>(editor);
            arg = sbx->value();
        }
        I::mymodbus()->writeHoldingRegisters(id, arg, ByteOrder::ABCD{});
    },
        const_cast<variant&>(data));
}

QWidget* Data::createEditor(QWidget* parent, const QStyleOptionViewItem& /*style*/) const {
    QWidget* w = nullptr;
    switch (data.index()) {
    case 0:
    case 1:
    case 2:
        w = new QDoubleSpinBox(parent);
        break;
    case 3:
        w = new QLineEdit(parent);
        break;
    case 4:
        w = new QTableView(parent);
        break;
    default:;
    }
    return w;
}

void Data::setEditorData(QWidget* editor) const {
    return std::visit([editor](auto&& arg) {
        using T = std::decay_t<decltype(arg)>;
        /*  */ if constexpr (std::is_integral_v<T>) {
            auto sbx = qobject_cast<QDoubleSpinBox*>(editor);
            sbx->setRange(std::numeric_limits<T>::min(), std::numeric_limits<T>::max());
            sbx->setDecimals(0);
            sbx->setAlignment(Qt::AlignCenter);
            sbx->setValue(arg);
            return;
        } else if constexpr (std::is_floating_point_v<T>) {
            auto sbx = qobject_cast<QDoubleSpinBox*>(editor);
            sbx->setRange(-std::numeric_limits<T>::max(), +std::numeric_limits<T>::max());
            sbx->setDecimals(6);
            sbx->setAlignment(Qt::AlignCenter);
            sbx->setValue(arg);
            return;
        } else if constexpr (std::is_same_v<T, Text>) {
            auto sbx = qobject_cast<QLineEdit*>(editor);
            sbx->setAlignment(Qt::AlignCenter);
            sbx->setText(arg.data);
            return;
        } else if constexpr (std::is_same_v<T, SetLinaCoef>) {
            auto tv = qobject_cast<QTableView*>(editor);
            tv->setModel(new SLCModel(arg, editor));
            tv->setAlternatingRowColors(true);
            tv->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
            tv->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
            tv->resize(tv->width(), QFontMetrics(tv->font()).height() * tv->model()->rowCount() * 2);
            return;
        }
    },
        const_cast<variant&>(data));
}

} // namespace HoldingRegisters
