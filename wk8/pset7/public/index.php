<?php

    // configuration
    require("../includes/config.php"); 

    $user_id = $_SESSION["id"];
    $user = CS50::query("SELECT username, cash FROM users WHERE id = ?", $user_id)[0];
    $portfolio_rows = CS50::query("SELECT symbol, shares FROM portfolios WHERE user_id = ?", $user_id);
    
    $positions = [];
    $total = 0;
    foreach ($portfolio_rows as $row)
    {
        $stock = lookup($row["symbol"]);
        if ($stock !== false)
        {
            $positions[] = [
                "name" => $stock["name"],
                "price" => $stock["price"],
                "shares" => $row["shares"],
                "symbol" => $row["symbol"]
            ];
            $total += $stock["price"] * $row["shares"];
        }
    }
        
    // render portfolio
    $args = [
        "title" => "Portfolio",
        "positions" => $positions,
        "portfolio_total" => $total,
        "user" => $user
    ];
    render("portfolio.php", $args);

?>
