#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "hashmap.h"


typedef struct HashMap HashMap;
int enlarge_called=0;

struct HashMap {
    Pair ** buckets;
    long size; //cantidad de datos/pairs en la tabla
    long capacity; //capacidad de la tabla
    long current; //indice del ultimo dato accedido
};

Pair * createPair( char * key,  void * value) {
    Pair * new = (Pair *)malloc(sizeof(Pair));
    new->key = key;
    new->value = value;
    return new;
}

long hash( char * key, long capacity) {
    unsigned long hash = 0;
     char * ptr;
    for (ptr = key; *ptr != '\0'; ptr++) {
        hash += hash*32 + tolower(*ptr);
    }
    return hash%capacity;
}

int is_equal(void* key1, void* key2){
    if(key1==NULL || key2==NULL) return 0;
    if(strcmp((char*)key1,(char*)key2) == 0) return 1;
    return 0;
}


void insertMap(HashMap * map, char * key, void * value) {
    long idx = hash(key, map->capacity );
    while(map->buckets[idx]!=NULL && map->buckets[idx]->key != NULL){
        if(is_equal(key, map->buckets[idx]->key)) return;
        idx = (idx + 1) % map->capacity;
    }

    if(map->buckets[idx] != NULL){
        map->buckets[idx]->key = key;
        map->buckets[idx]->value = value;
    }
    else{
        map->buckets[idx] = createPair(key, value);
    }
    map->size += 1;
    
}

void enlarge(HashMap * map) 
{
  enlarge_called = 1; //no borrar (testing purposes)
  Pair **auxiliar;
  int cont = hash(map->buckets[0]->key, map->capacity);
  map->capacity = map->capacity * 2;
  auxiliar = (Pair**) calloc(map->capacity,sizeof(Pair*));
  while(map->buckets[cont]!=NULL && map->buckets[cont]->key != NULL)
  {
    auxiliar[cont] = createPair(map->buckets[cont]->key, map->buckets[cont]->value);
    cont = (cont + 1) % map->capacity;
  }
  map->size = 0;
  cont = hash(map->buckets[0]->key, map->capacity);
  map = createMap(map->capacity);
  while(cont)
  {
    insertMap(map, auxiliar[cont]->key, auxiliar[cont]->value);
    cont ++;
  }
  
}


HashMap * createMap(long capacity) {
    HashMap* nuevoMapa = (HashMap*) malloc(sizeof(HashMap));
    nuevoMapa->buckets = (Pair**) calloc(capacity,sizeof(Pair*));
    nuevoMapa->capacity = capacity;
    nuevoMapa->size = 0;
    nuevoMapa->current = -1;
    return nuevoMapa;
}

void eraseMap(HashMap * map,  char * key) 
{
  if(searchMap(map,key))
  {
    map->buckets[map->current]->key = NULL;
    map->size -= 1; 
  }
  /*long idx = hash(key, map->capacity );
  while(map->buckets[idx]!=NULL && map->buckets[idx]->key != NULL){
    if(is_equal(key, map->buckets[idx]->key))
    {
      map->size = map->size - 1;
      map->current = idx;
      map->buckets[idx]->key = NULL;
    }
    idx = (idx + 1) % map->capacity;
  }*/
}

Pair * searchMap(HashMap * map,  char * key) 
{
  long idx = hash(key, map->capacity );
  while(map->buckets[idx]!=NULL && map->buckets[idx]->key != NULL){
      if(is_equal(key, map->buckets[idx]->key))
      {
          map->current = idx;
          return map->buckets[idx];
      }
      idx = (idx + 1) % map->capacity;
  }
  return NULL;   

}

Pair * firstMap(HashMap * map) 
{
  map->current = 0;
  while(map->current < map->capacity)
  {
    if(map->buckets[map->current] != NULL)
    {
      map->current = map->current;
      return map->buckets[map->current];
    }
    map->current = (map->current + 1) % map->capacity;
  }
  return NULL;
}

Pair * nextMap(HashMap * map) 
{ 
  long idx = (map->current + 1) % map->capacity;
  while(idx < map->capacity)
  {
    if(map->buckets[idx] != NULL)
    {
      map->current = idx;
      return map->buckets[idx];
    }
    idx = (idx + 1) % map->capacity;
  }

  return NULL;
}

