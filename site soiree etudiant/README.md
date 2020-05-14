# Description du site : Student Party
## Compte de test

* Email = a@a.a
* Mot de passe = admin

## Fonctionnalitées (sans compte, mais très limité)

* Voir la liste des soirées avec quelques informations (avec un menu de recherche et des sélections)
* Créer un compte et s'y connecter
* Mot de passe perdu (par mail) puis réinitialisation du mot de passe (sur le site)

## Fonctionnalitées (avec compte)

* Voir la liste des informations (avec un menu de recherche et des sélections particulières pour les utilisateurs)
* Ouvrir le profil de l'organisateur de la soirée dans un nouvel onglet
* Ouvrir une description détaillée de la soirée dans un nouvel onglet
* Voir la listes des participants de la soirée dans un nouvel onglet (depuis la page avec la description détaillée)
* Ajouter la soirée à ses favoris (depuis la page avec la description détaillée)
* S'inscire à la soirée (depuis la page avec la description détaillée) et recevoir la confirmation par email
* Créer une soirée
* Accéder à son profil et modifier son nom, prénom, adresse email ou mot de passe
* Accéder à son historique (d'organisation et de participation) depuis son profil
* Se déconnecter

## Fonctionnalitées globales

* Un formulaire de contact
* Les mentions légales (vide pour l'instant)
* Les réseaux sociaux (vide pour l'instant)
* Un changement de langue (seulement deux langues disponibles pour l'instant : anglais et français)

Note : elles ne semblaient plus fonctionner quand j'ai testé avant mon dernier commit, j'en suis désolé. Pourtant, ça n'était pas le cas il y a 2 mois.

## Commentaires

Je n'ai pas eu le temps d'ajouter toutes les fonctionnalitées auxquelles j'avais pensé. J'espère pouvoir les ajouter plus tard :
* régler le problème des librairies
* une vérification en temps réel des champs (lors de la connexion et de la création d'un compte)
* ajouter un graphique permettant d'indiquer à quel point l'utilisateur est actif
* ajouter un système de notation après une soirée
* ajouter un système de banissement (pour des étudiants un peu turbulant :p )
* déconnecter automatiquement l'utilisateur après un certain temps

## Annexes :

Je me suis servis des librairies suivantes :
* sendgrid-php : pour envoyer des emails
* phpqrcode : pour créer un qrcode (inscription à une soirée)
* fdpf17 : pour créer un pdf (à partir d'un qrcode pour l'envoyer par email)
* awesomplete : pour l'affichage des villes en ajax

# Description de l’agencement des pages
## Le site sera un site one-page :

* La page d’accueil « index.php » qui aura toujours :
    * Un « header » responsive avec le logo au centre, en-dessous des boutons pour accéder aux différentes sections : « accueil » - « login » - « signin » si aucune session n’est ouverte, sinon : « accueil » - « profil" - « add » - « déconnecter » . 
    * Un footer avec un formulaire de contact (déroulant) et en dessous les mentions légales, les réseaux sociaux et le choix de la langue (langue également enregistrée dans la base de donnée).
    
* La page « index.php » aura plusieurs sections variables, mais une seule à la fois :
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
