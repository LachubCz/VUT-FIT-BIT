/**
* @file { etepc_calc.cpp }
* @brief { Soubor obsahuje definice funkci z knihovny pro ETEPC-5000 }
*/


#include "etepc_calc.h"
#include "ui_etepc_calc.h"

#include "knihrary.h"


/**
 * @brief { Konstruktor rozhrani kalkulacky }
 * @param parent { rodicovsky objekt, jedna-li se o hlavni objekt, parent==0 }
 */
ETEPC_calc::ETEPC_calc(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ETEPC_calc)
{
    ui->setupUi(this);

    calc_status = false;
    Ans = op1 = op2 = 0.0;

    oper.status = false;
    oper.type = '+';
}


/**
 * @brief { Slot pro zpracovani signalu clicked numerickych tlacitek; pokud byl calc_status false, nastavi jej na true }
 */
void ETEPC_calc::num_hit()
{
    QPushButton * obj = qobject_cast<QPushButton*>(sender());
    int digit = obj->text().toInt();

    if(calc_status == false){
        ui->display->clear();
        ui->display->setText(QString::number(digit));
        calc_status = true;
    }
    else
        ui->display->setText(ui->display->text()+QString::number(digit));
}


/**
 * @brief { Slot pro zpracovani signalu clicked tlacitka desetinne tecky; pokud byl calc_status false, nastavi jej na true }
 */
void ETEPC_calc::dpoint_hit()
{
    if(calc_status == false){
        ui->display->clear();
        calc_status = true;
    }
    if(!ui->display->text().contains(('.'))){
      int num = ui->display->text().toInt();
      ui->display->setText(QString::number(num)+".");
    }
}


/**
 * @brief { Slot pro zpracovani signalu clicked tlacitka operatoru s jednim operandem; , calc_status nastavi na false }
 */
void ETEPC_calc::operator1_hit()
{
    QPushButton * obj = qobject_cast<QPushButton*>(sender());
    QString button = obj->objectName();
    char type;

    op1 = ui->display->text().toDouble();

    if(button == "Button_fact") type = '!';
    else if (button == "Button_ln") type = 'l';
    else return;

    solution_1op(op1, type);
    calc_status = false;
}


/**
 * @brief { Slot pro zpracovani signalu clicked tlacitka operatoru se dvema operandy; oper.status nastavi na true, calc_status nastavi na false }
 */
void ETEPC_calc::operator2_hit()
{
    if(oper.status)
    {
        op2 = ui->display->text().toDouble();
        solution_2op(op1, op2,  &oper);
        op1 = ui->display->text().toDouble();
    }
    else
    {
        op1 = ui->display->text().toDouble();
    }
    QPushButton * obj = qobject_cast<QPushButton*>(sender());
    QString button = obj->objectName();

    if(button == "Button_plus") oper.type = '+';
    else if(button == "Button_minus") oper.type = '-';
    else if(button == "Button_mul") oper.type = '*';
    else if(button == "Button_div") oper.type = '/';
    else if(button == "Button_power") oper.type = 'p';
    else return;

    oper.status = true;
    calc_status = false;
}


/**
 * @brief { Slot pro zpracovani signalu clicked tlacitka Ans; calc_status nastavi na false }
 */
void ETEPC_calc::ans_hit()
{
  ui->display->setText(QString::number(Ans));
  calc_status = false;
}


/**
 * @brief { Slot pro zpracovani signalu clicked tlacitka negace (+/-); zneguje cislo na obrazovce }
 */
void ETEPC_calc::neg_hit()
{
    double num;
    num = ui->display->text().toDouble();
    num = -num;
    ui->display->setText(QString::number(num));
}


/**
 * @brief { Slot pro zpracovani signalu clicked tlacitka C; vymaze displej, calc_status nastavi na false }
 */
void ETEPC_calc::clear_hit()
{
    ui->display->clear();
    calc_status = false;
}


/**
 * @brief { Slot pro zpracovani signalu clicked tlacitka vysledku (=); zavola funkci pro vypocet operaci se dvema operandy, do Ans ulozi vysledek (pokud neni nan ci nekonecno)  }
 */
void ETEPC_calc::result_hit()
{
    if(oper.status)
    {
        op2 = ui->display->text().toDouble();
        solution_2op(op1, op2, &oper);
        Ans = ui->display->text().toDouble();
    }

   calc_status = false;
}


/**
 * @brief { Slot pro zpracovani akce action_About; vypsani dialogu o programu }
 */
void ETEPC_calc::action_About()
{
            QString infotext = "<b>Name</b>: ETEPC-5000 (Balda) <br/><b>Authors</b>: Petr Buchal, Petra Buchtová, Pavel Černý, Radim Červinka <br/><b>Popis</b> Tupá kalkulačka<br/><b>Version</b>: 1.0 <br/><b>Year</b>: 2016";
            QMessageBox::about(this, "About ETEPC-5000", infotext);
}


/**
 * @brief { Slot pro zpracovani akce action_Help; vypsani napovedy }
 */
void ETEPC_calc::action_Help()
{
    QString helptext = "<b>Seznam klávesových zkratek</b>:<br/><b>[0-9]</b></b> - číslice<br/><b>[,]</b> - desetinná čárka<br/><b>[+]</b> - tlačítko operace plus<br/><b>[-]</b> - tlačítko operace mínus<br/><b>[*]</b> - tlačítko operace krát<br/><b>[/]</b> - tlačítko operace děleno<br/><b>[!]</b> - tlačítko operace faktoriál<br/><b>[return/enter]</b> - rovnítko (výpočet výsledku)<br/><b>[delete]</b> - tlačítko C, čistič obrazovky<br/><b>[space]</b> - tlačítko Ans, poslední vypočítaný výsledek, též [mezerník]<br/><br/><b>[Alt + F]</b> - file (nabídka)<br/>-> <b>[Q]</b> - Quit<br/>-> <b>[A]</b> - About<br/>-> <b>[H]</b> - Help";
    QMessageBox::information(this,"::Help::", helptext);
}


/**
 * @brief { Destruktor objektu }
 */
ETEPC_calc::~ETEPC_calc()
{
    delete ui;
}


/**
 * @brief { Funkce na zapsani vysledku na displej; misto nan vypise retezec "ErroR"}
 * @param num { vysledek, ktery se ma vypsat }
 */
void ETEPC_calc::writeResult(double num)
{
    if (isnan(num))
    {
        ui->display->setText("ErroR");
     }
    else
    {
       ui->display->setText(QString::number(num));
    }
}


/**
 *
 * @brief      { Funkce provede vypocet na zaklade dane operace, vypise vysledek na displej; oper->status nastavi na false }
 * @param      op1     { prvni operand }
 * @param      op2     { druhy operand }
 * @param      oper    { ukazatel na strukturu operatoru }
 */
void ETEPC_calc::solution_2op (double op1, double op2, opr *oper)
{
    double result;

    switch(oper->type)
    {
        case'+':
        result = add(op1,op2);
        break;
        case'-':
        result = sub(op1,op2);
        break;
        case'*':
        result = mul(op1,op2);
        break;
        case'/':
        result = division(op1,op2);
        break;
        case'p':
        result = power(op1,op2);
        break;
    } // switch

    oper->status = false;
    writeResult(result);
}

/**
 * @brief { Funkce provede vypocet na zaklade dane operace s jednim operandem, vypise vysledek na displej }
 * @param op { operand }
 * @param oper { operator }
 */
void ETEPC_calc::solution_1op(double op, const char oper)
{
    double result;

    switch(oper)
    {
        case'!':
        result = fact(op);
        break;
        case'l':
        result = ln(op);
        break;
    } // switch

    writeResult(result);
}
