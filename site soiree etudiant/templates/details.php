<section id="details">
    <?php
        if ($_GET['show'] === 'details') {
            if (!isset($_SESSION['Auth'])) {
                
                echo "<script>document.location.href = '/../../index.php?lang=".$lang_url."&show=base'</script>";
            }
            if(!empty($_GET['party'])) {
                $query = ' FROM party WHERE id = "'.$_GET['party'].'";';
                $rows = $db->query('SELECT COUNT(*)' . $query)->fetch();
                if($rows[0] > 0) {
                    $_PARTY = $db->query('SELECT *' . $query)->fetch();
                } else {
                    echo "<script>document.location.href='index.php?lang=".$lang_url."&show=base';</script>";
                }
            } else {
                echo "<script>document.location.href='index.php?lang=".$lang_url."&show=base';</script>";
            }
        }
    ?>

    <?php
        if (isset($coming_error)){
            echo "<p data-mlr-text class=error>" . $coming_error . "</p>";
        }
    ?>

    <div>
        <?php
            echo "<h3><p>" . $_PARTY['nom'] ."</p>";
            if ($_GET['show'] === 'details') {
                echo "<script> function fav(lang, id, type) { document.location.href='index.php?lang=' + lang + '&show=details&party=' + id + '&' + type; } </script>";

                if (isset($_GET['add'])) {
                    echo "<span id='testfav' onclick='fav(\"" .$lang_url. "\", " .$_PARTY['id'].", \"del\")'> <i style='color:#c94c4c;' class='fas fa-star'></i> </span>";
                    $db->query('DELETE FROM fav WHERE user_id= "'.$_SESSION['Auth']['id'].'" AND party_id= "'.$_GET['party'].'";');
                    $db->query('INSERT INTO fav (`user_id`, `party_id`) VALUES ("'.$_SESSION['Auth']['id'].'", "'.$_GET['party'].'");');
                } else if (isset($_GET['del'])) {
                    echo "<span id='testfav' onclick='fav(\"" .$lang_url. "\", " .$_PARTY['id'].", \"add\")'> <i style='color:white;' class='fas fa-star'></i> </span>";
                    $db->query('DELETE FROM fav WHERE user_id= "'.$_SESSION['Auth']['id'].'" AND party_id= "'.$_GET['party'].'";');
                } else {
                    $count_ids_fav = $db->query('SELECT COUNT(*) FROM fav WHERE user_id= "'.$_SESSION['Auth']['id'].'" AND party_id= "'.$_GET['party'].'";')->fetch();
                    if($count_ids_fav[0] > 0) {
                        echo "<span id='testfav' onclick='fav(\"" .$lang_url. "\", " .$_PARTY['id'].", \"del\")'> <i style='color:#c94c4c;' class='fas fa-star'></i> </span>";
                    } else {
                        echo "<span id='testfav' onclick='fav(\"" .$lang_url. "\", " .$_PARTY['id'].", \"add\")'> <i style='color:white;' class='fas fa-star'></i> </span>";
                    }
                }
            
        ?>
    </div>
    <div>
        <?php
            echo "</h3><div id='infodetails'><div><p>" . $_PARTY['date'] . "</p>";
            if ($lang_url == 'fr') {
                echo "<p>Places : " . $_PARTY['slots'] . "</p></div>
                    <div><p>Entrée : " . $_PARTY['price'] . " €</p>
                    <p>Reste : " . $_PARTY['rest'] . "</p></div></div>";

            } else {
                echo "<p>Slots : " . $_PARTY['slots'] . "</p></div>
                    <div><p>Entry : " . $_PARTY['price'] . " €</p>
                    <p>Left : " . $_PARTY['rest'] . "</p></div></div>";
            }
        ?>
    </div>
    <div id="adresse">
        <p><?php echo $_PARTY['city'] ?></p>
        <p><?php echo $_PARTY['adress'] ?></p>
        <?php
            echo "<a onclick=\"window.open('index.php?lang=".$lang_url."&show=profil&profil=".$_PARTY['user_id']."')\"
            title=\"Lien qui emmène vers le profil de l'organisateur\" >
            " . $_PARTY['org'] . "</a>";
        }
        ?>
    </div>
    <?php
        if ($_GET['show'] === 'details' && !empty($_GET['party'])) {

            $sql = $db->query('SELECT * FROM party WHERE id= "'.$_GET['party'].'";');
            $partysql = $sql->fetch();

            $partymale = (int)$partysql['male'];
            $partyfemale = (int)$partysql['female'];
            $partyother = (int)$partysql['other'];
            $slots = ((int)$partysql['slots'] - (int)$partysql['rest']);

            if ($partymale == 0 && $partyfemale == 0 && $partyother == 0) {
                $partymale = 33;
                $partyfemale = 33;
                $partyother = 33;
            } else {
                $partymale = $partymale / $slots * 100;
                $partyfemale = $partyfemale / $slots * 100;
                $partyother = $partyother / $slots * 100;
            }

            echo "<div class=\"perc\">
                    <div style=\"width:". $partymale ."%\" class=\"man\"></div>
                    <div style=\"width:". $partyfemale ."%\" class=\"others\"></div>
                    <div style=\"width:". $partyother ."%\" class=\"woman\"></div>
                </div>";
    ?>
    <section>
        <p style="width: 32%" id="man" data-mlr-text>Homme</p>
        <p style="width: 32%" id="others" data-mlr-text>Autre</p>
        <p style="width: 32%" id="woman" data-mlr-text>Femme</p>
    </section>
    <p><?php echo $_PARTY['description'] ?></p>
    <?php
        $current_date = date('Y-m-d H:i:s');
        if ($current_date < $_PARTY['date']){
            $count_coming = 'SELECT COUNT(*) FROM coming WHERE user_id= "'.$_SESSION['Auth']['id'].'" AND party_id= "'.$_GET['party'].'";';
            $rows = $db->query($count_coming)->fetch();
            if ($rows[0] == 0) {
                if ($lang_url == "fr" ){
                    echo "<form method='POST'><button class='bouton' name='coming_btn' data-mlr-text>Inscription</button></form>";
                } else {
                    echo "<form method='POST'><button class='bouton' name='coming_btn' data-mlr-text>Registration</button></form>";
                }
            } else {
                if ($lang_url == "fr" ){
                    echo "<form method='POST'><button class='bouton' name='not_coming_btn' data-mlr-text>Désinscription</button></form>";
                } else {
                    echo "<form method='POST'><button class='bouton' name='not_coming_btn' data-mlr-text>Unregister</button></form>";
                }
            }
            
            if ($lang_url == "fr" ){
                echo "<a href=\"index.php?lang=" .$lang_url."&show=coming&party=".$_GET['party']."\" title=\"Lien qui emmène vers la liste des participants\" data-mlr-text>Voir les participants ?</a>";
            } else {
                echo "<a href=\"index.php?lang=" .$lang_url."&show=coming&party=".$_GET['party']."\" title=\"Lien qui emmène vers la liste des participants\" data-mlr-text>See the participants?</a>";
            }
        }
    }
    ?>
</section>
