<?php
    if(isset($_REQUEST['login_btn'])) {

        if (isset($_POST['login_email'])
            && isset($_POST['login_password']) ){
            
            $login_email = htmlspecialchars($_POST['login_email'],ENT_QUOTES);
            $login_password = $db->quote($_POST['login_password']);
            
            $log_sql = 'SELECT * FROM users WHERE email="'.$login_email.'";';
            $log_data = $db->query($log_sql)->fetch();
            
            $query = 'SELECT COUNT(*) FROM users WHERE email="'.$login_email.'";';
            $rows = $db->query($query)->fetch();
            
            if($rows[0] == 1){
                if(password_verify($login_password, $log_data['password'])){
                    $_SESSION['Auth'] = $log_data;
                    $lang_user = $_SESSION['Auth']['langue'];
                    
                    // current date :
                    $CD = date("j");
                    $CM = date("n");
                    $CY = date("Y");
                    
                    $login_day = $_SESSION['Auth']['dobday'];
                    $login_month = $_SESSION['Auth']['dobmonth'];
                    $login_year = $_SESSION['Auth']['dobyear'];
                    
                    $login_age = $CY - $login_year;
                    if ($login_month == $CM) {
                        if ($CD < $login_day){
                            $login_age = $login_age - 1;
                        }
                    } else if ($CM < $login_month) {
                        $login_age = $login_age - 1;
                    }
                    
                    $zgeag = 'UPDATE users SET age="'.$login_age.'" WHERE dobday="'.$login_day.'");';
                               
                    echo "<script>document.location.href = 'index.php?lang=".$lang_user."&show=base'</script>";
                } else {
                    $login_error = "Mot de passe incorrect";
                }
            } else {
                $login_error = "Email inconnu";
            }
        } else {
            $login_error = "Il faut remplir tout les champs";
        }
    }
?>
