<?php
    $city_array = array("Strasbourg", "Paris", "Saverne");

    if (isset($_POST["suggestion"])) {
        $city = $_POST["suggestion"];

        if (!empty($city)) {
            foreach ($city_array as $city_array) {
                if (strpos($city_array, $city) !== false ) {
                    echo $city_array;
                    echo "<br>";
                }
            }
        }
    }
?>