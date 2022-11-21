/*
 * Tabuľka s rozptýlenými položkami
 *
 * S využitím dátových typov zo súboru hashtable.h a pripravených kostier
 * funkcií implementujte tabuľku s rozptýlenými položkami s explicitne
 * zreťazenými synonymami.
 *
 * Pri implementácii uvažujte veľkosť tabuľky HT_SIZE.
 */

#include "ial_hash.h"
#include <stdlib.h>
#include <string.h>

int HT_SIZE = MAX_HT_SIZE;

/*
 * Rozptyľovacia funkcia ktorá pridelí zadanému kľúču index z intervalu
 * <0,HT_SIZE-1>. Ideálna rozptyľovacia funkcia by mala rozprestrieť kľúče
 * rovnomerne po všetkých indexoch. Zamyslite sa nad kvalitou zvolenej funkcie.
 */
int get_hash(char *key) {
  int result = 1, i = 0;

  while(key[i] != '\0'){
      result += key[i];
      i++;
  }
  return (result % MAX_HT_SIZE);
}

/*
 * Inicializácia tabuľky — zavolá sa pred prvým použitím tabuľky.
 */
void ht_init(ht_table_t *table) {
    for (int i = 0; i < MAX_HT_SIZE; i++){
        (*table)[i] = NULL;
    }
}

/*
 * Vyhľadanie prvku v tabuľke.
 *
 * V prípade úspechu vráti ukazovateľ na nájdený prvok; v opačnom prípade vráti
 * hodnotu NULL.
 */
ht_item_t *ht_search(ht_table_t *table, char *key) {
    if (table == NULL) {
        return NULL;
    }
    //index do tabulky
    int index = get_hash(key);
    ht_item_t *search = (*table)[index];

    //dokym neprejdeme vsetko
    while(search != NULL){
        //ak sa rovnaju
        if(search->key == *key){
            return search;
        }
    }
    return NULL;
}

/*
 * Vloženie nového prvku do tabuľky.
 *
 * Pokiaľ prvok s daným kľúčom už v tabuľke existuje, nahraďte jeho hodnotu.
 *
 * Pri implementácii využite funkciu ht_search. Pri vkladaní prvku do zoznamu
 * synonym zvoľte najefektívnejšiu možnosť a vložte prvok na začiatok zoznamu.
 */
void ht_insert(ht_table_t *table, char *key, element *data) {
    if (table == NULL) {
        return;
    }

    ht_item_t *item_exist = ht_search(table, key);

    if(item_exist!= NULL){
        item_exist->e = *data;
    }
    else{
        int index = get_hash(key);
        //alokujeme si pamat
        ht_item_t *item = malloc(sizeof (ht_item_t));
        if(item == NULL){
            callError(ERR_INTERNAL);
        }
        //priradime hodnoty do noveho pridaneho prvku
        memcpy(&item->e, data, sizeof(element));
        memcpy(&item->key, key, sizeof(*key));
        (*table)[index] = item;
    }
}

/*
 * Získanie hodnoty z tabuľky.
 *
 * V prípade úspechu vráti funkcia ukazovateľ na hodnotu prvku, v opačnom
 * prípade hodnotu NULL.
 *
 * Pri implementácii využite funkciu ht_search.
 */
element *ht_get(ht_table_t *table, char *key) {
  ht_item_t *item = ht_search(table, key);

  if(item != NULL){
    return &item->e;
  } else {
      return NULL;
  }
}

/*
 * Zmazanie všetkých prvkov z tabuľky.
 *
 * Funkcia korektne uvoľní všetky alokované zdroje a uvedie tabuľku do stavu po
 * inicializácii.
 */

void ht_delete_all(ht_table_t *table) {
  ht_item_t *active;

  if (table != NULL){
    for(int i = 0; i < HT_SIZE; i++){
      if(((*table)[i]) != NULL){
        active = (*table)[i];
        free(active);
        (*table)[i] = NULL;
      }
    } 
  } else {
      return;
  }
}