/**
* @file { main.cpp }
* @brief { Soubor obsahuje inicializaci a spusteni grafickeho rozhrani kalkulacky }
*/


#include <cstdlib>


#include "etepc_calc.h"
#include <QApplication>

/**
 * @brief main { funkce main }
 * @param argc { pocet argumentu zadanych z prikazove radky }
 * @param argv { odkaz na pole argumentu }
 * @return { vrati nulu pri uspesnem ukonceni programu }
 */
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ETEPC_calc w;
    w.show();

    return a.exec();
}
