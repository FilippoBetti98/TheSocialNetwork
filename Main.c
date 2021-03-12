#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>


#define LEN 40

typedef struct node {
  char name[LEN];
  struct tree* link;
  int color;
  int counter;
  struct node *l;
  struct node *r;
  struct node *p;
}node_t;

typedef struct tree {
  node_t* root;
  node_t* nil;
}tree_t;

tree_t* createtree();
void fixup(tree_t*, node_t*);
void leftrotate(tree_t*, node_t*);
void rightrotate(tree_t*, node_t*);
void addent(tree_t*, char*);
void StampaAlbero(node_t*, tree_t*);
void addrel(char*, char*, char*);
node_t* search(tree_t*, node_t*, char*);
void report(node_t*, tree_t*);
void reportdest(node_t*, tree_t*);
void freetree(node_t*, tree_t*);
void delrel(char*, char*, char*);
void elim(tree_t*, node_t*);
node_t* min(tree_t*, node_t*);
node_t* succ(tree_t*, node_t*);
void deletefixup(tree_t*, node_t*);
void delent(char*);
void passaggio1(tree_t*, node_t*, char*);
void passaggio2(tree_t*, node_t*, char*);
node_t* checkrel(tree_t*, node_t*);
void check(tree_t*, node_t*);
void checkrec(tree_t*, node_t*);



/*
void recursion(tree_t*, node_t*, char*);
void passaggio(tree_t*, node_t*, char*, int);
void fixcounter(tree_t* tree, node_t* node);
*/

tree_t* ent;
tree_t* rel;
int i = 0;
int max = 0;
char* print[LEN];

//Da provare: Report, Addrel, Controlli prima di aver inserto o in un punto in cui ancora non ho inserito, Strcmp con un parametro a null;
//Forse ROTATE/FIXUP ed altre con alberi vuoti danno problemi ???

int main(int argc, char const *argv[]) {

    char t, id_dest[LEN], id_rel[LEN], id_ent[LEN], input[3*LEN];
    char* s= " ";

    char *token;


    ent = createtree();
    rel = createtree();
    //freopen ("batch1.2.txt", "r", stdin);

    //acquisisco istruzioni e lancio

    while(1){
        fgets(input, 3*LEN, stdin);
        token = strtok(input, s);
        t= *token;

        switch (t) {
            case 'e':
                /*freetree(rel->root, rel);
                freetree(ent->root, ent);
                */
                 return 0;
            case 'r':
                if(rel->root != rel->nil) {
                    report(rel->root, rel);
                    fputs("\n", stdout);
                }
                else fputs("none\n", stdout);
                break;
            case 'a':
                if (strcmp(token, "addent") == 0) {
                    token = strtok(NULL, s);
                 /*   if(strcmp(token, "\"") == 0){
                        strcpy(id_ent, "\" \"");
                        id_ent[strlen(token) - 1] = 0;   In caso di stringa vuota ?
                        addent(ent, id_ent);
                    }*/
                    strcpy(id_ent, token);
                    id_ent[strlen(token) - 1] = 0;
                    addent(ent, id_ent);
                }else{
                    token = strtok(NULL, s);
                    strcpy(id_ent, token);
                    token = strtok(NULL, s);
                    strcpy(id_dest, token);
                    token = strtok(NULL, s);
                    strcpy(id_rel, token);
                    id_rel[strlen(token) - 1] = 0;
                    addrel(id_ent, id_dest, id_rel);
                }
                break;
            case 'd':
                if(strcmp("delent", token) == 0){
                    token = strtok(NULL, s);
                    strcpy(id_ent, token);
                    id_ent[strlen(token) - 1] = 0;
                    delent(id_ent);
                }
                else {
                    token = strtok(NULL, s);
                    strcpy(id_ent, token);
                    token = strtok(NULL, s);
                    strcpy(id_dest, token);
                    token = strtok(NULL, s);
                    strcpy(id_rel, token);
                    id_rel[strlen(token) - 1] = 0;
                    delrel(id_ent, id_dest, id_rel);
                }
                break;
            /*case 's':
                StampaAlbero(ent->root, ent);
                fputs("\n\n\n", stdout);
                StampaAlbero2(rel->root, rel);
                */
            default:
                ;
           }
    }
}

void addent(tree_t* enti, char id_ent[]){
    if(enti->root != enti->nil)
        if(search(enti, enti->root, id_ent) != enti->nil)
            return;

    node_t* elem = malloc(sizeof(node_t));
    node_t* y, *x;

    strcpy(elem->name, id_ent);

    y = enti->nil;
    x = enti->root;


    while(x != enti->nil){
    y = x;
    if(strcmp(elem->name, x->name) < 0)
      x = x->l;
    else x = x->r;
  }
  elem->p = y;
  if(y == enti->nil)
    enti->root = elem;
  else{
    if(strcmp(elem->name, y->name) < 0)
      y->l = elem;
    else y->r = elem;
  }
  elem->l = enti->nil;
  elem->r = enti->nil;
  elem->color = 1;
  elem->counter = -1;
  fixup(enti, elem);
}

void fixup(tree_t* enti, node_t* elem){
  node_t *x, *y;
/*
  x = malloc(sizeof(node_t));
  y = malloc(sizeof(node_t));
*/
  if(enti->root != enti->nil && elem != NULL) {
      if (elem == enti->root) {
          enti->root->color = 0;
      } else {
          x = elem->p;
          if (x->color == 1) {
              if (x == x->p->l) {
                  y = x->p->r;
                  if (y->color == 1) {
                      x->color = 0;
                      y->color = 0;
                      x->p->color = 1;
                      fixup(enti, x->p);
                  } else {
                      if (elem == x->r) {
                          elem = x;
                          leftrotate(enti, elem);
                          x = elem->p;
                      }
                      x->color = 0;
                      x->p->color = 1;
                      rightrotate(enti, x->p);
                  }
              } else {
                  y = x->p->l;
                  if (y->color == 1) {
                      x->color = 0;
                      y->color = 0;
                      x->p->color = 1;
                      fixup(enti, x->p);
                  } else {
                      if (elem == x->l) {
                          elem = x;
                          rightrotate(enti, elem);
                          x = elem->p;
                      }
                      x->color = 0;
                      x->p->color = 1;
                      leftrotate(enti, x->p);
                  }
              }
          }
      }
  }
}

void leftrotate(tree_t* tr, node_t* ele){
  if(tr->root != tr->nil && ele != NULL) {
      node_t* y = ele->r;
      ele->r = y->l;
      if (y->l != tr->nil)
          y->l->p = ele;
      y->p = ele->p;
      if (ele->p == tr->nil)
          tr->root = y;
      else {
          if (ele == ele->p->l)
              y->p->l = y;
          else
              y->p->r = y;
      }
      y->l = ele;
      ele->p = y;
  }
}

void rightrotate(tree_t* tr, node_t* ele){

  if(tr->root != tr->nil && ele != NULL) {
      node_t* y = ele->l;
      ele->l = y->r;
      if (y->r != tr->nil)
          y->r->p = ele;
      y->p = ele->p;
      if (ele->p == tr->nil)
          tr->root = y;
      else {
          if (ele == ele->p->r)
              y->p->r = y;
          else
              y->p->l = y;
      }
      y->r = ele;
      ele->p = y;
  }
}

tree_t* createtree(){
  tree_t* t= malloc(sizeof(tree_t));
  t->root = malloc(sizeof(node_t));
  /*
  if(!t) {
        fprintf(stderr,"Errore di allocazione A\n");
        exit(-1);
  }
  if(!t->root) {
        fprintf(stderr,"Errore di allocazione B\n");
        exit(-2);
  }
*/
  t->nil = t->root;
  t->nil->l = t->nil->r= t->nil->p = t->nil;
  t->nil->color = 0;
  return t;
}

void StampaAlbero(node_t* a, tree_t* t) {
    if(a!=t->nil) {
        StampaAlbero(a->l, t);
        fputs(a->name, stdout);
        StampaAlbero(a->r, t);
    }
}
/*
void StampaAlbero2(node_t* node, tree_t* tree){
    if(node != tree->nil){


    }
}
*/
void addrel(char* id_ent, char* id_dest, char* id_rel){
  node_t* found, *dest, *useless;
  tree_t* ric;

  found = search(ent, ent->root, id_ent);
  dest = search(ent, ent->root, id_dest);

  if((found != ent->nil) && (dest != ent->nil)){
    if((found = search(rel, rel->root, id_rel)) == rel->nil){
      addent(rel, id_rel);  //creo Rel

      useless = search(rel, rel->root, id_rel);
      ric = createtree();
      useless->link = ric; //creo albero ruceventi

      addent(useless->link, id_dest); //aggiungo all'albero riceventi appena creato
      useless->link->root->counter = 1;

      ric = createtree();
      useless->link->root->link = ric; //cre

      addent(ric, id_ent); // aggiungo all'albero dei mandanti appena creato
    }else{
      if(found->link->root == found->link->nil){              //c'è l'albero e non la trovo o manca proprio l'albero ? if(...)

        ric = createtree();
        found->link = ric;
        found->link->root->counter = 1;
        addent(found->link, id_dest);//NON C'È, creo AE1 e AE2, E1, E2

        ric = createtree();

        found->link = ric;
        addent(found->link->root->link, id_ent);
      }else{
        if((dest = search(found->link, found->link->root, id_dest)) == found->link->nil){
          //creo E2
          addent(found->link, id_dest);//c'è l'albero e ce lo devo aggiungere
          useless = (search(found->link, found->link->root, id_dest));
          useless->counter = 1;

          ric = createtree();
          useless->link = ric;
          addent(ric, id_ent);
        }else{
          if((search(dest->link, dest->link->root, id_ent)) == dest->link->nil){ //SBAGLIATO: c'è id_ent nell'albero delle origini?
            addent(dest->link, id_ent);
            dest->counter++;
            //AGGIUNGO id_ent nell'albero delle origini (ci dovrebbe essere visto che c'è quello delle destinazioni)
          }else{
            //non faccio niente perchè c'era già tutto
          }
        }
      }
    }
  }
//non faccio niente (primo if)
}

node_t* search(tree_t* tree, node_t* tr, char id_ent[]){
    int val = 0;

    if(tr == tree->nil)
        return tree->nil;

    val = strcmp(tr->name, id_ent);

    if(val == 0)
        return tr;
    if(val < 0)
        return search(tree, tr->r, id_ent);
    return search(tree, tr->l, id_ent);
}

void report(node_t* x, tree_t* t){
  int k;

  if(x != t->nil) {
      report(x->l, t);
      fputs(x->name, stdout);
      fputs(" ", stdout);
      max = 0;
      reportdest(x->link->root, x->link);
      for (k = 0; k <= i; k++) {
          fputs(print[k], stdout);
          fputs(" ", stdout);
          print[k] = NULL;
      }
      printf("%d; ", max);
      max = 0;

      report(x->r, t);
  }
}

void reportdest(node_t* node, tree_t* tree){

  if(node != tree->nil){
    reportdest(node->l, tree);
    if(node->counter > max){
      i = 0;
      print[i] = node->name;
      max = node->counter;
    }else if(node->counter == max) {
        i++;
        print[i] = node->name;
    }
    reportdest(node->r, tree);
  }
}

void freetree(node_t* node, tree_t* tree){

  if(node != tree->nil && node != NULL){
    freetree(node->l, tree);
    freetree(node->r, tree);
    /*if(node->link != NULL || node->link->root != NULL)
      freetree(node->link->root, node->link);
    */
     if(node == tree->root) {
        free(tree->nil);
        free(tree);
    }
    free(node);
  }
}

void delrel(char* id_ent, char* id_dest, char* id_rel){
  node_t *foundrel, *foundest, *foundent;

  if((foundrel = search(rel, rel->root, id_rel)) != rel->nil){
    if((foundest = search(foundrel->link, foundrel->link->root, id_dest)) != foundrel->link->nil){
      if((foundent = search(foundest->link, foundest->link->root, id_ent)) != foundest->link->nil){
        elim(foundest->link, foundent);
        foundest->counter--;
        if(foundest->link->root == foundest->link->nil){
          elim(foundrel->link, foundest);//Diminuire contaotre
          if(foundrel->link->root == foundrel->link->nil){
            elim(rel, foundrel);
          }
        }
      }
    }
  }
}

void elim(tree_t* T, node_t* z){ //Forse devo modificare tree_t **
  node_t *y, *x;
  if(z != NULL && T->nil != T->root) {
      if (z->l == T->nil || z->r == T->nil)
          y = z;
      else y = succ(T, z);
      if (y->l != T->nil)
          x = y->l;
      else x = y->r;
      x->p = y->p;
      if (y->p == T->nil)
          T->root = x;
      else if (y == y->p->l)
          y->p->l = x;
      else y->p->r = x;
      if (y != z) {
          strcpy(z->name, y->name);
          z->counter = y->counter;
          z->link = y->link;
      }
      if (y->color == 0)
          deletefixup(T, x);
      free(y);
  }
}

void deletefixup(tree_t* T, node_t* x){
  node_t *w;

  if(x->color == 1 || x->p == T->nil)
    x->color = 0;
  else
    if(x == x->p->l){
      w = x->p->r;
      if(w->color == 1){
        w->color = 0;
        x->p->color = 1;
        leftrotate(T, x->p);
        w = x->p->r;
      }
      if(w->l->color == 0 && w->r->color == 0){
        w->color = 1;
        deletefixup(T, x->p);
      }
      else{
       if(w->r->color == 0){
         w->l->color = 0;
         w->color = 1;
         rightrotate(T, w);
         w = x->p->r;
        }
        w->color = x->p->color;
        x->p->color = 0;
        w->r->color = 0;
        leftrotate(T, x->p);
      }
  } else {
    w = x->p->l;
    if(w->color == 1){
      w->color = 0;
      x->p->color = 1;
      rightrotate(T, x->p);
      w = x->p->l;
    }
    if(w->r->color == 0 && w->l->color == 0){
      w->color = 1;
      deletefixup(T, x->p);
    }
    else{
     if(w->l->color == 0){
       w->r->color = 0;
       w->color = 1;
       leftrotate(T, w);
       w = x->p->l;
      }
      w->color = x->p->color;
      x->p->color = 0;
      w->l->color = 0;
      rightrotate(T, x->p);
    }
  }
}

node_t* succ(tree_t* tree, node_t* x){
  node_t* y;

  if(x->r != tree->nil)
      return min(tree, x->r);
  y = x->p;
  while(y != tree->nil && y->r == x){
      x = y;
      y = y->p;
  }
  return y;
}

node_t* min(tree_t* tree, node_t* x){
    node_t* cur = x;
    while (cur->l != tree->nil) {
        cur = cur->l;
    }
    return cur;
}













void delent(char* id_ent){
    node_t* foundent, *deleterel;

  if((foundent = search(ent, ent->root, id_ent)) != ent->nil) {
    passaggio1(rel, rel->root, id_ent);
    checkrec(rel, rel->root);
    check(rel, rel->root);
    while((deleterel = checkrel(rel, rel->root)) != rel->nil)
       elim(rel, deleterel);
    elim(ent, foundent);
  }// CONTROLLA SE QUALCHE ELEMENTO HA ROOT == NULL
}

void passaggio1(tree_t* tree, node_t* node, char* id_ent){
  node_t *delete;

  if(node != tree->nil){
    passaggio1(tree, node->l, id_ent);
    passaggio1(tree, node->r, id_ent);
    if((delete = search(node->link, node->link->root, id_ent)) != node->link->nil){
      freetree(delete->link->root, delete->link);
      elim(node->link, delete);
      passaggio2(node->link, node->link->root, id_ent);
    }else
    passaggio2(node->link, node->link->root, id_ent);
    /*check(node->link, node->link->root);
    check(rel, rel->root);
    while((deleterel = checkrel(rel, rel->root)) != rel->nil)
      elim(rel, deleterel);*/
  }
}

void passaggio2(tree_t* tree, node_t* node, char* id_ent){
  node_t* delete;

  if(node != tree->nil){
    passaggio2(tree, node->l, id_ent);
    passaggio2(tree, node->r, id_ent);
    if(strcmp(node->name, id_ent) != 0) {
        if ((delete = search(node->link, node->link->root, id_ent)) != node->link->nil) {
            elim(node->link, delete);
            node->counter--;
        }
    }
  }
}

node_t* checkrel(tree_t* tree, node_t* node){
  node_t* contr = tree->nil;

  if(node != tree->nil){
    contr = checkrel(tree, node->l);
    if(contr != tree->nil)
      return contr;
    contr = checkrel(tree, node->r);
    if(contr != tree->nil)
      return contr;
    if(node->link->root == node->link->nil || node->link == NULL)
      return node;
  }
  return contr;
}

void check(tree_t* tree, node_t* node){

  if(node != tree->nil){
    check(tree, node->l);
    check(tree, node->r);
    if(node->link->root == node->link->nil || node->link == NULL) {
        freetree(node->link->root, node->link);
        elim(tree, node);
    }
  }
}

void checkrec(tree_t* tree, node_t* node){
  if(node != tree->nil){
    checkrec(tree, node->l);
    checkrec(tree, node->r);
    check(node->link, node->link->root);
  }
}















/*
node_t* del[50];

void delent(char* id_ent){
    node_t* foundent, *delete = NULL;
    int k = 0;

    if((foundent = search(ent, ent->root, id_ent)) != ent->nil){
    elim(ent, foundent);
    passaggio(rel, rel->root, id_ent, k);
    for(k = 0; del[k] != rel->nil && del[k] != NULL; k++)
        elim(rel, del[k]);
  }
}

void passaggio(tree_t* tree, node_t* node, char* id_ent, int k){
  node_t *ris = tree->nil;
  node_t* delete;

  if(node != tree->nil){
      passaggio(tree, node->l, id_ent, k);
      passaggio(tree, node->r, id_ent, k);
      if((delete = search(node->link, node->link->root, id_ent)) != node->link->nil) {
          freetree(delete->link->root, delete->link);
          elim(node->link, delete);
      } else { //QUI
          recursion(node->link, node->link->root, id_ent);
          if (node->link->nil == node->link->root) {
              del[k] = node;
              k++;
              del[k] = tree->nil;
          }
          //fixcounter(tree, node);
      }
  }
}


  if(node->counter == 0)
    elim(tree, node);



void recursion(tree_t* tree, node_t* node, char* id_ent){
  //int doit = 0;

  if(node != tree->nil){
      recursion(tree, node->l, id_ent);
      recursion(tree, node->r, id_ent);
      //inutile if(strcmp(node->name, id_ent) == 0) {
          if (node->link != NULL && node->link->root != node->link->nil) {
              node_t *found = search(node->link, node->link->root, id_ent);
              if (found != node->link->nil) {
                  elim(node->link, found);
                  if(node->link->nil == node->link->root)
                      elim(tree, node);
                  else
                      node->counter--;
              }
          }

  }
}


//    else{
        node->counter = 0;
    }
//}else {
    if(node->link->root != NULL) {
        node_t *found = search(node->link, node->link->root, id_ent);
        if (found != node->link->nil) {
            elim(node->link, found);
            node->counter--;
        }
    }
//}





void fixcounter(tree_t* tree, node_t* node) {
    //node_t *ris = tree->nil;
    //node_t *ris2 = tree->nil;

    if (node != tree->nil) {
        fixcounter(tree, node->l);
        fixcounter(tree, node->r);
//
        if(ris != tree->nil)
            freetree(ris->link->root, ris->link);

        if(ris2 != tree->nil)
            freetree(ris2->link->root, ris2->link);

        if (node->counter == 0) {
            freetree(node->link->root, node->link);
            freetree(tree->root, tree);
        }
    }

}
//funziona solo se c'è al massimo uno zero
if (node == tree->nil && node->link->root == node->link->nil){
  freetree(node->link->root, node->link);
  freetree(node, tree);
}

*/
