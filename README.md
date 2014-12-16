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
