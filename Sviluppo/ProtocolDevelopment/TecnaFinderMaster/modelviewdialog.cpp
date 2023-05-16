#include "modelviewdialog.h"
#include "qstringlistmodel.h"
#include "ui_modelviewdialog.h"

ModelViewDialog::ModelViewDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ModelViewDialog)
{
    ui->setupUi(this);

    // Create model
    model = new QStringListModel(this);

    // Make data

    // List << "Clair de Lune" << "Reverie" << "Prelude";

    // Populate our model
    model->setStringList(List);

    // Glue model and view together
    ui->listView->setModel(model);
    // ui->comboBox->setModel(model);

    // Add additional feature so that
    // we can manually modify the data in ListView
    // It may be triggered by hitting any key or double-click etc.
    ui->listView->
            setEditTriggers(QAbstractItemView::AnyKeyPressed |
                            QAbstractItemView::DoubleClicked);

    // Init
    m_ItemSelected = -1;

}

int ModelViewDialog::ClearList()
{
    // ui->listView->reset();
    // List.clear();
    // List.begin();
    // ui->comboBox->clear();
    int iCount = ui->comboBox->count();

    for (int i=0; i < iCount; i++ ){
        ui->comboBox->removeItem(0);
    }

    iCount = ui->comboBox->currentIndex();

    int IRow = model->rowCount();
    for (int i=0; i< IRow; i++){
           model->removeRow(0);
    }
    emit m_listCleared(model->rowCount());

    return model->rowCount();

//    ui->listView->setModel(model);
//    ui->comboBox->setModel(model);
}

void ModelViewDialog::AddItemToList(QString _item)
{
    // ui->listView->setModel(nullptr);
    // ui->listView->reset();
    // ui->listView->scrollToTop();
    // ui->listView->setRootIndex(0);

//    QStringList list = model->stringList();
//    list.append(_item);
//    model->setStringList(list);

    // List.append(_item);
    // model->setStringList(List);


//    ui->listView->setModel(model);
//    ui->comboBox->setModel(model);

    qDebug() << "ModelViewDialog::AddItemToList";

    if (model->insertRow(model->rowCount())){
        QModelIndex index = model->index(model->rowCount() - 1, 0);
        model->setData(index, _item);
    }

    ui->comboBox->addItem(_item);
}

ModelViewDialog::~ModelViewDialog()
{
    delete ui;
}

void ModelViewDialog::on_pushButton_clicked()
{

}


void ModelViewDialog::on_pushButton_2_clicked()
{

}


void ModelViewDialog::on_pushButton_3_clicked()
{

}


void ModelViewDialog::on_comboBox_currentIndexChanged(int index)
{
    m_ItemSelected = index;
    qDebug() << "ModelViewDialog::on_comboBox_currentIndexChanged: Now selecting" << index;
}

int ModelViewDialog::itemSelected()
{
    return m_ItemSelected;
}

