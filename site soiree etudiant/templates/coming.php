<section id="coming">
    <?php
        if ($_GET['show'] === 'coming') {
            if(!empty($_GET['party'])) {
                $coming = $db->query('SELECT * FROM coming WHERE party_id = "'.$_GET['party'].'";');
                $query = 'SELECT COUNT(*) FROM coming WHERE party_id = "'.$_GET['party'].'";';
                $rows = $db->query($query)->fetch();
                if($rows[0] > 0) {
                    $query = "SELECT * FROM users WHERE ";
                    $or = " OR ";
                    $first = true;
                    foreach ($coming as $row) {
                        if ($first) {
                            $query = $query . "id = '" . $row['user_id'] . "'";
                            $first = false;
                        } else {
                            $query = $query . $or . "id = '" . $row['user_id'] . "'";
                        }
                    }
                    $members = $db->query($query);

                    $query = $db->query('SELECT * FROM party WHERE id = "'.$_GET['party'].'";');
                    $nameparty = $query->fetch();
                    echo "<h2>" . $nameparty['nom'] . "</h2>";
                    echo "<ul>";
                    foreach ($members as $row) {
                        echo "<li>" . $row['firstname'] . " " . $row['name'] . "</li>";
                    }
                    echo "</ul>";
                } else {
                    echo "<script>document.location.href='index.php?lang=".$lang_url."&show=base';</script>";
                }
            } else {
                echo "<script>document.location.href='index.php?lang=".$lang_url."&show=base';</script>";
            }
        }
    ?>
</section>
