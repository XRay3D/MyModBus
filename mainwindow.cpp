#include "mainwindow.h"
#include "EnumHelper.h"
#include "holdingregisters/hldreg_delegate.h"
#include "holdingregisters/hldreg_model.h"
#include "interface.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QRadioButton>
#include <QtSerialPort>
#include <coils/coils_model.h>
#include <discreteinputs/di_model.h>
#include <ranges>

namespace rng = std::ranges;

using LibModbus = MyModbus;

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    ui->setupUi(this);
    initCbxPorts();
    initTableView();
    loadSettings();
    // on_pbReadAll_clicked();
    // regDataGen();
}

MainWindow::~MainWindow() {

    saveSettings();
    delete ui;
}

void MainWindow::loadSettings() {
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

void MainWindow::saveSettings() {
    QSettings settings;
    settings.beginGroup("MainWindow");
    settings.setValue("Geometry", saveGeometry());
    settings.setValue("State", saveState());
    settings.setValue("cbxPorts", ui->cbxPorts->currentText());
    int rbCtr {};
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

void MainWindow::initCbxPorts() {
    auto spInfos {QSerialPortInfo::availablePorts().toVector()};
    rng::sort(spInfos, {}, [](const QSerialPortInfo& pi) { return pi.portName().midRef(3).toUInt(); });
    for(auto&& pi : spInfos)
        ui->cbxPorts->addItem(pi.portName());
    //connect(ui->cbxPorts, &QComboBox::currentTextChanged, I::mymodbus(), &MyModbus::setPortName);
}

void MainWindow::initTableView() {
    { // HoldingRegisters
        QSortFilterProxyModel* proxyModel = new QSortFilterProxyModel(this);
        proxyModel->setSourceModel(m_hrModel = new HoldingRegisters::Model(this));
        proxyModel->setDynamicSortFilter(true);
        ui->tvHoldingRegisters->setModel(proxyModel);

        //        ui->tvHoldingRegisters->setSortingEnabled(true);
        ui->tvHoldingRegisters->sortByColumn(HoldingRegisters::Model::Acces);
        ui->tvHoldingRegisters->setItemDelegateForColumn(HoldingRegisters::Model::Value, new HoldingRegisters::ItemDelegate(this));

        auto verticalHeader = ui->tvHoldingRegisters->verticalHeader();
        verticalHeader->setDefaultSectionSize(QFontMetrics(font()).height());
        verticalHeader->setSectionResizeMode(QHeaderView::Fixed);

        auto horizontalHeader = ui->tvHoldingRegisters->horizontalHeader();
        horizontalHeader->setSectionResizeMode(QHeaderView::ResizeToContents);
        horizontalHeader->setSectionResizeMode(HoldingRegisters::Model::Error, QHeaderView::Stretch);
        horizontalHeader->setSectionResizeMode(HoldingRegisters::Model::Value, QHeaderView::Stretch);
        horizontalHeader->setContextMenuPolicy(Qt::CustomContextMenu);
        connect(horizontalHeader, &QWidget::customContextMenuRequested, [horizontalHeader, this](const QPoint& pos) {
            qDebug() << pos << horizontalHeader->logicalIndexAt(pos);
            QMenu menu;

            auto addAct = [&menu, this](int column, uint id, QString name) {
                auto action = menu.addAction(name);
                action->setCheckable(true);
                for(int row = 0; row < m_hrModel->rowCount(); ++row) {
                    auto index = ui->tvHoldingRegisters->model()->index(row, column);
                    if(index.data(Qt::UserRole).toUInt() == id) {
                        action->setChecked(!ui->tvHoldingRegisters->isRowHidden(row));
                        break;
                    }
                }
                connect(action, &QAction::toggled, [column, id, this](bool togled) {
                    for(int row = 0; row < m_hrModel->rowCount(); ++row) {
                        auto index = ui->tvHoldingRegisters->model()->index(row, column);
                        if(index.data(Qt::UserRole).toUInt() == id)
                            togled ? ui->tvHoldingRegisters->showRow(row) : ui->tvHoldingRegisters->hideRow(row);
                    }
                });
            };

            if(horizontalHeader->logicalIndexAt(pos) == HoldingRegisters::Model::Type) {
                auto typeOf = [&]<size_t... Is>(std::index_sequence<Is...>) { // бежим по типам
                    auto addAct2 = [&]<size_t I>(key<I>) {
                        addAct(HoldingRegisters::Model::Type, I, typeid(std::get<I>(HoldingRegisters::Data::variant {})).name());
                    };
                    (addAct2(key<Is> {}), ...);
                };
                typeOf(std::make_index_sequence<std::variant_size_v<HoldingRegisters::Data::variant>> {});
                menu.exec(horizontalHeader->mapToGlobal(pos));
            }
            if(horizontalHeader->logicalIndexAt(pos) == HoldingRegisters::Model::Acces) {
                addAct(HoldingRegisters::Model::Acces, HoldingRegisters::Data::ReadOnly, EnumHelper::toString(HoldingRegisters::Data::ReadOnly));
                addAct(HoldingRegisters::Model::Acces, HoldingRegisters::Data::ReadWrite, EnumHelper::toString(HoldingRegisters::Data::ReadWrite));
                menu.exec(horizontalHeader->mapToGlobal(pos));
            }
        });

        { // radiobuttons Type
            auto typeOf = [this]<size_t... Is>(std::index_sequence<Is...>) {
                auto addRadioButton = [this]<size_t I>(key<I>) {
                    auto radioButton = new QRadioButton(typeid(std::get<I>(HoldingRegisters::Data::variant {})).name(), this);
                    ui->vlayType->addWidget(radioButton);
                    connect(radioButton, &QRadioButton::toggled, [id = I, this](bool togled) {
                        for(int row = 0; row < m_hrModel->rowCount(); ++row) {
                            auto index = ui->tvHoldingRegisters->model()->index(row, HoldingRegisters::Model::Type);
                            if(index.data(Qt::UserRole).toUInt() == id)
                                togled ? ui->tvHoldingRegisters->showRow(row)
                                       : ui->tvHoldingRegisters->hideRow(row);
                            else
                                ui->tvHoldingRegisters->hideRow(row);
                        }
                    });
                };
                (addRadioButton(key<Is> {}), ...);
                auto radioButton = new QRadioButton("All", this);
                ui->vlayType->addWidget(radioButton);
                connect(radioButton, &QRadioButton::toggled, [this](bool togled) {
                    if(togled)
                        for(int row = 0; row < m_hrModel->rowCount(); ++row)
                            ui->tvHoldingRegisters->showRow(row);
                });
            };
            typeOf(std::make_index_sequence<std::variant_size_v<HoldingRegisters::Data::variant>> {});
        }
        { // radiobuttons Acces
            auto addRadioButton = [this](auto E) {
                auto radioButton = new QRadioButton(EnumHelper::toString(E), this);
                ui->vlayAcces->addWidget(radioButton);
                connect(radioButton, &QRadioButton::toggled, [id = E, this](bool togled) {
                    for(int row = 0; row < m_hrModel->rowCount(); ++row) {
                        auto index = ui->tvHoldingRegisters->model()->index(row, HoldingRegisters::Model::Acces);
                        if(index.data(Qt::UserRole).toUInt() == id)
                            togled ? ui->tvHoldingRegisters->showRow(row)
                                   : ui->tvHoldingRegisters->hideRow(row);
                        else
                            ui->tvHoldingRegisters->hideRow(row);
                    }
                });
            };
            addRadioButton(HoldingRegisters::Data::ReadOnly);
            addRadioButton(HoldingRegisters::Data::ReadWrite);

            auto radioButton = new QRadioButton("All", this);
            ui->vlayAcces->addWidget(radioButton);
            connect(radioButton, &QRadioButton::toggled, [this](bool togled) {
                if(togled)
                    for(int row = 0; row < m_hrModel->rowCount(); ++row)
                        ui->tvHoldingRegisters->showRow(row);
            });
        }
        auto pb = new QPushButton("Read All Holding Registers", this);
        ui->verticalLayout->addWidget(pb);
        connect(pb, &QPushButton::clicked, [this] {
            for(int row = 0; row < m_hrModel->rowCount(); ++row)
                if(!ui->tvHoldingRegisters->isRowHidden(row))
                    readHr(ui->tvHoldingRegisters->model()->index(row, HoldingRegisters::Model::Value));
        });
    }

    { // Coils
        QSortFilterProxyModel* proxyModel = new QSortFilterProxyModel(this);
        proxyModel->setSourceModel(m_clModel = new Coils::Model(this));
        proxyModel->setDynamicSortFilter(true);
        ui->tvCoils->setModel(proxyModel);
        ui->tvCoils->sortByColumn(HoldingRegisters::Model::Acces);
        //        ui->tvCoils->setItemDelegateForColumn(HoldingRegisters::Model::Value, new HoldingRegisters::ItemDelegate(this));

        auto verticalHeader = ui->tvCoils->verticalHeader();
        verticalHeader->setDefaultSectionSize(QFontMetrics(font()).height());
        verticalHeader->setSectionResizeMode(QHeaderView::Fixed);

        auto horizontalHeader = ui->tvCoils->horizontalHeader();
        horizontalHeader->setSectionResizeMode(QHeaderView::ResizeToContents);
        horizontalHeader->setSectionResizeMode(HoldingRegisters::Model::Error, QHeaderView::Stretch);
        horizontalHeader->setSectionResizeMode(HoldingRegisters::Model::Value, QHeaderView::Stretch);
        horizontalHeader->setContextMenuPolicy(Qt::CustomContextMenu);

        auto pb = new QPushButton("Read All Coils", this);
        ui->verticalLayout->addWidget(pb);
        connect(pb, &QPushButton::clicked, [this] {
            for(int row = 0; row < m_clModel->rowCount(); ++row)
                if(!ui->tvCoils->isRowHidden(row))
                    readCl(ui->tvCoils->model()->index(row, Coils::Model::Value));
        });
    }

    { // DiscreteInputs
        //static int m_diModel;
        QSortFilterProxyModel* proxyModel = new QSortFilterProxyModel(this);
        proxyModel->setSourceModel(m_diModel = new DiscreteInputs::Model(this));
        proxyModel->setDynamicSortFilter(true);
        ui->tvDiscreteInputs->setModel(proxyModel);
        ui->tvDiscreteInputs->sortByColumn(HoldingRegisters::Model::Acces);
        //        ui->tvDiscreteInputs->setItemDelegateForColumn(HoldingRegisters::Model::Value, new HoldingRegisters::ItemDelegate(this));

        auto verticalHeader = ui->tvDiscreteInputs->verticalHeader();
        verticalHeader->setDefaultSectionSize(QFontMetrics(font()).height());
        verticalHeader->setSectionResizeMode(QHeaderView::Fixed);

        auto horizontalHeader = ui->tvDiscreteInputs->horizontalHeader();
        horizontalHeader->setSectionResizeMode(QHeaderView::ResizeToContents);
        horizontalHeader->setSectionResizeMode(HoldingRegisters::Model::Error, QHeaderView::Stretch);
        horizontalHeader->setSectionResizeMode(HoldingRegisters::Model::Value, QHeaderView::Stretch);
        horizontalHeader->setContextMenuPolicy(Qt::CustomContextMenu);

        auto pb = new QPushButton("Read All Discrete Inputs", this);
        ui->verticalLayout->addWidget(pb);
        connect(pb, &QPushButton::clicked, [this] {
            for(int row = 0; row < m_diModel->rowCount(); ++row)
                if(!ui->tvDiscreteInputs->isRowHidden(row))
                    readDi(ui->tvDiscreteInputs->model()->index(row, DiscreteInputs::Model::Value));
        });
    }
}

void MainWindow::readHr(QModelIndex&& index) {
    if(!index.isValid())
        return;
    auto& var = HoldingRegisters::toRVariant(index);
    ui->tvHoldingRegisters->selectRow(index.row());
    auto visitor = [this, &index](auto&& arg) {
        using T = std::decay_t<decltype(arg)>;
        auto model = ui->tvHoldingRegisters->model();
        bool result {};
        auto reg = index.siblingAtColumn(HoldingRegisters::Model::Id).data(Qt::UserRole).toUInt();
        /**/ if constexpr(std::is_same_v<T, HoldingRegisters::Text>)
            result = I::mymodbus()->readHoldingRegisters(reg, arg, ByteOrder::ABCD {}); //USonicFlowmeter
        else if constexpr(std::is_same_v<T, HoldingRegisters::SetLinaCoef>)
            result = I::mymodbus()->readHoldingRegisters(reg, arg, ByteOrder::ABCD {});
        else
            result = I::mymodbus()->readHoldingRegisters(reg, arg, ByteOrder::ABCD {});

        if(result) {
            model->setData(index.siblingAtColumn(HoldingRegisters::Model::Error), "");
            model->setData(index.siblingAtColumn(HoldingRegisters::Model::Value), QVariant::fromValue(arg));
            emit model->dataChanged(index.siblingAtColumn(HoldingRegisters::Model::Value), index.siblingAtColumn(HoldingRegisters::Model::Error), {Qt::DisplayRole});
        } else {
            index = index.siblingAtColumn(HoldingRegisters::Model::Error);
            model->setData(index, I::mymodbus()->errorString());
            emit model->dataChanged(index, index, {Qt::DisplayRole});
        }
    };
    std::visit(visitor, var);
}

void MainWindow::readCl(QModelIndex&& index) {
    if(!index.isValid())
        return;
    ui->tvCoils->selectRow(index.row());

    int arg {};
    auto reg = index.siblingAtColumn(Coils::Model::Id).data(Qt::UserRole).toUInt();
    bool result = I::mymodbus()->readCoils(reg, arg);
    auto model = ui->tvCoils->model();

    if(result) {
        model->setData(index.siblingAtColumn(Coils::Model::Error), "");
        model->setData(index.siblingAtColumn(Coils::Model::Value), QVariant::fromValue(arg), Qt::UserRole);
        emit model->dataChanged(index.siblingAtColumn(Coils::Model::Value), index.siblingAtColumn(Coils::Model::Error), {Qt::DisplayRole});
    } else {
        index = index.siblingAtColumn(Coils::Model::Error);
        model->setData(index, I::mymodbus()->errorString());
        emit model->dataChanged(index, index, {Qt::DisplayRole});
    }
    //    auto& var = HoldingRegisters::toRVariant(index);
    //    ui->tvHoldingRegisters->selectRow(index.row());
    //    auto visitor = [this, &index](auto&& arg) {
    //        using T = std::decay_t<decltype(arg)>;
    //        auto model = ui->tvHoldingRegisters->model();
    //        bool result {};
    //        auto reg = index.siblingAtColumn(HoldingRegisters::Model::Id).data(Qt::UserRole).toUInt();
    //        /**/ if constexpr (std::is_same_v<T, HoldingRegisters::Text>)
    //            result = I::mymodbus()->readHoldingRegisters(reg, arg, ByteOrder::ABCD {}); //USonicFlowmeter
    //        else if constexpr (std::is_same_v<T, HoldingRegisters::SetLinaCoef>)
    //            result = I::mymodbus()->readHoldingRegisters(reg, arg, ByteOrder::ABCD {});
    //        else
    //            result = I::mymodbus()->readHoldingRegisters(reg, arg, ByteOrder::ABCD {});
    //        if (result) {
    //            model->setData(index.siblingAtColumn(HoldingRegisters::Model::Error), "");
    //            model->setData(index.siblingAtColumn(HoldingRegisters::Model::Value), QVariant::fromValue(arg));
    //            emit model->dataChanged(index.siblingAtColumn(HoldingRegisters::Model::Value), index.siblingAtColumn(HoldingRegisters::Model::Error), { Qt::DisplayRole });
    //        } else {
    //            index = index.siblingAtColumn(HoldingRegisters::Model::Error);
    //            model->setData(index, I::mymodbus()->errorString());
    //            emit model->dataChanged(index, index, { Qt::DisplayRole });
    //        }
    //    };
    //    std::visit(visitor, var);
}

void MainWindow::readDi(QModelIndex&& index) {
    if(!index.isValid())
        return;
    ui->tvDiscreteInputs->selectRow(index.row());

    int arg {};
    auto reg = index.siblingAtColumn(DiscreteInputs::Model::Id).data(Qt::UserRole).toUInt();
    bool result = I::mymodbus()->readDiscreteInputs(reg, arg);
    auto model = ui->tvDiscreteInputs->model();

    if(result) {
        model->setData(index.siblingAtColumn(DiscreteInputs::Model::Error), "");
        model->setData(index.siblingAtColumn(DiscreteInputs::Model::Value), QVariant::fromValue(arg), Qt::UserRole);
        emit model->dataChanged(index.siblingAtColumn(DiscreteInputs::Model::Value), index.siblingAtColumn(DiscreteInputs::Model::Error), {Qt::DisplayRole});
    } else {
        index = index.siblingAtColumn(DiscreteInputs::Model::Error);
        model->setData(index, I::mymodbus()->errorString());
        emit model->dataChanged(index, index, {Qt::DisplayRole});
    }
}

HoldingRegisters::Model* MainWindow::model() const { return m_hrModel; }

void MainWindow::on_pbReadSelected_clicked() {
    for(auto&& index : ui->tvHoldingRegisters->selectionModel()->selectedRows())
        readHr(index.siblingAtColumn(HoldingRegisters::Model::Value));
}

void MainWindow::regDataGen() {
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
        QString str {"RegData{"};
        for(auto& s : var) {
            str += s + ", ";
        }
        str += "},";
        qDebug() << str.toLocal8Bit().data();
    }
}
