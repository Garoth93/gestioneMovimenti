#ifndef DIALOGSEPARATORE_H
#define DIALOGSEPARATORE_H

#include <QDialog>

namespace Ui {
class dialogSeparatore;
}

class dialogSeparatore : public QDialog
{
    Q_OBJECT

public:
    explicit dialogSeparatore(QWidget *parent = nullptr);
    ~dialogSeparatore();
    virtual void init(QString * separatore);

private:
    Ui::dialogSeparatore *ui;
};

#endif // DIALOGSEPARATORE_H
