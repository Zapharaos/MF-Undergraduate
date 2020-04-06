<?php
    if (isset($contact_success)){
        echo "<p data-mlr-text class=success>" . $contact_success . "</p>";
    } else if (isset($forgot_success)){
        echo "<p data-mlr-text class=success>" . $forgot_success . "</p>";
    } else if (isset($reset_success)){
        echo "<p data-mlr-text class=success>" . $reset_success . "</p>";
    } else if (isset($profil_success)){
        echo "<p data-mlr-text class=success>" . $profil_success . "</p>";
    } else if (isset($profil_error)){
        echo "<p data-mlr-text class=error>" . $profil_error . "</p>";
    } else if (isset($coming_success)){
           echo "<p data-mlr-text class=success>" . $coming_success . "</p>";
       }
?>
