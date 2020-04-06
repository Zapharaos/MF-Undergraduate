<?php
    if(isset($_REQUEST['signin_btn'])) {
        
        if (isset($_POST['signin_name'])
            && isset($_POST['signin_prenom'])
            && isset($_POST['signin_mail'])
            && isset($_POST['signin_gender'])
            && isset($_POST['signin_password'])
            && isset($_POST['signin_confirm'])
            && isset($_POST['signin_day'])
            && isset($_POST['signin_month'])
            && isset($_POST['signin_year'])
            ){
            
            $signin_name = htmlspecialchars($_POST['signin_name'],ENT_QUOTES);
            $signin_prenom = htmlspecialchars($_POST['signin_prenom'],ENT_QUOTES);
            $signin_mail = htmlspecialchars($_POST['signin_mail'],ENT_QUOTES);
            $signin_gender = htmlspecialchars($_POST['signin_gender'],ENT_QUOTES);
            $signin_password = $db->quote($_POST['signin_password']);
            $signin_confirm = $db->quote($_POST['signin_confirm']);
            $signin_day = $_POST['signin_day'];
            $signin_month = $_POST['signin_month'];
            $signin_year = $_POST['signin_year'];
            
        
            // current date :
            $CD = date("j");
            $CM = date("n");
            $CY = date("Y");
            
            $signin_age = $CY - $signin_year;
            if ($signin_month == $CM) {
                if ($CD < $signin_day){
                    $signin_age = $signin_age - 1;
                }
            } else if ($CM < $signin_month) {
                $signin_age = $signin_age - 1;
            }
            
            // TEST NOM
            if ( !ctype_alpha($_POST['signin_name']) || (strlen($_POST['$signin_name']) > 50)) {
                $signin_error = "Le nom doit être composé uniquement de lettres et de moins de 50 caractères";
            }
            // TEST PRENOM
            else if ( !ctype_alpha($_POST['signin_prenom']) || (strlen($_POST['$signin_prenom']) > 50)) {
               $signin_error = "Le prénom doit être composé uniquement de lettres et de moins de 50 caractères";
            }
            // TEST DATE DE NAISSANCE
            else if (!ctype_digit($_POST['signin_day']) || $_POST['signin_day'] < 1 || $_POST['signin_day'] > 31 ){
               $signin_error = "Le jour de naissance doit être entre 1 et 31";
            } else if (!ctype_digit($_POST['signin_month']) || $_POST['signin_month'] < 1 || $_POST['signin_month'] > 12 ){
               $signin_error = "Le mois de naissance doit être entre 1 et 12";
            } else if (!ctype_digit($_POST['signin_year']) || $_POST['signin_year'] < 1950 || $_POST['signin_year'] > 2020 ){
               $signin_error = "L'année de naissance doit être entre 1950 et 2020";
            } else if ($_POST['signin_month'] == 2 && $_POST['signin_day'] > 29 ){
               $signin_error = "Fevrier ne comporte que 28 ou 29 jours";
            }
            // TEST AGE
            else if ($signin_age < 1){
               $signin_error = "L'age ne peut pas etre nul";
            }
            // TEST SEXE
            else if ( !ctype_alpha($_POST['signin_gender']) || ($_POST['signin_gender']!='male' && $_POST['signin_gender']!='female' && $_POST['signin_gender']!='other')){
               $signin_error = "Le sexe doit être 'homme', 'femme' ou 'autre' et composé uniquement de lettres";
            }
            // TEST EMAIL
            else if (!filter_var($_POST['signin_mail'], FILTER_VALIDATE_EMAIL) || strlen($_POST['$signin_mail']) > 50) {
               $signin_error = "L'email doit être composé de moins de 50 caractères, un '@' et un '.'";
            }
            // TEST MDP
            else if (strlen($_POST['signin_password']) < 8){
                $signin_error = "Le mot de passe doit être composé d'au moins 8 caractères";
            } else if (strlen($_POST['signin_password']) > 255){
                $signin_error = "Le mot de passe doit être composé de moins de 255 caractères";
            } else if (ctype_upper($_POST['signin_password'])){
                $signin_error = "Le mot de passe doit être composé d'au moins 1 minuscule";
            } else if (ctype_lower($_POST['signin_password'])){
                $signin_error = "Le mot de passe doit être composé d'au moins 1 majuscule";
            } else if ($_POST['signin_password'] != $_POST['signin_confirm']) {
                $signin_error = "La confirmation est fausse";
            } else {
            
                $query = 'SELECT COUNT(*) FROM users WHERE email="'.$signin_mail.'";';
                $rows = $db->query($query)->fetch();
                $signin_hash = password_hash($signin_password, PASSWORD_DEFAULT);

                if($rows[0] == 0){
                    $db->query('INSERT INTO users (`email`, `name`, `firstname`, `password`, `gender`, `dobday`, `dobmonth`, `dobyear`, `age`, `langue`) VALUES ("'.$signin_mail.'", "'.$signin_name.'", "'.$signin_prenom.'", "'.$signin_hash.'", "'.$signin_gender.'", "'.$signin_day.'", "'.$signin_month.'", "'.$signin_year.'", "'.$signin_age.'", "'.$_GET['lang'].'");');
                    echo "<script>document.location.href = 'index.php?lang=".$lang_url."&show=login'</script>";
                } else {
                    $signin_error = "Email déjà utilisé";
                }
            }
        } else {
            $signin_error = "Il faut remplir tout les champs";
        }
    }
?>
