#ifndef FLOWMETER_H
#define FLOWMETER_H

#include <QMainWindow>
#include <QThread>

namespace Reg {
class Model;
}
namespace Ui {
class Flowmeter;
}

class Flowmeter : public QMainWindow {
    Q_OBJECT

public:
    explicit Flowmeter(QWidget* parent = nullptr);
    ~Flowmeter();

    Reg::Model* model() const;

private slots:
    void regDataGen();
    void on_pbReadAll_clicked();
    void on_pbReadSelected_clicked();

private:
    Ui::Flowmeter* ui;

    void loadSettings();
    void saveSettings();
    void initCbxPorts();
    void initTableView();

    void read(QModelIndex&& index);

    Reg::Model* m_model;
};

#endif // FLOWMETER_H
