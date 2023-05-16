#ifndef MODELVIEWDIALOG_H
#define MODELVIEWDIALOG_H

#include <QDialog>
#include <QStringList>
#include <QStringListModel>
#include <QAbstractItemView>


namespace Ui {
class ModelViewDialog;

}

class ModelViewDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ModelViewDialog(QWidget *parent = nullptr);
    ~ModelViewDialog();
    void AddItemToList(QString _item);
    int ClearList();
    int itemSelected();

signals:
    void m_listCleared(int _size);

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_comboBox_currentIndexChanged(int index);

private:
    Ui::ModelViewDialog *ui;
    QStringListModel *model;
    QStringList List;
    int     m_ItemSelected;
};

#endif // MODELVIEWDIALOG_H
