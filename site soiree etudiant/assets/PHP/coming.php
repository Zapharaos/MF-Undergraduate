<?php
    if(isset($_REQUEST['coming_btn'])) {
        if (isset($_SESSION['Auth'])) {
            if (!empty($_GET['party']) && is_numeric($_GET['party'])) {
                $query = 'SELECT COUNT(*) FROM coming WHERE user_id= "'.$_SESSION['Auth']['id'].'" AND party_id= "'.$_GET['party'].'";';
                $rows = $db->query($query)->fetch();
                if ($rows[0] == 0) {
                    $db->query('INSERT INTO coming (`user_id`, `party_id`) VALUES ("'.$_SESSION['Auth']['id'].'", "'.$_GET['party'].'");');
                    $db->query('UPDATE party SET rest=rest-1 WHERE user_id= "'.$_SESSION['Auth']['id'].'" AND id= "'.$_GET['party'].'";');
                    if ($_SESSION['Auth']['gender'] == "male") {
                        $db->query('UPDATE party SET male=male+1 WHERE user_id= "'.$_SESSION['Auth']['id'].'" AND id= "'.$_GET['party'].'";');
                    } else if ($_SESSION['Auth']['gender'] == "female") {
                        $db->query('UPDATE party SET female=female+1 WHERE user_id= "'.$_SESSION['Auth']['id'].'" AND id= "'.$_GET['party'].'";');
                    } else {
                        $db->query('UPDATE party SET other=other+1 WHERE user_id= "'.$_SESSION['Auth']['id'].'" AND id= "'.$_GET['party'].'";');
                    }


                // QR CODE
                    require("phpqrcode/qrlib.php");
                    $nom = 'user_'.$_SESSION['Auth']['id'].'-party_'.$_GET['party'];
                    $path = "assets/QR/".$nom;
                    QRcode::png($nom,$path.".png", $level = QR_ECLEVEL_L, 17);

                // PDF
                    require('fpdf17/fpdf.php');
                    $pdf = new FPDF('P', 'mm', 'a5');
                    $pdf->AddPage();
                    $pdf->Image($path.".png");
                    $pdf->Output($path.".pdf", "F");

                    $sql = $db->query('SELECT nom FROM party WHERE id= "'.$_GET['party'].'";');
                    $partysql = $sql->fetch();

                    if ($lang_url == 'fr') {
                        $coming_sujet = "Inscription à : ";
                        $comingtxt = "Voici la confirmation de votre inscription. Veuillez vous munir de cette confirmation afin de pouvoir y entrer.";
                    } else {
                        $coming_sujet = "Signup to : ";
                        $comingtxt = "Here is the confirmation of your registration. Please have this confirmation in order to be able to come in.";
                    }

                    $coming_text = "<h3 style='text-decoration: underline'>".$coming_sujet.$partysql['nom']."</h3>
                                    <p>" . $comingtxt ."</p><img src='cid:qrcode'></div>";

                    require("sendgrid-php/sendgrid-php.php");

                    $coming_mail = new \SendGrid\Mail\Mail();
                    $coming_mail->setFrom("noreply@studentsparty.com", "noreply@studentsparty.com");
                    $coming_mail->setSubject($coming_sujet . $partysql['nom']);
                    // à remplacer par une adresse mail accessible par l'utilisateur :
                    $coming_mail->addTo($_SESSION['Auth']['email'], $_SESSION['Auth']['email']);
                    $coming_mail->addContent("text/html", $coming_text);
                    $file_encoded = base64_encode(file_get_contents($path.".pdf"));
                    $coming_mail->addAttachment(
                        $file_encoded,
                        "application/pdf",
                        "my_qrcode.pdf",
                        "attachment"
                    );

                    $coming_sendgrid = new \SendGrid('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');
                    
                    try {
                        $response = $coming_sendgrid->send($coming_mail);
                        $coming_success = "Nous avons enregistré votre inscription. Votre place vous a été envoyé par email.";
                    } catch (Exception $e) {
                        echo 'Caught exception: '. $e->getMessage() ."\n";
                    }
                }
            } else {
                $coming_error = "'party' doit être un nombre";
            }
        } else {
            $coming_error = "Vous devez être connecté";
        }
    }

    if(isset($_REQUEST['not_coming_btn'])) {
        if (isset($_SESSION['Auth'])) {
            if (!empty($_GET['party']) && is_numeric($_GET['party'])) {
                $query = 'SELECT COUNT(*) FROM coming WHERE user_id= "'.$_SESSION['Auth']['id'].'" AND party_id= "'.$_GET['party'].'";';
                $rows = $db->query($query)->fetch();
                if ($rows[0] == 1) {
                    $db->query("DELETE FROM coming WHERE user_id= ".$_SESSION['Auth']['id']." AND party_id= ".$_GET['party']);
                    $db->query('UPDATE party SET rest=rest+1 WHERE user_id= "'.$_SESSION['Auth']['id'].'" AND id= "'.$_GET['party'].'";');
                    if ($_SESSION['Auth']['gender'] == "male") {
                        $db->query('UPDATE party SET male=male-1 WHERE user_id= "'.$_SESSION['Auth']['id'].'" AND id= "'.$_GET['party'].'";');
                    } else if ($_SESSION['Auth']['gender'] == "female") {
                        $db->query('UPDATE party SET female=female-1 WHERE user_id= "'.$_SESSION['Auth']['id'].'" AND id= "'.$_GET['party'].'";');
                    } else {
                        $db->query('UPDATE party SET other=other-1 WHERE user_id= "'.$_SESSION['Auth']['id'].'" AND id= "'.$_GET['party'].'";');
                    }
                    
                }
                $coming_success = "Vous avez annulé votre inscription.";
            } else {
                $coming_error = "'party' doit être un nombre";
            }
        } else {
            $coming_error = "Vous devez être connecté";
        }
    }
?>
