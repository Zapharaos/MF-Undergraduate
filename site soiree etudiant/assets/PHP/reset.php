<?php
    if(isset($_REQUEST['reset_btn'])) {
        
        $selector = $_GET['selector'];
        $validator = $_GET['validator'];
        $reset_password = $db->quote($_POST['reset_password']);
        $reset_confirmer = $db->quote($_POST['reset_confirmer']);
        $date = date("U");
        
        if (!empty($selector) && ctype_xdigit($selector) && !empty($validator) && ctype_xdigit($validator)){
            
            $reset_select = $db->query("SELECT * FROM pwdReset WHERE pwdResetExpires >= '".$date."';");
            $reset_result = $reset_select->fetch();

            $query = "SELECT COUNT(*) FROM pwdReset WHERE pwdResetExpires >= '".$date."';";
            $rows = $db->query($query)->fetch();
            
            if($rows[0] == 0) {
                $reset_error = "Temps écoulé. Il faut reformuler une requête";
            } else {
                
                $tokenBin = hex2bin($validator);
                $tokenCheck = password_verify($tokenBin, $result['pwdResetToken']);
                
                if (password_verify($tokenBin, $reset_result['pwdResetToken'])){
                    if (isset($reset_password) && isset($reset_confirmer)) {

                        $reset_email = $reset_result['pwdResetEmail'];
                        
                        if (strlen($_POST['reset_password']) < 8){
                            $reset_error = "Le mot de passe doit être composé d'au moins 8 caractères";
                        } else if (strlen($_POST['reset_password']) > 255){
                            $reset_error = "Le mot de passe doit être composé de moins de 255 caractères";
                        } else if (ctype_upper($_POST['reset_password'])){
                            $reset_error = "Le mot de passe doit être composé d'au moins 1 minuscule";
                        } else if (ctype_lower($_POST['reset_password'])){
                            $reset_error = "Le mot de passe doit être composé d'au moins 1 majuscule";
                        } else if ($_POST['reset_password'] != $_POST['reset_confirmer']) {
                            $reset_error = "La confirmation est fausse";
                        } else {
                            $reset_hash = password_hash($reset_password, PASSWORD_DEFAULT);
                            $db->query("UPDATE users SET
                                       password='".$reset_hash."'
                                       WHERE email='".$reset_email."';");
                            $reset_success = "Le mot de passe a bien été réinitialisé.";
                            echo "<script>document.location.href = 'index.php?lang=".$lang_url."&show=login'</script>";
                        }
                    } else {
                        $reset_error = "Il faut remplir tout les champs";
                    }
                } else {
                    $reset_error = "Erreur. Il faut reformuler une requête";
                }
            }
        } else {
            $reset_error = "Erreur. Il faut reformuler une requête";
        }
    }
?>
