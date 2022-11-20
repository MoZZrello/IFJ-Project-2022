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
  int result = 1;
  int length = (int)strlen(key);
  for (int i = 0; i < length; i++) {
    result += key[i];
  }
  return (result % HT_SIZE);
}

/*
 * Inicializácia tabuľky — zavolá sa pred prvým použitím tabuľky.
 */
void ht_init(ht_table_t *table) {
    for (int i = 0; i < HT_SIZE; i++){
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
  ht_item_t *item = (*table)[get_hash(key)];

  for (;item; item = item->next){
    if (strcmp(item->key, key) == 0){
      return item;
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
void ht_insert(ht_table_t *table, char *key, element data) {
  if (table == NULL) return;

    int index = get_hash(key);
    ht_item_t *item = ht_search(table, key);

    if (item != NULL) {
        item->e = &data;
    }
    else {
        item = (ht_item_t *) malloc(sizeof(ht_item_t));
        item->e = &data;
        item->key = key;

        if ((*table)[index] != NULL) {
          item->next = (*table)[index];
        } else {
          item->next = NULL;
        }
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

  if(&(item->e->name) != NULL){
    return item->e;
  } else {
      return NULL;
  }
}

/*
 * Zmazanie prvku z tabuľky.
 *
 * Funkcia korektne uvoľní všetky alokované zdroje priradené k danému prvku.
 * Pokiaľ prvok neexistuje, nerobte nič.
 *
 * Pri implementácii NEVYUŽÍVAJTE funkciu ht_search.
 */
void ht_delete(ht_table_t *table, char *key) {
  int hashtag = get_hash(key);
  ht_item_t *NEXTitem = NULL;
  ht_item_t *PREVitem = NULL;
  ht_item_t *item = (*table)[hashtag];

  for(; item; PREVitem = item, item = item->next){
      NEXTitem = item->next;
      if(strcmp(key, item->key) == 0){
          free(item);
          if(!PREVitem){
              (*table)[hashtag] = NEXTitem;
              return;
          }
          PREVitem->next = NEXTitem;
          return;
      }
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
        active = active->next;
      }
    } 
  } else {
      return;
  }
}
