#pragma once

#include <QMainWindow>
#include <QThread>

namespace HoldingRegisters {
class Model;
}
namespace Coils {
class Model;
}
namespace DiscreteInputs {
class Model;
}
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

    HoldingRegisters::Model* model() const;

private slots:
    void regDataGen();
    void on_pbReadSelected_clicked();

private:
    Ui::MainWindow* ui;

    void loadSettings();
    void saveSettings();
    void initCbxPorts();
    void initTableView();

    void readHr(QModelIndex&& index);
    void readCl(QModelIndex&& index);
    void readDi(QModelIndex&& index);

    HoldingRegisters::Model* m_hrModel;
    Coils::Model* m_clModel;
    DiscreteInputs::Model* m_diModel;
};
