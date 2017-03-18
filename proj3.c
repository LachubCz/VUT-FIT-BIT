/**
 * Kostra programu pro 3. projekt IZP 2015/16
 *
 * Jednoducha shlukova analyza: 2D nejblizsi soused.
 * Single linkage
 * http://is.muni.cz/th/172767/fi_b/5739129/web/web/slsrov.html
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h> // sqrtf
#include <limits.h> // INT_MAX

/*****************************************************************
 * Ladici makra. Vypnout jejich efekt lze definici makra
 * NDEBUG, napr.:
 *   a) pri prekladu argumentem prekladaci -DNDEBUG
 *   b) v souboru (na radek pred #include <assert.h>
 *      #define NDEBUG
 */
#ifdef NDEBUG
#define debug(s)
#define dfmt(s, ...)
#define dint(i)
#define dfloat(f)
#else

// vypise ladici retezec
#define debug(s) printf("- %s\n", s)

// vypise formatovany ladici vystup - pouziti podobne jako printf
#define dfmt(s, ...) printf(" - "__FILE__":%u: "s"\n",__LINE__,__VA_ARGS__)

// vypise ladici informaci o promenne - pouziti dint(identifikator_promenne)
#define dint(i) printf(" - " __FILE__ ":%u: " #i " = %d\n", __LINE__, i)

// vypise ladici informaci o promenne typu float - pouziti
// dfloat(identifikator_promenne)
#define dfloat(f) printf(" - " __FILE__ ":%u: " #f " = %g\n", __LINE__, f)

#endif

/*****************************************************************
 * Deklarace potrebnych datovych typu:
 *
 * TYTO DEKLARACE NEMENTE
 *
 *   struct obj_t - struktura objektu: identifikator a souradnice
 *   struct cluster_t - shluk objektu:
 *      pocet objektu ve shluku,
 *      kapacita shluku (pocet objektu, pro ktere je rezervovano
 *          misto v poli),
 *      ukazatel na pole shluku.
 */

struct obj_t {
    int id;
    float x;
    float y;
};

struct cluster_t {
    int size;
    int capacity;
    struct obj_t *obj;
};

/*****************************************************************
 * Deklarace potrebnych funkci.
 *
 * PROTOTYPY FUNKCI NEMENTE
 *
 * IMPLEMENTUJTE POUZE FUNKCE NA MISTECH OZNACENYCH 'TODO'
 *
 */

/*
 Inicializace shluku 'c'. Alokuje pamet pro cap objektu (kapacitu).
 Ukazatel NULL u pole objektu znamena kapacitu 0.
*/
void init_cluster(struct cluster_t *c, int cap)
{
    assert(c != NULL);
    assert(cap >= 0);

    c->size = 0;
    c->capacity = cap;
    c->obj = malloc(cap*sizeof(struct obj_t));
}

/*
 Odstraneni vsech objektu shluku a inicializace na prazdny shluk.
 */
void clear_cluster(struct cluster_t *c)
{
    c->size = 0;
    c->capacity = 0;
    free(c->obj);
    c->obj = NULL;
}

/// Chunk of cluster objects. Value recommended for reallocation.
const int CLUSTER_CHUNK = 10;

/*
 Zmena kapacity shluku 'c' na kapacitu 'new_cap'.
 */
struct cluster_t *resize_cluster(struct cluster_t *c, int new_cap)
{
    // TUTO FUNKCI NEMENTE
    assert(c);
    assert(c->capacity >= 0);
    assert(new_cap >= 0);

    if (c->capacity >= new_cap)
        return c;

    size_t size = sizeof(struct obj_t) * new_cap;

    void *arr = realloc(c->obj, size);
    if (arr == NULL)
        return NULL;

    c->obj = arr;
    c->capacity = new_cap;
    return c;
}

/*
 Prida objekt 'obj' na konec shluku 'c'. Rozsiri shluk, pokud se do nej objekt
 nevejde.
 */
void append_cluster(struct cluster_t *c, struct obj_t obj)
{
if(c->capacity <= c->size)
    {
        c = resize_cluster(c, c-> capacity + CLUSTER_CHUNK);
    }
    c->obj[c->size] = obj;
    c->size = c->size + 1;
}

/*
 Seradi objekty ve shluku 'c' vzestupne podle jejich identifikacniho cisla.
 */
void sort_cluster(struct cluster_t *c);

/*
 Do shluku 'c1' prida objekty 'c2'. Shluk 'c1' bude v pripade nutnosti rozsiren.
 Objekty ve shluku 'c1' budou serazny vzestupne podle identifikacniho cisla.
 Shluk 'c2' bude nezmenen.
 */
void merge_clusters(struct cluster_t *c1, struct cluster_t *c2)
{
    assert(c1 != NULL);
    assert(c2 != NULL);

    for (int u = 0; u < c2->size; u++)
        append_cluster(c1, c2->obj[u]);

    sort_cluster(c1);
}

/**********************************************************************/
/* Prace s polem shluku */

/*
 Odstrani shluk z pole shluku 'carr'. Pole shluku obsahuje 'narr' polozek
 (shluku). Shluk pro odstraneni se nachazi na indexu 'idx'. Funkce vraci novy
 pocet shluku v poli.
*/
int remove_cluster(struct cluster_t *carr, int narr, int idx)
{
    assert(idx < narr);
    assert(narr > 0);

    clear_cluster (&carr[idx]);
    init_cluster(&carr[idx], CLUSTER_CHUNK);
    merge_clusters(&carr[idx], &carr[narr - 1]);
    clear_cluster (&carr[narr -1]);

    return narr-1;
}

/*
 Pocita Euklidovskou vzdalenost mezi dvema objekty.
 */
float obj_distance(struct obj_t *o1, struct obj_t *o2)
{
    assert(o1 != NULL);
    assert(o2 != NULL);

    float vys = sqrtf((o1->x - o2->x)*(o1->x - o2->x)+(o1->y - o2->y)*(o1->y - o2->y));
    return vys;
}

/*
 Pocita vzdalenost dvou shluku. Vzdalenost je vypoctena na zaklade nejblizsiho
 souseda.
*/
float cluster_distance(struct cluster_t *c1, struct cluster_t *c2)
{
    assert(c1 != NULL);
    assert(c1->size > 0);
    assert(c2 != NULL);
    assert(c2->size > 0);

    int u, o;
    float e, p;

    for (u = 0; u < c1->size; u++)
        for (o = 0; o < c2->size; o++)
            {
                e = obj_distance(&c1->obj[u], &c2->obj[o]);  //pocitani vzdalenosti dvou objektu
                if (u == 0 && o == 0)  //ukladani nejmensi vzdalenosti
                    {
                        p = e;
                    }
                else
                {
                    if (e < p)
                        p = e;
                }
            }
    return p;  //fce vraci minimalni vzdalenost mezi clustery
}

/*
 Funkce najde dva nejblizsi shluky. V poli shluku 'carr' o velikosti 'narr'
 hleda dva nejblizsi shluky (podle nejblizsiho souseda). Nalezene shluky
 identifikuje jejich indexy v poli 'carr'. Funkce nalezene shluky (indexy do
 pole 'carr') uklada do pameti na adresu 'c1' resp. 'c2'.
*/
void find_neighbours(struct cluster_t *carr, int narr, int *c1, int *c2)
{
    assert(narr > 0);

    float f, win;
    int u, o;
    for (u = 0; u < narr; u++)
        {
            for (o = u + 1; o < narr; o++)
                {
                    f = cluster_distance(&carr[u], &carr[o]);  //pocitani vzdalenosti dvou clusteru
                    if (u == 0 && o ==1)  //ukladani nejmensi vzdalenosti
                        {
                            win = f;
                            *c1 = u;
                            *c2 = o;
                        }
                    else
                        {
                            if (win > f)
                            {
                                win = f;
                                *c1 = u;
                                *c2 = o;
                            }
                        }
                }
        }
}

// pomocna funkce pro razeni shluku
static int obj_sort_compar(const void *a, const void *b)
{
    // TUTO FUNKCI NEMENTE
    const struct obj_t *o1 = a;
    const struct obj_t *o2 = b;
    if (o1->id < o2->id) return -1;
    if (o1->id > o2->id) return 1;
    return 0;
}

/*
 Razeni objektu ve shluku vzestupne podle jejich identifikatoru.
*/
void sort_cluster(struct cluster_t *c)
{
    // TUTO FUNKCI NEMENTE
    qsort(c->obj, c->size, sizeof(struct obj_t), &obj_sort_compar);
}

/*
 Tisk shluku 'c' na stdout.
*/
void print_cluster(struct cluster_t *c)
{
    // TUTO FUNKCI NEMENTE
    for (int i = 0; i < c->size; i++)
    {
        if (i) putchar(' ');
        printf("%d[%g,%g]", c->obj[i].id, c->obj[i].x, c->obj[i].y);
    }
    putchar('\n');
}

/*
 Ze souboru 'filename' nacte objekty. Pro kazdy objekt vytvori shluk a ulozi
 jej do pole shluku. Alokuje prostor pro pole vsech shluku a ukazatel na prvni
 polozku pole (ukalazatel na prvni shluk v alokovanem poli) ulozi do pameti,
 kam se odkazuje parametr 'arr'. Funkce vraci pocet nactenych objektu (shluku).
 V pripade nejake chyby uklada do pameti, kam se odkazuje 'arr', hodnotu NULL.
*/

int load_clusters(char *filename, struct cluster_t **arr)
{
    assert(arr != NULL);

    int q = 0;
    int cislo = 0;
    int kontrola;
    char c;
    struct obj_t o;
    FILE *fp;


    fp = fopen(filename,"r");  //otevreni souboru pro test chybnych znaku
    fscanf(fp, "count=%d", &cislo);
    while((c = fgetc(fp)) != EOF)
    {
        if (c > 32)  //test znaku pres ASCII hodnoty
            if (c < 48 || c > 57)
            {
                arr = NULL;
                fclose(fp);
                return -2;
            }
    }
    fclose(fp);  //uzavreni souboru


    fp = fopen(filename,"r");  //otevreni souboru pro test velikosti souradnic
    fscanf(fp, "count=%d", &cislo);
    while(q < cislo)
            {
                kontrola = fscanf(fp, "%d %f %f\n", &o.id, &o.x, &o.y);
                if (kontrola != 3)
                {
                    fclose(fp);
                    arr = NULL;
                    return -2;
                }

                if (o.x > 1000 || o.y > 1000 || o.x < 0 || o.y < 0)
                {
                    arr = NULL;
                    fclose(fp);
                    return -1;
                }
                q++;
            }
    fclose(fp);  //uzavreni souboru


    fp = fopen(filename,"r");  //otevreni souboru pro realne nahravani

    *arr = (struct cluster_t *) (malloc(sizeof(struct cluster_t) * (cislo)));  //alokovani mista pro ukazatel na pole clusteru
    fscanf(fp, "count=%d", &cislo);  //precteni prvniho radku

    q = 0;
    while (q < cislo)  //inicializace clusteru
    {
        init_cluster(&(*arr)[q], CLUSTER_CHUNK);
        q++;
    }

    q = 0;
            while(q < cislo)  //necitani objektu do jednotlivych clusteru
            {
                fscanf(fp, "%d %f %f\n", &o.id, &o.x, &o.y);
                append_cluster(&(*arr)[q], o);
                q++;
            }

    fclose(fp);  //uzavreni souboru

   return cislo;  //vraci pocet clusteru
}

/*
 Tisk pole shluku. Parametr 'carr' je ukazatel na prvni polozku (shluk).
 Tiskne se prvnich 'narr' shluku.
*/
void print_clusters(struct cluster_t *carr, int narr)
{
    printf("Clusters:\n");
    for (int u = 0; u < narr; u++)
    {
        printf("Cluster %d: ", u);
        print_cluster(&carr[u]);
    }
}

int main(int argc, char *argv[])
{
    int pocet;  //promenna pro hledany pocet shluku

    int u;  //promenna opakovane pouzivanna pro cykly for
    struct cluster_t *clusters;  //ukazatel na prvni cluster v poli


    if (argc > 3 || argc < 2)  //kontrola argumentu
    {
        printf ("INVALID_NUMBER_OF_ARGUMENTS\n");
        return 0;
    }
    if (argc == 2)  //pokud je argument dva nezadan je implicitne 1
    {
        pocet = 1;
    }
    else
    {
        char *control;
        pocet = strtoul(argv[2], &control, 0);  //ulozeni argumentu do promenne
        if (*control != '\0')  //test spravnosti zadane hodnoty argumentu
        {
            printf ("INVALID_ARGUMENT\n");
            return 0;
        }
    }
    if (pocet <= 0)  //pokud je hledany pocet shluku 0 nebo mensi, program se ukonci
    {
        printf("INVALID_ARGUMENT\n");
        return 0;
    }


    FILE *fp;  //kontrola existence souboru
    if ((fp = fopen(argv[1],"r")) == NULL)  //otevreni souboru
        {
            printf("FILE_DOES_NOT_EXIST\n");
            return 0;
        }
    fclose(fp);  //uzavreni souboru


    int lol = load_clusters(argv[1], &clusters);  //vraci pocet clustru stejny jako pocet objektu


    if (lol == -1)  //souradnice nektereho objektu jsou mimo rozsah 0 <= X <= 1000, 0 <= Y <= 1000
    {
        printf("OBJECT_WITH_INVALID_COORDINATES\n");
        return 0;
    }
    if (lol == -2)  //soubor nema spravny format
    {
        printf("INVALID_FILE\n");
        return 0;
    }
    if (lol < pocet)  //program hleda vice shluku nez je celkem objektu
    {
        printf("TOO_FEW_OBJECTS\n");
        for (u = 0; u < lol; u++)  //cyklus na vycisteni clusteru
            clear_cluster(clusters + u);
        free(clusters);
        return 0;
    }


    int *c1 = malloc(sizeof(int));  //ukazatel pro fci find_neighbours
    int *c2 = malloc(sizeof(int));  //ukazatel pro fci find_neighbours
    if (lol == pocet);  //pokud je zadan stejny pocet objektu jako pocet pozadovanach shuluku, program je vypise a vycisti cluster
    else
    {
        while (lol != pocet)  //probiha dokud neni pocet hledanych clustru stejny jako pocet ziskanych clusteru
        {
            find_neighbours(clusters, lol, c1, c2);  //najde nejblizsi sousedy
            int s1 = *c1;
            int s2 = *c2;
            merge_clusters(&clusters[s1], &clusters[s2]);  //nahraje objekty z druheho souseda do prvniho (nejblizsi sousedi)
            lol = remove_cluster(clusters, lol, (s2));  //presune druheho souseda nakonec a posledni cluster vycisti
        }
    }


        print_clusters(clusters, lol);  //vytisknuti clusteru
        for (u = 0; u < lol; u++)  //cyklus na vycisteni clusteru
            clear_cluster(clusters + u);


    free(c1);
    free(c2);
    free(clusters);


    return 0;
}
