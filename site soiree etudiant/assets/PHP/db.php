<?php
    /*
try {
    $db = new PDO('mysql:host=localhost;dbname=StudentParty', 'root', 'password');
    $db->setAttribute(PDO::ATTR_DEFAULT_FETCH_MODE, PDO::FETCH_ASSOC);
    $db->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_WARNING);
} catch(Exception $e) {
    echo 'Impossible de se connecter à la base de donnée';
    echo $e->getMessage();
    die();
}*/
    try {
        $db = new PDO("sqlite:database.db");
    } catch(PDOException $e) {
        echo 'Impossible de se connecter à la base de donnée';
        echo $e->getMessage();
        die();
    }
?>
