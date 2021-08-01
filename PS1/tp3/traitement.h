

/** Configuration des traitements.

    Il faut appeler la fonction configuration avant d'appeler sieste()
    ou traitement().

    Il faut passer à cette fonction une unique chaîne de caractères.
    La fonction renvoie 0 si il y a un problème, ou 1 si tout est OK.

    La chaîne peut être de différentes formes :

    - "+slow" : le traitement d'un fichier prend environ une
      demi-seconde, et la fonction traitement() ne se plante jamais

    - "-slow" : idem, mais la fonction se plante au plus tard au 10e
      appel.

    - "+fast" : le traitement d'un fichier prend environ dix
      milli-secondes, et la fonction traitement() ne se plante jamais

    - "-fast" : idem, mais la fonction se plante au plus tard au 10e
      appel.

    - "nnn:ppp" (où "nnn" et "ppp" sont des entiers quelconques)~:
      dans ce cas, le traitement d'un fichier prend "nnn"
      millisecondes, et la fonction traitement() se plante au plus
      tard au "ppp"-ème appel. Si "ppp" est égal à 0, la fonction ne
      se plante jamais.

    Notez que le nombre (exact) d'appels avant plantage est tiré
    aléatoirement, vous ne pouvez donc pas prévoir à quel appel elle
    va se planter. Si vous utilisez "...:1", la fonction se plantera
    au premier appel.

    \param config Une chaîne de config (voir ci-dessus)

    \returns 1 si la configuration est réussie, 0 si vous devez mieux
    lire la documentation ci-dessus.
*/
int configuration (const char * config);


/** Traitement d'un fichier.

    Cette fonction effectue le traitement d'un fichier dont le nom est
    passé en argument. Le fichier doit exister : si ce n'est pas le
    cas, elle affiche un message sur l'erreur standard.

    À chaque appel de cette fonction, il peut se passer deux choses :

    - La fonction renvoie 1 ou 0 selon que le traitement a réussi ou
      échoué. Ce résultat est tiré aléatoirement, avec une probabilité
      de succès de 75%.

    - La fonction provoque un arrêt anormal du processus, par appel de
      abort(), après s'être assurée qu'aucun fichier "core" n'est
      produit. Lisez le manuel de abort(3) ainsi que signal(7) si vous
      voulez connaître les détails de ce mécanisme.

    Le traitement à proprement parler, si il a lieu, prend un temps
    fixé par la configuration (par défaut une semaine !), temps
    pendant lequel elle ne fait rien.

    \param name Le nom du fichier à traiter

    \returns 1 en cas de succès, 0 en cas d'échec
*/
int traitement (const char * name);


/** Sieste.

    Cette fonction ne fait rien, mais prend pour cela un temps
    similaire à celui d'un traitement.
*/
void sieste ();
