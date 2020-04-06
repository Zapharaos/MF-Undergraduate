<?php
    $show = $_GET['show'];
    $id = $_SESSION['Auth']['id'];
    
    if (empty($show)){
        echo "<script> section('base')</script>";
        echo "<script>document.location.href = '/../../index.php?lang=".$lang_url."&show=base'</script>";
    } else if (isset($id) && ($show=="login" || $show=="signin" || $show=="reset_pwd" || $show=="forgot")){
        echo "<script> section('base')</script>";
        echo "<script>document.location.href = '/../../index.php?lang=".$lang_url."&show=base'</script>";
    } else if (!isset($id) && ($show=="add" || $show=="details" || $show=="profil" || $show=="coming")){
        echo "<script> section('base')</script>";
        echo "<script>document.location.href = '/../../index.php?lang=".$lang_url."&show=base'</script>";
    } else if ($show=="add" || $show=="profil" || $show=="login" || $show=="signin" || $show=="base" || $show=="details" || $show=="reset_pwd" || $show=="forgot" || $show=="coming") {
        echo "<script> section('".$show."')</script>";
        echo "<script>document.location.href = '/../../index.php?lang=".$lang_url."&show=".$show."</script>";
    } else {
        echo "<script> section('base')</script>";
        echo "<script>document.location.href = '/../../index.php?lang=".$lang_url."&show=base'</script>";
    } 
?>
