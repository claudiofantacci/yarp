#ifndef BATCHQOSCONFDIALOG_H
#define BATCHQOSCONFDIALOG_H

#include <QDialog>
#include <yarp/profiler/NetworkProfiler.h>

namespace Ui {
class BatchQosConfDialog;
}

class BatchQosConfDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BatchQosConfDialog(QWidget *parent = 0);
    ~BatchQosConfDialog();

private slots:
    void openCons();
    void updateQos();
    void configureQos();

private:
    Ui::BatchQosConfDialog *ui;
    yarp::profiler::graph::Graph* graph;
};

#endif // BATCHQOSCONFDIALOG_H
