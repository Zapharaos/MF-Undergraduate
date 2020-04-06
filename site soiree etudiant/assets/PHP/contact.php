<?php
    if(isset($_REQUEST['contact_btn'])) {
        
        if (isset($_POST['contact_name'])
            && isset($_POST['contact_email'])
            && isset($_POST['contact_sujet'])
            && isset($_POST['contact_message'])){
            
            $contact_name = $_POST['contact_name'];
            $contact_email = $_POST['contact_email'];
            $contact_sujet = $_POST['contact_sujet'];
            $contact_message = $_POST['contact_message'];
            
            if (!filter_var($_POST['contact_email'], FILTER_VALIDATE_EMAIL) || strlen($_POST['$contact_email']) > 50) {
               $contact_error = "L'email doit être composé de moins de 50 caractères, un '@' et un '.'";
            } else {
                $contact_send = "De : " . $contact_name .
                        "\nEmail : " . $contact_email .
                        " \n\n\nSujet : " . $contact_sujet .
                        "\n\n" . $contact_message;

                require("sendgrid-php/sendgrid-php.php");

                $contact_mail = new \SendGrid\Mail\Mail();
                $contact_mail->setFrom("noreply@studentsparty.com", "noreply@studentsparty.com");
                $contact_mail->setSubject("" . $contact_sujet);
                // à remplacer par une adresse mail accessible par l'utilisateur :
                $contact_mail->addTo("roiking67@gmail.com", "roiking67@gmail.com");
                $contact_mail->addContent("text/plain", $contact_send);
                $contact_sendgrid = new \SendGrid('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');
                
                try {
                    $response = $contact_sendgrid->send($contact_mail);
                    $contact_success = "Le message nous a bien été transmis et nous vous en remercions. Nous allons y donner suite dans les meilleurs délais.";
                } catch (Exception $e) {
                    echo 'Caught exception: '. $e->getMessage() ."\n";
                }
            }
            
        } else {
            $contact_error = "Il faut remplir tout les champs";
        }
    }
?>
