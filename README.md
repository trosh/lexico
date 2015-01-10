Analyse de champs lexicaux pour notre projet de M1 en MIHPS 2014/2015
=====================================================================

Rolih Meynard, Yann Kergutuil, John Gliksberg

Dépendances
-----------

- unac

		sudo apt-get install libunac1-dev

Usage
-----

Utiliser `make` pour effectuer différentes opérations basiques

- `make [all]` pour compiler
- `make clean` pour supprimer les fichiers `.o`
- `make purge` pour supprimer tout ce que `make all` crée et
  les fichiers qu'on a pu générer avec les exécutables
- `make test` pour effectuer un cas test avec un fichier wiki test
- `make 1` idem avec `wiki/1.txt`

Les programmes qu'on génère sont :

- `char` qui donne des infos basiques sur les strings en `argv`

- `splitwiki` pour splitter un fichier type wiki en un fichier par
  article dans `files/[nom article]`

- `noac` désaccentue le contenu et le nom des fichiers
  (le fichier de sortie est `noac[nom fichier]`;
  par exemple `noacfiles/[nom article desaccentue]`)

- `decoupe` initialise un dictionnaire avec `init_dico` puis effectue
  pour chaque fichier en argument :

  - remplit une liste de ( mots + occurences dans le fichier ) avec
    `decoupe_fichier`
  - l'ajoute au dictionnaire avec `ajoute_dico`

  puis transforme les nombre d'occurences en score `float`
  et affiche le dictionnaire avec `affiche_dico`
