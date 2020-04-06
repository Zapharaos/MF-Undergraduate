# Description de l’agencement des pages
## Le site sera un site one-page :

* La page d’accueil « index.php » qui aura toujours :
    * Un « header » responsive avec le logo au centre, en-dessous des boutons pour accéder aux différentes sections : « accueil » - « login » - « signin » si aucune session n’est ouverte, sinon : « accueil » - « profil" - « add » - « déconnecter » . 
    * Un footer avec un formulaire de contact (déroulant) et en dessous les mentions légales, les réseaux sociaux et le choix de la langue (langue également enregistrée dans la base de donnée).
    
* La page « index.php » aura plusieurs sections variables, mais une seule à la fois.
    * Par défaut : une section « base » avec la liste des annonces. Il y aura un select pour laisser le choix à l’utilisateur de trier les annonces et un champ de recherche pour une annonce spécifique. Même section pour voir l’historique des soirées accéssible depuis le profil d'un utilisateur.
    * Une section « login » avec la possibilité de se connecter ou de cliquer sur des liens afin de créer un compte ou de récupérer ses identifiants (affichage d’autres sections).
    * Une section « forgot » pour demander une réinitialisation du mot de passe.
    * Une section « reset_pwd » pour réinitialiser son mot de passe.
    * Une section « signin » pour créer un compte.
    * Une section « add » pour ajouter ou éditer une annonce.
    * Une section « details » pour afficher les détails d’une annonce. Il pourra ajouter des annonces à ses favoris
    * Une section « coming » pour afficher les participants.
    * Une section « profil » pour modifier ses informations et son mot de passe (seulement si une session est ouverte que l’utilisateur clique sur son profil) et les informations sur un utilisateur (accessible de tous).
    
* L’utilisateur pourra : 
    * cliquer sur un autre profil et voir l’historique des soirées (participé, organisé).
    * Voir le nombre de places restantes et la proportion des sexes d’une annonce.
    * Recevoir la confirmation de son inscription par email avec un QR code joint au format pdf.

## Structure du projet

* Dossier `assets` : y mettre les fichiers `*.js`, `*.css`, `*.php` et toutes les images, dans dossiers consacrés.
* Dossier `templates` : y mettre tous les fichiers communs à toutes les pages (navbar, footer...).
* Page d'accueil = `index.php`
* Pour les autres pages, les organiser en fonction des urls voulues.

## Compte de test

* Email = admin@admin.admin
* Mot de passe = admin
