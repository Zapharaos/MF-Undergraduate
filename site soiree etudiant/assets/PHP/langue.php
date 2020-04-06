<?php

    // si lang pas défini dans url, alors FR par défaut
    if (!isset($_GET['lang'])){
        $lang_url = 'fr';
    }
    // sinon récupère lang dans l'url
    else {
        $lang_url = $_GET['lang'];
    }

    // si lang différent de FR ou EN, alors FR par défaut
    if ($lang_url != 'fr' && $lang_url != 'en') {
        $lang_url = 'fr';
    }

    if ($lang_url == 'fr') {
        $lang_two = 'en';
    } else {
        $lang_two = 'fr';
    }

    // envoie variable PHP dans une variable JS
    echo "<script>var lang_name='" . $lang_url . "';</script>";
    echo "<script>var lang_two='" . $lang_two . "';</script>";

?>
