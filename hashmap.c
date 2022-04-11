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
  int cont;
  long tamanioOriginal;
  
  auxiliar = map->buckets;
  tamanioOriginal = map->capacity;
  
  map->capacity = map->capacity * 2;
  map->buckets = (Pair**)calloc(map->capacity, sizeof(Pair*));
  map->size = 0;
  cont = 0;
  
  while(cont < tamanioOriginal)
  {
    if(auxiliar[cont] != NULL && auxiliar[cont]->key != NULL)
      insertMap(map, auxiliar[cont]->key, auxiliar[cont]->value); 
    cont++;
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
  if(searchMap(map,key) != NULL)
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
  long idx = 0;
  while(idx < map->capacity)
  {
    if(map->buckets[idx] != NULL && map->buckets[idx]->key !=NULL)
    {
      map->current = idx;
      return map->buckets[idx];
    }
    idx = idx + 1;
  }
  return NULL;
}

Pair * nextMap(HashMap * map) 
{ 
  long idx = (map->current + 1) ;
  while(idx < map->capacity - 1)
  {
    if(map->buckets[idx] != NULL && map->buckets[idx]->key != NULL)
    {
      map->current = idx;
      return map->buckets[idx];
    }
    idx = idx + 1;
  }
  return NULL;
}

