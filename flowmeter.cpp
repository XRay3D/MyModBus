#include "flowmeter.h"
#include "EnumHelper.h"
#include "interface.h"
#include "regitemdelegate.h"
#include "regmodel.h"
#include "ui_flowmeter.h"
#include <QFile>
#include <QRadioButton>
#include <QtSerialPort>
#include <ranges>

namespace rng = std::ranges;

Flowmeter::Flowmeter(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::Flowmeter) {
    ui->setupUi(this);

    initCbxPorts();

    initTableView();

    loadSettings();
    // on_pbReadAll_clicked();
    // regDataGen();
}

Flowmeter::~Flowmeter() {

    saveSettings();
    delete ui;
}

void Flowmeter::loadSettings() {
    QSettings settings;
    settings.beginGroup("MainWindow");
    restoreGeometry(settings.value("Geometry").toByteArray());
    restoreState(settings.value("State").toByteArray());
    ui->cbxPorts->setCurrentText(settings.value("cbxPorts").toString());

    auto radioButtons = ui->grbxAcces->findChildren<QRadioButton*>();
    radioButtons[settings.value("radioButtonAcces", radioButtons.size() - 1).toInt()]->setChecked(true);

    radioButtons = ui->grbxType->findChildren<QRadioButton*>();
    radioButtons[settings.value("radioButtonType", radioButtons.size() - 1).toInt()]->setChecked(true);
    settings.endGroup();
}

void Flowmeter::saveSettings() {
    QSettings settings;
    settings.beginGroup("MainWindow");
    settings.setValue("Geometry", saveGeometry());
    settings.setValue("State", saveState());
    settings.setValue("cbxPorts", ui->cbxPorts->currentText());
    int rbCtr{};
    for(auto rb : ui->grbxAcces->findChildren<QRadioButton*>()) {
        if(rb->isChecked()) {
            settings.setValue("radioButtonAcces", rbCtr);
            break;
        }
        ++rbCtr;
    }
    rbCtr = {};
    for(auto rb : ui->grbxType->findChildren<QRadioButton*>()) {
        if(rb->isChecked()) {
            settings.setValue("radioButtonType", rbCtr);
            break;
        }
        ++rbCtr;
    }
    settings.endGroup();
}

void Flowmeter::initCbxPorts() {
    auto spInfos{QSerialPortInfo::availablePorts().toVector()};
    rng::sort(spInfos, {}, [](const QSerialPortInfo& pi) { return pi.portName().midRef(3).toUInt(); });
    for(auto&& pi : spInfos)
        ui->cbxPorts->addItem(pi.portName());
    connect(ui->cbxPorts, &QComboBox::currentTextChanged, I::modbus(), &LibModbus::setPortName);
}

void Flowmeter::initTableView() {
    QSortFilterProxyModel* proxyModel = new QSortFilterProxyModel(this);
    proxyModel->setSourceModel(m_model = new Reg::Model(this));
    proxyModel->setDynamicSortFilter(true);
    ui->tableView->setModel(proxyModel);

    //        ui->tableView->setSortingEnabled(true);
    ui->tableView->sortByColumn(Reg::Model::Acces);
    ui->tableView->setItemDelegateForColumn(Reg::Model::Value, new Reg::ItemDelegate(this));

    ui->tableView->verticalHeader()->setDefaultSectionSize(QFontMetrics(font()).height());
    ui->tableView->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);

    auto horizontalHeader = ui->tableView->horizontalHeader();
    horizontalHeader->setSectionResizeMode(QHeaderView::ResizeToContents);
    horizontalHeader->setSectionResizeMode(Reg::Model::Error, QHeaderView::Stretch);
    horizontalHeader->setSectionResizeMode(Reg::Model::Value, QHeaderView::Stretch);
    horizontalHeader->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(horizontalHeader, &QWidget::customContextMenuRequested, [horizontalHeader, this](const QPoint& pos) {
        qDebug() << pos << horizontalHeader->logicalIndexAt(pos);
        QMenu menu;

        auto addAct = [&menu, this](int column, uint id, QString name) {
            auto action = menu.addAction(name);
            action->setCheckable(true);
            for(int row = 0; row < m_model->rowCount(); ++row) {
                auto index = ui->tableView->model()->index(row, column);
                if(index.data(Qt::UserRole).toUInt() == id) {
                    action->setChecked(!ui->tableView->isRowHidden(row));
                    break;
                }
            }
            connect(action, &QAction::toggled, [column, id, this](bool togled) {
                for(int row = 0; row < m_model->rowCount(); ++row) {
                    auto index = ui->tableView->model()->index(row, column);
                    if(index.data(Qt::UserRole).toUInt() == id)
                        togled ? ui->tableView->showRow(row) : ui->tableView->hideRow(row);
                }
            });
        };

        if(horizontalHeader->logicalIndexAt(pos) == Reg::Model::Type) {
            auto typeOf = [&]<size_t... Is>(std::index_sequence<Is...>) { // бежим по типам
                auto addAct2 = [&]<size_t I>(key<I>) {
                    addAct(Reg::Model::Type, I, typeid(std::get<I>(Reg::Data::variant{})).name());
                };
                (addAct2(key<Is>{}), ...);
            };
            typeOf(std::make_index_sequence<std::variant_size_v<Reg::Data::variant>>{});
            menu.exec(horizontalHeader->mapToGlobal(pos));
        }
        if(horizontalHeader->logicalIndexAt(pos) == Reg::Model::Acces) {
            addAct(Reg::Model::Acces, Reg::Data::ReadOnly, EnumHelper::toString(Reg::Data::ReadOnly));
            addAct(Reg::Model::Acces, Reg::Data::ReadWrite, EnumHelper::toString(Reg::Data::ReadWrite));
            menu.exec(horizontalHeader->mapToGlobal(pos));
        }
    });

    { // radiobuttons Type
        auto typeOf = [this]<size_t... Is>(std::index_sequence<Is...>) {
            auto addRadioButton = [this]<size_t I>(key<I>) {
                auto radioButton = new QRadioButton(typeid(std::get<I>(Reg::Data::variant{})).name(), this);
                ui->vlayType->addWidget(radioButton);
                connect(radioButton, &QRadioButton::toggled, [id = I, this](bool togled) {
                    for(int row = 0; row < m_model->rowCount(); ++row) {
                        auto index = ui->tableView->model()->index(row, Reg::Model::Type);
                        if(index.data(Qt::UserRole).toUInt() == id)
                            togled ? ui->tableView->showRow(row)
                                   : ui->tableView->hideRow(row);
                        else
                            ui->tableView->hideRow(row);
                    }
                });
            };
            (addRadioButton(key<Is>{}), ...);
            auto radioButton = new QRadioButton("All", this);
            ui->vlayType->addWidget(radioButton);
            connect(radioButton, &QRadioButton::toggled, [this](bool togled) {
                if(togled)
                    for(int row = 0; row < m_model->rowCount(); ++row)
                        ui->tableView->showRow(row);
            });
        };
        typeOf(std::make_index_sequence<std::variant_size_v<Reg::Data::variant>>{});
    }
    { // radiobuttons Acces
        auto addRadioButton = [this](auto E) {
            auto radioButton = new QRadioButton(EnumHelper::toString(E), this);
            ui->vlayAcces->addWidget(radioButton);
            connect(radioButton, &QRadioButton::toggled, [id = E, this](bool togled) {
                for(int row = 0; row < m_model->rowCount(); ++row) {
                    auto index = ui->tableView->model()->index(row, Reg::Model::Acces);
                    if(index.data(Qt::UserRole).toUInt() == id)
                        togled ? ui->tableView->showRow(row)
                               : ui->tableView->hideRow(row);
                    else
                        ui->tableView->hideRow(row);
                }
            });
        };
        addRadioButton(Reg::Data::ReadOnly);
        addRadioButton(Reg::Data::ReadWrite);

        auto radioButton = new QRadioButton("All", this);
        ui->vlayAcces->addWidget(radioButton);
        connect(radioButton, &QRadioButton::toggled, [this](bool togled) {
            if(togled)
                for(int row = 0; row < m_model->rowCount(); ++row)
                    ui->tableView->showRow(row);
        });
    }
}

void Flowmeter::read(QModelIndex&& index) {
    if(!index.isValid())
        return;
    auto& var = Reg::toRVariant(index);
    ui->tableView->selectRow(index.row());
    auto visitor = [this, &index](auto&& arg) {
        using T = std::decay_t<decltype(arg)>;
        auto model = ui->tableView->model();
        bool result{};
        auto reg = index.siblingAtColumn(Reg::Model::Id).data(Qt::UserRole).toUInt();
        /**/ if constexpr(std::is_same_v<T, Reg::Text>)
            result = I::modbus()->readRegisters(reg, arg, LibModbus::DataN{}); //USonicFlowmeter
        else if constexpr(std::is_same_v<T, Reg::SetLinaCoef>)
            result = I::modbus()->readRegisters(reg, arg, LibModbus::DataS{});
        else
            result = I::modbus()->readRegisters(reg, arg, LibModbus::DataSR{});

        if(result) {
            model->setData(index.siblingAtColumn(Reg::Model::Error), "");
            model->setData(index.siblingAtColumn(Reg::Model::Value), QVariant::fromValue(arg));
            emit model->dataChanged(index.siblingAtColumn(Reg::Model::Value), index.siblingAtColumn(Reg::Model::Error), {Qt::DisplayRole});
        } else {
            index = index.siblingAtColumn(Reg::Model::Error);
            model->setData(index, I::modbus()->errorString());
            emit model->dataChanged(index, index, {Qt::DisplayRole});
        }
    };
    std::visit(visitor, var);
}

Reg::Model* Flowmeter::model() const { return m_model; }

void Flowmeter::on_pbReadAll_clicked() {
    for(int row = 0; row < m_model->rowCount(); ++row)
        if(!ui->tableView->isRowHidden(row))
            read(ui->tableView->model()->index(row, Reg::Model::Value));
}

void Flowmeter::on_pbReadSelected_clicked() {
    for(auto&& index : ui->tableView->selectionModel()->selectedRows())
        read(index.siblingAtColumn(Reg::Model::Value));
}

void Flowmeter::regDataGen() {
    QMap<QString, QVector<QString>> reg;

    if(QFile file("../DataBase.cpp"); file.open(QIODevice::ReadOnly)) {
        while(!file.atEnd()) {
            QString str = file.readLine();
            static const QRegExp rx("^    ([A-Z0-9_]+) = (0x\\S{4}),");
            if(rx.indexIn(str) > -1) {
                reg[rx.cap(1)] << rx.cap(1) << ("0x" + rx.cap(2));
            }
        }
    } else
        qDebug() << file.errorString();

    if(QFile file("../DataBase.cpp"); file.open(QIODevice::ReadOnly)) {
        QString key;
        while(!file.atEnd()) {
            QString str = file.readLine();
            static const QRegExp rx("^    case ([A-Z0-9_]+)\\:");
            if(rx.indexIn(str) > -1) {
                if(reg.contains(key) && reg[key].size() < 3)
                    reg[key] << "";
                key = rx.cap(1);
            }
            static const QRegExp rx2("^        size = sizeof\\((\\S+)\\)\\;");
            if(rx2.indexIn(str) > -1) {
                if(reg.contains(key))
                    reg[key] << rx2.cap(1) + "{}";
                key.clear();
            }
            static const QRegExp rx3("^        size = (\\S+)\\;");
            if(rx3.indexIn(str) > -1) {
                if(reg.contains(key))
                    reg[key] << rx3.cap(1);
                key.clear();
            }
        }
    } else
        qDebug() << file.errorString();

    if(QFile file("../DataBase.cpp"); file.open(QIODevice::ReadOnly)) {
        QString key;
        while(!file.atEnd()) {
            QString str = file.readLine();
            static const QRegExp rx("^    case ([A-Z0-9_]+)\\:");
            if(rx.indexIn(str) > -1) {
                //                if (reg.contains(key) && reg[key].size() < 4)
                //                    reg[key] << "ReadOnly";
                key = rx.cap(1);
            }
            static const QRegExp rx3("^        memcpy\\S*\\(pPar, .+\\);");
            if(rx3.indexIn(str) > -1) {
                if(reg.contains(key))
                    reg[key] << "Read";
                key.clear();
                qDebug() << rx3.capturedTexts();
                continue;
            }

            static const QRegExp rx2("^        memcpy\\(.+, pPar, .+\\);");
            if(rx2.indexIn(str) > -1) {
                if(reg.contains(key))
                    reg[key].last() += "Write";
                key.clear();
                qDebug() << rx2.capturedTexts();
            }
        }
    } else
        qDebug() << file.errorString();

    for(auto& var : reg) {
        if(var.size() < 3)
            continue;
        QString str{"RegData{"};
        for(auto& s : var) {
            str += s + ", ";
        }
        str += "},";
        qDebug() << str.toLocal8Bit().data();
    }
}
