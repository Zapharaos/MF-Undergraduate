<section id="base">
    <div>
        <?php
            if (!empty($_GET['histo'])) {
                echo '<!--';
            }
        ?>

        <div>
            <label for="order" data-mlr-text>Trier par :</label>
            <select name="order" id="order">
                <option value="come" data-mlr-text>Soirées à venir</option>
                <option value="old" data-mlr-text>Soirées passées</option>
                <option value="all" data-mlr-text>Toutes</option>
                <?php
                    if (isset($_SESSION['Auth'])){
                        echo "<option value='me' data-mlr-text>Mes soirées</option>
                        <option value='fav' data-mlr-text>Mes favoris</option>";
                    }
                ?>
            </select>
        </div>

        <?php
            if (!empty($_GET['histo'])) {
                echo '-->';
            }

            echo "<script> $('#order').val(\"".$_GET['order']."\");</script>";
        ?> 

        <form id="search" method="POST">
            <label for="search" data-mlr-text>Rechercher une annonce :</label>
            <input type="search" name="search" aria-label="Rechercher une annonce">
            <button name="search_btn" data-mlr-text>Rechercher</button>
        </form>
    </div>

    <ul>
        <?php
            $id = $_SESSION['Auth']['id'];
            $current_date = date('Y-m-d H:i:s');

            if(isset($_REQUEST['search_btn'])) {
                $search = htmlspecialchars($_POST['search']);
                $party = "WHERE nom LIKE '" . $search . "' OR
                    city LIKE '" . $search . "' OR
                    adress LIKE '" . $search . "' OR
                    org LIKE '" . $search ."';";
            } else if ($_GET['order'] === "me") {
                $party = "WHERE user_id='".$id."';";
            } else if ($_GET['order'] === "fav") {
                $tempo_query = "SELECT * FROM fav WHERE user_id='".$id."';";
                $party = "WHERE ";
                $or = " OR ";
                $first = true;

                foreach ($db->query($tempo_query) as $row) {
                    if ($first) {
                        $party = $party . "id = '" . $row['party_id'] . "'";
                        $first = false;
                    } else {
                        $party = $party . $or . "id = '" . $row['party_id'] . "'";
                    }
                }

            } else if ($_GET['order'] === "old") {
                $party = "WHERE date < '".$current_date."';";
            } else if (!empty($_GET['histo'])) {
                if (isset($_GET['org'])) {
                    $party = "WHERE user_id = '" . $_GET['histo'] ."';";
                } else if (isset($_GET['part'])) {
                    $tempo_query = "SELECT party_id FROM coming WHERE user_id = '" . $_GET['histo'] ."';";
                    $party = "WHERE ";
                    $or = " OR ";
                    $first = true;

                    foreach ($db->query($tempo_query) as $row) {
                        if ($first) {
                            $party = $party . "id = '" . $row['party_id'] . "'";
                            $first = false;
                        } else {
                            $party = $party . $or . "id = '" . $row['party_id'] . "'";
                        }
                    }
                }
            } else if ($_GET['order'] === "all") {
                $party = ";";
            } else {
                $party = "WHERE date > '".$current_date."';";
            }

            $query = "SELECT COUNT(*) FROM party " . $party;
            $rows = $db->query($query)->fetch();
            if ($rows[0] == 0) {
                echo "<p data-mlr-text class=error>Aucun résultat</p>";
            } else {
                $party = "SELECT * FROM party " . $party;
                foreach ($db->query($party) as $row) {
                    echo "
                        <li onmouseenter='show_details(this)' onmouseleave='close_details(this)'>
                            <div>
                                <div>
                                    <h3>" . $row['nom'] .  "</h3>
                                </div>
                                <div>
                                    <p>" . $row['date'] . "</p>";

                    if ($lang_url == 'fr') {
                        echo        "
                                    <p>Entrée : " . $row['price'] . " €</p>
                                    <p id='slots'>Places : " . $row['slots'] ;

                    } else {
                        echo        "
                                    <p>Entry : " . $row['price'] . " €</p>
                                    <p id='slots'>Slots : " . $row['slots'] ;
                    }

                    echo "
                                </div>
                            </div>
                            <div class='about'>
                                <a onclick=\"window.open('index.php?lang=".$lang_url."&show=profil&profil=".$row['user_id']."')\"
                                    title=\"Lien qui emmène vers le profil de l'organisateur\" >
                                    " . $row['org'] . "</a>
                                <p>" . $row['description'] . "</p>";
                    if (isset($_SESSION['Auth'])){
                        echo " <a onclick=\"window.open('index.php?lang=".$lang_url."&show=details&party=".$row['id']."')\" title=\"Lien qui emmène vers la page de la soirée\">";
                                    
                        if ($lang_url == 'fr') {
                            echo        "Plus de détails</a>";

                        } else {
                            echo        "More details</a>";
                        }
                    }
                    
                    echo "
                            </div>
                        </li>
                    ";
                }
            }

        ?>
    </ul>
</section>
