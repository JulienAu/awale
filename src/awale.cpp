#include <iostream>
#include <climits>
#include <ctime>
#include <algorithm>

#define SIZE 12 

 struct Position {
       int cases[SIZE*2]; // chaque case contient un nombre de
                                       // pions
       //int cases_ordi[SIZE];
       int ordi_joue; // boolean 1 si c'est à l'ordi de jouer et 0 sinon
       int pions_pris_joueur; // pions pris par le joueur
       int pions_pris_ordi; // pions pris par l'ordi
 };

 
int positionFinale(Position * pos_courante , int ordi_joue , int prof){
  int res = 0;
  if(ordi_joue){
    if(pos_courante->pions_pris_ordi > (SIZE * 4)){
      return 1;
    }
    for (int i = 0 ; i < SIZE ; i++){
        res += pos_courante->cases[i];
    }
    return !res;
    } else {
        if(pos_courante->pions_pris_joueur > (SIZE * 4)){
         return 1;
        }
        for (int i = SIZE ; i < SIZE * 2 ; i++){
          res += pos_courante->cases[i];
        }
       return !res;
    }
}

int evaluation(Position * pos_courante , int ordi_joue , int prof){
      return pos_courante->pions_pris_ordi - pos_courante->pions_pris_joueur;
}

int coupValide(Position * pos_courante , int ordi_joue , int i){
  if(ordi_joue){
    return pos_courante->cases[i];
  }
  return pos_courante->cases[SIZE+i];
}

int jouerCoup(Position * pos_next , Position * pos_courante , int ordi_joue , int i){
  // Position courante vers position next
  for(int x=0 ; x < SIZE * 2 ; x++){
    pos_next->cases[x]=pos_courante->cases[x];
  }

  pos_next->ordi_joue = !pos_courante->ordi_joue;

  pos_next->pions_pris_ordi = pos_courante->pions_pris_ordi;
  pos_next->pions_pris_joueur = pos_courante->pions_pris_joueur;

  //Distribue les pions
  if(!ordi_joue){
    i = i+SIZE;
  }
  int k = i;
  int j;
  for(j = 1 ; j <= pos_next->cases[i] ; j++){
    if(k+j >= SIZE * 2){
      k = k - SIZE * 2;
    }
    if(k+j == i){
     j++;
     pos_next->cases[i]++;
    }
    pos_next->cases[k+j]++;
  }
  // vide la cases jouée
  pos_next->cases[i] = 0 ;
  j--;
  // prend les pions gagnés
  if(ordi_joue){
    //j--;
    while(k+j>=SIZE && (pos_next->cases[k+j] == 2 || pos_next->cases[k+j]== 3) ){
      pos_next->pions_pris_ordi += pos_next->cases[k+j];
      pos_next->cases[k+j]=0;
      j--;
    }
  }else{
    //std::cout << " LA valeur de k+j est : " << k+j << std::endl;
    while(k+j>=0 && (pos_next->cases[k+j] == 2 || pos_next->cases[k+j]== 3) && k+j < SIZE){
      pos_next->pions_pris_joueur += pos_next->cases[k+j];
      pos_next->cases[k+j]=0;
      j--;
    }
  }
  int sum = 0;
  if(pos_next->ordi_joue){
    for(int i = 0 ; i < SIZE ; i++){
      sum += pos_next->cases[i];
    }
  }else{
    for(int i = SIZE ; i < SIZE*2 ; i++){
      sum += pos_next->cases[i];
    }
  }
  if(sum == 0){
    if(pos_next->pions_pris_ordi <=48){
      return 0;
    }
  }
  return 1 ;
}

int profondeurVariable(Position* pos_courante, int ordi_joue){
  int count = 0;
  if(ordi_joue){
    for(int i=0;i<SIZE;i++){
      if(pos_courante->cases[i] > 0){
        count++;
      }
    }
  }else {
    for(int i=SIZE;i<SIZE*2;i++){
      if(pos_courante->cases[i] > 0){
        count++;
      }
    }
  }
  return count;
}

int valeurMinMax(Position* pos_courante, int ordi_joue, int prof, int profMax , int alpha , int beta){
       // ordi_joue est un booleen qui est vrai si l'ordi joue
       int alpha_courant = alpha;
       int beta_courant = beta;
       int tab_valeurs[SIZE];
       //int egale = 0;
       Position pos_next; // En C on crée dans la pile = TRES rapide

       if (positionFinale(pos_courante, ordi_joue,prof)){
               // code à écrire
               // on retourne VALMAX (=48) si l'ordi gagne et -48 si l'ordi perd  et 0 si nul
              if (pos_courante->pions_pris_ordi > pos_courante->pions_pris_joueur){
                return SIZE * 8 - prof;
              } else if(pos_courante->pions_pris_ordi < pos_courante->pions_pris_joueur){
                return -SIZE * 8;
              }else{
                return 0;
              }
       }
       if (prof == profMax) {
               //std::cout<<"La profondeur Max est : " << profMax << std::endl;
               return evaluation(pos_courante, ordi_joue,prof);
               // dans un premier temps l'évaluation sera la
               // différence du nb de pions pris
       }
       if(!prof){
        
        clock_t begin = std::clock();
        int k= 0;
        for(int i=0;i<SIZE;i++){

          tab_valeurs[i]=INT_MIN;

          clock_t end = std::clock();
          double timeCourant = (double(end - begin) / CLOCKS_PER_SEC);
          if(timeCourant > 5 ){
            profMax = 7;
          }
          if( timeCourant > i/2 && i>=2){
            profMax--;
            //std::cout<<"La profondeur Max est : " << profMax << "pour la branche : " << i <<std::endl;
            k = 1;
          }/*else if ((double(end - begin) / CLOCKS_PER_SEC) < i/2 && i>=2){
            profMax++;
          }*/
          
          if(profMax < 19){
            if(i == 0 && profondeurVariable(pos_courante,ordi_joue) <= 2){
              profMax ++;
              //std::cout<<"La profondeur Max est : " << profMax << std::endl;
            }
           } 
                 // on joue le coup i
                 // ecrire la fn coupValide(pos_courante,ordi_joue,i)
                 // elle teste si on peut prendre les pions dans la
                 // cases i et les jouer (si pas de pion alors elle retourne invalide). La position de départ
                 // est pos_courante
                 if (coupValide(pos_courante,ordi_joue,i)){
                         // ecrire la fn :
                         // on joue le coup i a partir de la position
                         // pos_courante et on met le résultat
                         // dans pos_next
                         if(jouerCoup(&pos_next,pos_courante,ordi_joue,i)){
        // pos_next devient la position courante, et on change le joueur
                         
                         tab_valeurs[i]=valeurMinMax(&pos_next, !ordi_joue,prof+1,profMax,alpha_courant , beta_courant);
                       } else {
                          if (pos_courante->ordi_joue){
                            tab_valeurs[i]= -100;
                          }
                          else {
                            tab_valeurs[i] = 100;
                         }
                       }
                 } else {
                    if (pos_courante->ordi_joue){
                      tab_valeurs[i]= -100;
                       }
                    else {
                      tab_valeurs[i] = 100;
                       }
                 }
                 if(k){
                  profMax++;
                  k=0;
                 }
         }

       } else {
       if(!ordi_joue){
        int val = INT_MAX;
         for(int i=0;i<SIZE;i++){
           if(profMax < 19){
            if(i == 0 && profondeurVariable(pos_courante,ordi_joue) <= 2){
                profMax ++;
                //std::cout<<"La profondeur Max est : " << profMax << std::endl;
              }
            } 
                 // on joue le coup i
                 // ecrire la fn coupValide(pos_courante,ordi_joue,i)
                 // elle teste si on peut prendre les pions dans la
                 // cases i et les jouer (si pas de pion alors elle retourne invalide). La position de départ
                 // est pos_courante
                 if (coupValide(pos_courante,ordi_joue,i)){
                         // ecrire la fn :
                         // on joue le coup i a partir de la position
                         // pos_courante et on met le résultat
                         // dans pos_next
                         if(jouerCoup(&pos_next,pos_courante,ordi_joue,i)){
        // pos_next devient la position courante, et on change le joueur
                         val=std::min(val , valeurMinMax(&pos_next, !ordi_joue,prof+1,profMax , alpha_courant , beta_courant));
                         if(alpha_courant >= val){
                          //std::cout << "Je fais une coupe Min val vaut :" << val << " alpah vaut :" << alpha_courant << " beta vaut : "<< beta << std::endl;
                              return val;
                         }
                         beta_courant=std::min(beta_courant,val);
                         //tab_valeurs[i]=valeurMinMax(&pos_next, !ordi_joue,prof+1,profMax);
                       }
                 }
                }
                return val;
         
      }else{
         int val = INT_MIN;

         for(int i=0;i<SIZE;i++){
          if(profMax < 19){
            if(i == 0 && profondeurVariable(pos_courante,ordi_joue) <= 2){
              profMax ++;
              //std::cout<<"La profondeur Max est : " << profMax << std::endl;
              }
             } 
                 // on joue le coup i
                 // ecrire la fn coupValide(pos_courante,ordi_joue,i)
                 // elle teste si on peut prendre les pions dans la
                 // cases i et les jouer (si pas de pion alors elle retourne invalide). La position de départ
                 // est pos_courante
                 if (coupValide(pos_courante,ordi_joue,i)){
                         // ecrire la fn :
                         // on joue le coup i a partir de la position
                         // pos_courante et on met le résultat
                         // dans pos_next
                      if(jouerCoup(&pos_next,pos_courante,ordi_joue,i)){
        // pos_next devient la position courante, et on change le joueur
                         val=std::max(val , valeurMinMax(&pos_next, !ordi_joue,prof+1,profMax , alpha_courant , beta_courant));
                         if(val >= beta_courant){
                            //std::cout << "Je fais une coupe Min val vaut :" << val << " alpah vaut :" << alpha_courant << " beta vaut : "<< beta << std::endl;
                              return val;
                         }
                         alpha_courant=std::max(alpha_courant,val);
                         //tab_valeurs[i]=valeurMinMax(&pos_next, !ordi_joue,prof+1,profMax);
                       }
                 }
         }
         return val;
        }
      }
       int res = tab_valeurs[0];
       int indice = 0;
       if (ordi_joue){
              
              /*if(egale){
               for(int i = 1 ; i < SIZE ; i++){
                if(res < tab_valeurs[i]){
                  res = tab_valeurs[i];
                  indice = i;
                }
               }
              } else {*/
                for(int i = 1 ; i < SIZE ; i++){
                  if(res < tab_valeurs[i]){
                   res = tab_valeurs[i];
                   indice = i;
                  }
               }
             // }
       } else {
             /* if(egale){
               for(int i = 1 ; i < SIZE ; i++){
                if(res > tab_valeurs[i]){
                  res = tab_valeurs[i];
                  indice = i;
                }
               }
              }else{*/
                for(int i = 1 ; i < SIZE ; i++){
                  if(res > tab_valeurs[i]){
                    res = tab_valeurs[i];
                    indice = i;
                }
               }
             // }
       }       
      for(int k=0 ; k < SIZE ; k++){
        std::cout << " " << tab_valeurs[k] << " " ;
      }
      std::cout << std::endl;
      return indice;
}

void afficherPlateau(Position * pos){
  std::cout <<"Points du joueur : " << pos->pions_pris_joueur<< std::endl;
  for(int i = SIZE * 2 - 1  ; i >= SIZE ; i--){
    std::cout <<"|" << pos->cases[i] << "| ";
  }
  std::cout << "" << std::endl;
  for(int i = 0  ; i < SIZE ; i++){
    std::cout <<"|" << pos->cases[i] << "| ";
  }
    std::cout << "\n Points de l'ordi :" << pos->pions_pris_ordi << std::endl;
}


int main()
{
  int coupAdversaire;
  Position pos;
  pos.pions_pris_joueur=0;
  pos.pions_pris_ordi=0;
  

  for(int i = 0  ; i < SIZE * 2 ; i++){
    pos.cases[i] = 4;
  }

/*
  pos.cases[0] = 0;
  pos.cases[1] = 0;
  pos.cases[2] = 0;
  pos.cases[3] = 0;
  pos.cases[4] = 0;
  pos.cases[5] = 0;
  pos.cases[6] = 0;
  pos.cases[7] = 0;
  pos.cases[8] = 0;
  pos.cases[9] = 4;
  pos.cases[10] = 0;
  pos.cases[11] = 1;
  pos.cases[12] = 0;
  pos.cases[13] = 3;
  pos.cases[14] = 3;
  pos.cases[15] = 2;
  pos.cases[16] = 1;
  pos.cases[17] = 3;
  pos.cases[18] = 1;
  pos.cases[19] = 3;
  pos.cases[20] = 3;
  pos.cases[21] = 3;
  pos.cases[22] = 2;
  pos.cases[23] = 1;
  pos.pions_pris_ordi = 40;
  pos.pions_pris_joueur = 26;
  */

  std::cout << "Qui commence (joueur(0) / ordi(1)): "; 
  std::cin >> pos.ordi_joue;
  
  //std::cout << "profMax function :  " << profondeurVariable(&pos,pos.ordi_joue) << std::endl;

  Position pos_next;
  if(pos.ordi_joue){
    while(!positionFinale(&pos , pos.ordi_joue , 0)){
      clock_t begin = std::clock();
      int indice = valeurMinMax(&pos , pos.ordi_joue , 0 ,11 , INT_MIN , INT_MAX);
      clock_t end = std::clock();
      std::cout << "Temps: " << double(end - begin) / CLOCKS_PER_SEC << std::endl;
      std::cout << "L'ordi joue ==================>";
      std::cout << indice << std::endl;
      jouerCoup(&pos_next , &pos , pos.ordi_joue , indice);
      afficherPlateau(&pos_next);
      std::cout << "Enter le coup du joueur: "; 
      std::cin >> coupAdversaire;
      while(!coupValide(&pos_next , pos_next.ordi_joue , coupAdversaire)){
            std::cout << "Enter le coup du joueur: "; 
            std::cin >> coupAdversaire;
      }
      jouerCoup(&pos , &pos_next , pos_next.ordi_joue , coupAdversaire);
      afficherPlateau(&pos);
    }
  }else{
    while(!positionFinale(&pos , pos.ordi_joue , 0)){
      std::cout << "Enter le coup du joueur: "; 
      std::cin >> coupAdversaire;
      while(!coupValide(&pos , pos.ordi_joue , coupAdversaire)){
            std::cout << "Enter le coup du joueur: "; 
            std::cin >> coupAdversaire;
      }
      jouerCoup(&pos_next , &pos , pos.ordi_joue , coupAdversaire);
      afficherPlateau(&pos_next);
      clock_t begin = std::clock();
      int indice = valeurMinMax(&pos_next , pos_next.ordi_joue , 0 ,11 , INT_MIN , INT_MAX);
      clock_t end = std::clock();
      std::cout << "Temps: " << double(end - begin) / CLOCKS_PER_SEC << std::endl;
      std::cout << "L'ordi joue ==================>";
      std::cout << indice << std::endl;
      jouerCoup(&pos , &pos_next , pos_next.ordi_joue , indice);
      afficherPlateau(&pos);
      
    }    
  }

  return 0;
}

