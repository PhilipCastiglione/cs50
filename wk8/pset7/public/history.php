<?php

    // configuration
    require("../includes/config.php"); 

    $transactions = CS50::query("SELECT symbol, qty, price, timestamp, sale FROM transactions WHERE user_id = ?", $_SESSION["id"]);
    if ($transactions)
    {
        render("history_list.php", ["transactions" => $transactions]);
    }
    else
    {
        apologize("You need to make some transactions before any history will be visible.");
    }

?>
