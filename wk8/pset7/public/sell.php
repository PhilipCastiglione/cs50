<?php

    // configuration
    require("../includes/config.php"); 

    // if user reached page via GET (as by clicking a link or via redirect)
    if ($_SERVER["REQUEST_METHOD"] == "GET")
    {
        render("sell_form.php", ["title" => "Sell"]);
    }

    // else if user reached page via POST (as by submitting a form via POST)
    else if ($_SERVER["REQUEST_METHOD"] == "POST")
    {
        if (empty($_POST["symbol"]))
        {
            apologize("Please enter a stock symbol.");
        }
        else
        {
                
            $stock = lookup($_POST["symbol"]);
            $position = CS50::query("SELECT shares FROM portfolios WHERE user_id = ? AND symbol = ?", $_SESSION["id"], $stock["symbol"]);
            
            if (count($position) > 0 && $stock)
            {
                $delete = CS50::query("DELETE FROM portfolios WHERE user_id = ? AND symbol = ?", $_SESSION["id"], $stock["symbol"]);
                if ($delete)
                {
                    $proceeds = $position[0]["shares"] * $stock["price"];
                    $update = CS50::query("UPDATE users SET cash = cash + ? WHERE id = ?", $proceeds, $_SESSION["id"]);
                    if ($update)
                    {
                        CS50::query("INSERT INTO transactions (user_id, symbol, qty, price, sale) VALUES(?, ?, ?, ?, 1)", $_SESSION["id"], $stock["symbol"], $position[0]["shares"], $stock["price"]);
                        render("sold.php", ["symbol" => $stock["symbol"], "proceeds" => $proceeds]);
                    }
                    else
                    {
                        apologize("Your shares were sold, but we didn't give you any money for them. Maybe try ETrade?");
                    }
                }
                else
                {
                    apologize("Your shares couldn't be sold rn. Soz.");
                }
            }
            elseif ($stock)
            {
                apologize("You don't have any shares in {$stock["symbol"]}.");
            }
            else
            {
                apologize("The entered stock code {$_POST["symbol"]} wasn't recognized.");
            }
        }
    }

?>