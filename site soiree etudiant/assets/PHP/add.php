<?php
    if(isset($_REQUEST['add_btn'])) {

        if (isset($_POST['add_name'])
            && isset($_POST['add_ville'])
            && isset($_POST['add_adresse'])
            && isset($_POST['add_prix'])
            && isset($_POST['add_place'])
            && isset($_POST['add_org'])
            && isset($_POST['add_description'])
            && isset($_POST['add_day'])
            && isset($_POST['add_month'])
            && isset($_POST['add_year'])
            && isset($_POST['add_hour'])
            && isset($_POST['add_min'])
            ){
            
            $add_name = htmlspecialchars($_POST['add_name']);
            $add_ville = htmlspecialchars($_POST['add_ville']);
            $add_adresse = htmlspecialchars($_POST['add_adresse']);
            $add_prix = htmlspecialchars($_POST['add_prix']);
            $add_place = htmlspecialchars($_POST['add_place']);
            $add_org = htmlspecialchars($_POST['add_org']);
            $add_orgID = $_SESSION['Auth']['id'];
            $add_description = htmlspecialchars($_POST['add_description']);
            $add_day = htmlspecialchars($_POST['add_day']);
            $add_month = htmlspecialchars($_POST['add_month']);
            $add_year = htmlspecialchars($_POST['add_year']);
            $add_hour = htmlspecialchars($_POST['add_hour']);
            $add_min = htmlspecialchars($_POST['add_min']);

            // TEST NOM
            if ( strlen($_POST['$add_name'] > 50)) {
                $add_error = "Le nom doit être composé uniquement de lettres et de chiffres et de moins de 50 caractères";
            }
            // TEST VILLE 
            else if ( strlen($_POST['$add_ville']) > 50) {
               $add_error = "La ville doit être composée uniquement de lettres et de moins de 50 caractères";
            }
            // TEST ADRESSE 
            else if ( strlen($_POST['$add_adresse']) > 50) {
                $add_error = "L'adresse doit être composée uniquement de lettres et de chiffres et de moins de 50 caractères";
            }
            // TEST DESCRIPTION 
            else if ( strlen($_POST['$add_description']) > 1024) {
                $add_error = "La description doit être composée de moins de 1024 caractères";
            }
            // TEST PRIX 
            else if (!ctype_digit($_POST['add_prix']) || $_POST['add_prix'] <= 0){
               $add_error = "Le prix doit être supérieur ou égal à 0";
            }
             // TEST PLACES
            else if (!ctype_digit($_POST['add_place']) || $_POST['add_place'] <= 0){
               $add_error = "Le nombre de place doit être supérieur à 0";
            }
            // TEST ORG 
            else if ( strlen($_POST['$add_org']) > 50) {
                $add_error = "L'organisateur doit être composé uniquement de lettres et de chiffres et de moins de 50 caractères";
             }
            // TEST DATE
            else if (!ctype_digit($_POST['add_day']) || $_POST['add_day'] < 1 || $_POST['add_day'] > 31 ){
                $add_error = "Le jour doit être entre 1 et 31";
            } else if (!ctype_digit($_POST['add_month']) || $_POST['add_month'] < 1 || $_POST['add_month'] > 12 ){
                $add_error = "Le mois doit être entre 1 et 12";
            } else if (!ctype_digit($_POST['add_year']) || $_POST['add_year'] < 2020 || $_POST['add_year'] > 2029 ){
                $add_error = "L'année doit être entre 2020 et 2029";
            } else if ($_POST['add_month'] == 2 && $_POST['add_day'] > 29 ){
                $add_error = "Fevrier ne comporte que 28 ou 29 jours";
            }
            // TEST HEURE
            else if (!ctype_digit($_POST['add_hour']) || $_POST['add_hour'] < 0 || $_POST['add_hour'] > 23 ){
                $add_error = "L'heure doit être entre 1 et 23";
            } else if (!ctype_digit($_POST['add_min']) || $_POST['add_min'] < 0 || $_POST['add_min'] > 59 ){
                $add_error = "Les minutes doivent être entre 0 et 59";
            }

            // SINON
            else {

                if ($_POST['add_day'] < 10) {
                    $add_day = "0" . $add_day;
                }
                if ($_POST['add_month'] < 10) {
                    $add_month = "0" . $add_month;
                }

                if ($_POST['add_hour'] < 10) {
                    $add_hour = "0" . $add_hour;
                }
                if ($_POST['add_min'] < 10) {
                    $add_min = "0" . $add_min;
                }

                $add_date = $add_year ."-". $add_month ."-". $add_day ." ". $add_hour .":". $add_min .":00";

                $query = 'INSERT INTO party (`nom`, `city`, `adress`, `price`, `slots`, `rest`, `male`, `female`, `other`, `org`, `user_id`, `description`, `date`) VALUES ("'.$add_name.'", "'.$add_ville.'", "'.$add_adresse.'", "'.$add_prix.'", "'.$add_place.'", "'.$add_place.'", "0", "0", "0", "'.$add_org.'", "'.$add_orgID.'", "'.$add_description.'", "'.$add_date.'");';
                echo $query;
                $db->query($query);
            }
        } else {
            $add_error = "Il faut remplir tout les champs";
        }
    }
    
?>
