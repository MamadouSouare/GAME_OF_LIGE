Je suis un débutant en programmation passionné par la bibliothèque SDL2, et j'ai réalisé en une matinée une version du "Jeu de la Vie" de John Conway. Mon programme n'est ni trop simple ni optimisé, mais il fonctionne et permet de visualiser l'évolution de cellules sur une grille.

### Règles du Jeu de la Vie
Le Jeu de la Vie est un automate cellulaire inventé par le mathématicien John Conway. Il se joue sur une grille où chaque cellule peut être vivante (1) ou morte (0). À chaque itération, l'état des cellules change selon les règles suivantes :

1. Une cellule vivante avec moins de 2 voisins vivants meurt (isolement).
2. Une cellule vivante avec 2 ou 3 voisins vivants survit.
3. Une cellule vivante avec plus de 3 voisins vivants meurt (surpopulation).
4. Une cellule morte avec exactement 3 voisins vivants devient vivante (naissance).

Dans ma version, j'ai modifié certaines règles pour tester différentes variantes et observer des motifs différents.

### Présentation du Code
Mon programme est codé en C avec SDL2 et fonctionne de la manière suivante :

- **Affichage avec SDL2** : Le jeu utilise SDL2 pour représenter la grille et les cellules.
- **Gestion des événements** : L'utilisateur peut interagir avec la simulation en activant ou désactivant la grille, en mettant en pause la simulation ou en réinitialisant la matrice.
- **Mise à jour des générations** : Une fonction calcule la prochaine génération de cellules en appliquant les règles.
- **Dessin de la matrice** : Une boucle principale met à jour l'affichage à chaque itération.

Deux fonctions permettent d'éditer la grille, mais elles le font de manière différente. L'une modifie directement la matrice, tandis que l'autre utilise une approche plus progressive.

### Points à Améliorer
Bien que fonctionnel, mon programme pourrait être optimisé. Actuellement :
- La vérification des voisins et la mise à jour de la matrice ne sont pas les plus efficaces.
- L'affichage peut être amélioré avec des animations plus fluides.
- L'interface pourrait inclure un meilleur contrôle utilisateur avec des boutons.


### Commandes Utilisateur
Le programme intègre plusieurs touches permettant d'interagir avec la simulation :
- **Q** : Ferme la fenêtre.
- **G** : Affiche ou masque la grille.
- **P** : Met en pause l'évolution de l'automate.
- **R** : Permet d'éditer la matrice (attention : l'édition n'est possible que lorsque le programme est en pause, il faut donc mettre en pause avant de modifier la grille).

Ce projet m'a permis de mieux comprendre SDL2 et la gestion d'une simulation en temps réel. J'aimerais continuer à l'améliorer en intégrant des optimisations et de nouvelles fonctionnalités. J'attends vos retours pour des améliorations potentielles !
