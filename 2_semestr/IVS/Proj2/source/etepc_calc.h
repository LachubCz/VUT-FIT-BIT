/**
* @file { etepc_calc.h }
* @brief { Soubor obsahuje knihovnu pro ETEPC-5000 }
*/


#ifndef ETEPC_CALC_H
#define ETEPC_CALC_H

#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include <QAction>
#include <QButtonGroup>
#include <QGridLayout>
#include <QHeaderView>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QWidget>
#include <QString>
#include <QMessageBox>


/**
 * @brief { Namespace pro rozhrani }
 */
namespace Ui {
class ETEPC_calc;
}


/**
 * @brief { Trida popisujici rozhrani kalkulacky }
 */
class ETEPC_calc : public QMainWindow
{
    Q_OBJECT

public:
    explicit ETEPC_calc(QWidget *parent = 0);
    ~ETEPC_calc();

private:
    Ui::ETEPC_calc *ui;

    QLineEdit * display;
    QAction *actionQuit;
    QAction *action_help;
    QAction *action_about;
    QWidget *centralwidget;
    QWidget *layoutWidget;
    QGridLayout *gridLayout;
    QPushButton *Button_mul;
    QPushButton *Button_div;
    QPushButton *Button_minus;
    QPushButton *Button_7;
    QPushButton *Button_8;
    QPushButton *Button_9;
    QPushButton *Button_plus;
    QPushButton *Button_4;
    QPushButton *Button_5;
    QPushButton *Button_6;
    QPushButton *Button_1;
    QPushButton *Button_2;
    QPushButton *Button_3;
    QPushButton *Button_result;
    QPushButton *Button_0;
    QPushButton *Button_DPoint;
    QPushButton *Button_plusminus;
    QWidget *layoutWidget1;
    QVBoxLayout *verticalLayout;
    QPushButton *Button_C;
    QPushButton *Button_fact;
    QPushButton *Button_ln;
    QPushButton *Button_power;
    QPushButton *Button_Ans;
    QMenuBar *menubar;
    QMenu *menuFile;
    QStatusBar *statusbar;


    /**
     * @brief { Promenna pro rizeni prepisovani displeje. Pokud je ve stavu false, bude displej pred psanim vycisten, pokud ve stavu true, cisla se dopisuji na konec }
     */
    bool calc_status;
    double op1, op2;
    double Ans;

    /**
     * @struct { opr }
     * @brief { Struktura pro popsani operatoru se dvema operandy }
     * @var opr::type
     * { Typ operandu (+, -, *, /, p (mocnina s celociselnym nezapornym exponentem)) }
     * @var opr::status
     * { Stav aktualnosti operatoru (byl-li stisknut jiz nejaky operator a ceka se na druhy operand, status==true) }
     *
     */
    typedef struct
    {
        char type;
        bool status;
    }opr;
     opr oper;


    void writeResult(double num);
    void solution_2op (double op1, double op2, opr *oper);
    void solution_1op (double op, const char oper);

private slots:
    void num_hit();
    void dpoint_hit();
    void operator1_hit();
    void operator2_hit();
    void neg_hit();
    void clear_hit();
    void result_hit();
    void action_About();
    void action_Help();
    void ans_hit();
};

#endif // ETEPC_CALC_H
