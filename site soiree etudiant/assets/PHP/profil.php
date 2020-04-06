<?php
    $id = $_SESSION['Auth']['id'];
    $_POST['profil_n'] = $_SESSION['Auth']['name'];
    $_POST['profil_p'] = $_SESSION['Auth']['firstname'];
    $_POST['profil_m'] = $_SESSION['Auth']['email'];

    if(isset($_REQUEST['profil_info_btn'])) {
        if (isset($_POST['profil_name'])
                && isset($_POST['profil_prenom'])
                && isset($_POST['profil_mail'])) {
            
            $profil_name = htmlspecialchars($_POST['profil_name'],ENT_QUOTES);
            $profil_prenom = htmlspecialchars($_POST['profil_prenom'],ENT_QUOTES);
            $profil_mail = htmlspecialchars($_POST['profil_mail'],ENT_QUOTES);
            
             /* TEST NOM */
            if ( !ctype_alpha($_POST['profil_name']) || (strlen($_POST['$profil_name']) > 50)) {
                $profil_error = "Le nom doit être composé uniquement de lettres et de moins de 50 caractères";
            }
            /* TEST PRENOM */
            else if ( !ctype_alpha($_POST['profil_prenom']) || (strlen($_POST['$profil_prenom']) > 50)) {
               $profil_error = "Le prénom doit être composé uniquement de lettres et de moins de 50 caractères";
            }
            /* TEST EMAIL */
            else if (!filter_var($_POST['profil_mail'], FILTER_VALIDATE_EMAIL) || strlen($_POST['$profil_mail']) > 50) {
               $profil_error = "L'email doit être composé de moins de 50 caractères, un '@' et un '.'";
            } else {
            
                $querycount = 'SELECT COUNT(*) FROM users WHERE email="'.$profil_mail.'";';
                $rows = $db->query($querycount)->fetch();
                $query = 'SELECT id FROM users WHERE email="'.$profil_mail.'";';
                $result = $db->query($query)->fetch();

                if($rows[0] == 0 || ($rows[0] == 1 && $id==$result['id'])){
                    $query = 'UPDATE users SET name="'.$profil_name.'", firstname="'.$profil_prenom.'", email="'.$profil_mail.'" WHERE id="'.$id.'";';
                    $db->query($query);
                    $profil_success = "Informations modfiées avec succes.";
                    
                    $query = 'SELECT * FROM users WHERE id="'.$_SESSION['Auth']['id'].'";';
                    $_SESSION['Auth'] = $db->query($query)->fetch();
                    echo "<script> location.reload(true);</script>";
                } else {
                    $profil_error = "Email déjà utilisé";
                }
            }
            
        } else {
            $profil_error = "Il faut remplir tout les champs";
        }
        
        $profil_info_error = $profil_error;
    }
    
    if(isset($_REQUEST['profil_pwd_btn'])) {
        if (isset($_POST['profil_current'])
            && isset($_POST['profil_password'])
            && isset($_POST['profil_confirm'])) {
            
            $password = $_SESSION['Auth']['password'];
            $profil_current = $db->quote($_POST['profil_current']);
            $profil_password = $db->quote($_POST['profil_password']);
            $profil_confirm = $db->quote($_POST['profil_confirm']);
            $profil_c_hash = password_hash($profil_current, PASSWORD_DEFAULT);
            
            if (!password_verify($profil_current, $password)) {
                $profil_error = "Mot de passe incorrect";
            } else if (strlen($_POST['profil_password']) < 8){
                $profil_error = "Le mot de passe doit être composé d'au moins 8 caractères";
            } else if (strlen($_POST['profil_password']) > 255){
                $profil_error = "Le mot de passe doit être composé de moins de 255 caractères";
            } else if (ctype_upper($_POST['profil_password'])){
                $profil_error = "Le mot de passe doit être composé d'au moins 1 minuscule";
            } else if (ctype_lower($_POST['profil_password'])){
                $profil_error = "Le mot de passe doit être composé d'au moins 1 majuscule";
            } else if ($_POST['profil_password'] != $_POST['profil_confirm']) {
                $profil_error = "La confirmation est fausse";
            } else {
                $profil_password = password_hash($profil_password, PASSWORD_DEFAULT);
                $query = 'UPDATE users SET password="'.$profil_password.'" WHERE id="'.$id.'";';
                $db->query($query);
                $profil_success = "Mot de passe modfié avec succes.";
                
                $query = 'SELECT * FROM users WHERE id="'.$_SESSION['Auth']['id'].'";';
                $_SESSION['Auth'] = $db->query($query)->fetch();
                echo "<script> location.reload(true);</script>";
            }
        } else {
            $profil_error = "Il faut remplir tout les champs";
        }
        
        $profil_pwd_error = $profil_error;
    }
    
?>
