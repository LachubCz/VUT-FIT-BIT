/**
 * \file Proj3.h
 * @brief IZP - Project #3
 * 
 * Program provadi jednoduchou shlukovou analyzu. Pouziva metodu nejblizsiho souseda (angl. single linkage). Shlukova analyza se provadi na dvourozmernych objektech. 
 * @author Petr Buchal
 * @date 13.12.2015
 * @see https://goo.gl/K2ybAk
 */

/** @defgroup group1 Modul deklarace potrebnych datovych typu
 *  
 *  @{
 */

/**
 * @brief      objekt
 */
struct obj_t {
    int id; /**< identifikator */
    float x; /**< souradnice x */
    float y; /**< souradnice y */
};
/**
 * @brief      shluk objektu
 */
struct cluster_t {
    int size; /**< pocet objektu ve shluku */
    int capacity; /**< kapacita shluku (pocet objektu, pro ktere je alokovane misto) */
    struct obj_t *obj; /**< ukazatel na pole shluku */
};

/** @} */ // end of group1

/** @defgroup group2 Modul deklarace potrebnych funkci
 * 	
 *  @{
 */

/**
 * @brief      Inicializuje shluk 'c'
 * 
 * @param      c     ukazatel na shluk 'c'
 * @param      cap   kapacita na kterou je shluk inicializovan
 * 
 * @pre      c != NULL
 * @pre      cap >= 0
 * @post     c->size = 0
 * @post     c->capacity = cap
 */
void init_cluster(struct cluster_t *c, int cap);

/**
 * @brief      Vycisti shluk 'c'
 *
 * @param      c     ukazatel na shluk 'c'
 * 
 * @post     c->size = 0
 * @post     c->capacity = 0
 * @post     c->obj = NULL
 */
void clear_cluster(struct cluster_t *c);

/**
 * @brief    Doporucena hodnota pro realokaci
 */
extern const int CLUSTER_CHUNK;

/**
 * @brief      Zmeni kapacitu shluku 'c'
 * 
 * Hodnota nové kapacity je new_cap.
 *
 * @param      c        ukazatel na shluk 'c'
 * @param  new_cap  nova velikost shluku 'c'
 * 
 * @pre     c->capacity >= 0
 * @pre     new_cap >= 0
 * @post     c->capacity = new_cap
 * 
 * @return     funkce vraci ukazatel na zvetseny shluk se zvetsenou kapacitou
 */
struct cluster_t *resize_cluster(struct cluster_t *c, int new_cap);

/**
 * @brief      Prida objekt 'obj' na konec shluku 'c'
 * 
 * Rozsiri shluk, pokud se do nej objekt nevejde.
 * 
 * @param      c     ukazatel na shluk 'c'
 * @param      obj   objekt, ktery bude pridan do shluku 'c'
 * 
 * @post     c->obj[c->size] = obj
 * @post     c->size = c->size + 1
 */
void append_cluster(struct cluster_t *c, struct obj_t obj);

/**
 * @brief      Prida objekty ze shluku 'c2' do 'c1'
 * 
 * Shluk 'c1' bude v pripade nutnosti rozsiren. Objekty ve shluku 'c1' budou serazny vzestupne podle identifikacniho cisla. Shluk 'c2' bude nezmenen.
 *
 * @param      c1    ukazatel na shluk 'c1'
 * @param      c2    ukazatel na shluk 'c2'
 * 
 * @pre      c1 != NULL
 * @pre      c2 != NULL
 */
void merge_clusters(struct cluster_t *c1, struct cluster_t *c2);

/** @} */ // end of group2

/** @defgroup group3 Modul deklarace funkci pracujicich s polem shluku
 *  
 *  @{
 */

/**
 * @brief      Odstrani shluk z pole shluku 'carr'
 *
 * @param      carr  ukazatel na pole shluku 'carr'
 * @param      narr  pocet objektu v poli shluku 'carr'
 * @param      idx   index shluku, ktery ma byt odstranen
 * 
 * @pre      idx < narr
 * @pre      narr > 0
 * @post     narr = narr - 1
 *
 * @return     funkce vraci novy pocet shluku v poli
 */
int remove_cluster(struct cluster_t *carr, int narr, int idx);

/**
 * @brief      Pocita Euklidovskou vzdalenost mezi dvema objekty
 *
 * @param      o1    ukazatel na prvni objekt
 * @param      o2    ukazatel na druhy objekt
 * 
 * @pre      o1 != NULL
 * @pre      o2 != NULL
 *
 * @return     funkce vraci vzdalenost dvou objektu
 */
float obj_distance(struct obj_t *o1, struct obj_t *o2);

/**
 * @brief      Pocita vzdalenost dvou shluku
 * 
 * Vzdalenost je vypoctena na zaklade nejblizsiho souseda.
 *
 * @param      c1    ukazatel na shluk 'c1'
 * @param      c2    ukazatel na shluk 'c2'
 * 
 * @pre      c1 != NULL
 * @pre      c1->size > 0
 * @pre      c2 != NULL
 * @pre      c2->size > 0
 *
 * @return     funkce vraci vzdalenost dvou shluku
 */
float cluster_distance(struct cluster_t *c1, struct cluster_t *c2);

/**
 * @brief      Hleda dva nejblizsi shluky
 * 
 * Vyuziva metody nejblizsiho souseda. Nalezene shluky identifikuje jejich indexy v poli 'carr'. Funkce nalezene shluky (indexy do pole 'carr') uklada do pameti na adresu 'c1' resp. 'c2'.
 *
 * @param      carr  ukazatel na pole shluku 'carr'
 * @param      narr  velikost pole shluku 'carr'
 * @param      c1    ukazatel na index prviho ze dvou nejblizsich shluku
 * @param      c2    ukazatel na index druheho ze dvou nejblizsich shluku
 * 
 * @pre      narr > 0
 * @post     *c1 = u
 * @post     *c2 = o
 */
void find_neighbours(struct cluster_t *carr, int narr, int *c1, int *c2);

/**
 * @brief      Seradi objekty ve shluku 'c'
 * 
 * Razeni probiha podle jejich identifikacniho cisla.
 *
 * @param      c     ukazatel na shluk 'c'
 */
void sort_cluster(struct cluster_t *c);

/**
 * @brief      Tiskne shluk 'c' na stdout
 *
 * @param      c     ukazatel na shluk 'c'
 */
void print_cluster(struct cluster_t *c);

/**
 * @brief      Nacita objekt ze souboru do clusteru
 * 
 * Objekty nacita ze souboru 'filename'. Pro kazdy objekt vytvori shluk a ulozi jej do pole shluku. Alokuje prostor pro pole vsech shluku a ukazatel na prvni polozku pole (ukalazatel na prvni shluk v alokovanem poli) ulozi do pameti, kam se odkazuje parametr 'arr'. Funkce vraci pocet nactenych objektu (shluku).V pripade nejake chyby uklada do pameti, kam se odkazuje 'arr', hodnotu NULL.
 *
 * @param      filename  ukazatel na jmeno souboru pro nacitani
 * @param      arr       ukazatel na pole shluku
 * 
 * @pre      arr != NULL
 *
 * @return     funkce vraci pocet nactenych shluku
 */
int load_clusters(char *filename, struct cluster_t **arr);

/**
 * @brief      Tiskne pole shluku
 * 
 * Tiskne se prvnich 'narr' shluku.
 * 
 * @param      carr  ukazatel na pole shluku 'carr'
 * @param      narr  pocet tisknoucich se shluku v poli shluku 'carr'
 */
void print_clusters(struct cluster_t *carr, int narr);

/** @} */ // end of group3
