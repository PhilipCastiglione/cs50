<?php

    // configuration
    require("../includes/config.php"); 

    // if user reached page via GET (as by clicking a link or via redirect)
    if ($_SERVER["REQUEST_METHOD"] == "GET")
    {
        render("buy_form.php", ["title" => "Buy"]);
    }

    // else if user reached page via POST (as by submitting a form via POST)
    else if ($_SERVER["REQUEST_METHOD"] == "POST")
    {
        if (empty($_POST["symbol"]))
        {
            apologize("Please enter a stock symbol.");
        }
        elseif (preg_match("/^\d+$/", $_POST["shares"]))
        {
            $number = $_POST["shares"];
            $stock = lookup($_POST["symbol"]);
            if ($stock)
            {
                $cost = $number * $stock["price"];
                $user = CS50::query("SELECT cash FROM users WHERE id = ?", $_SESSION["id"])[0];
                if ($user["cash"] > $cost)
                {
                    $insert = CS50::query("INSERT INTO portfolios (user_id, symbol, shares) VALUES(?, ?, ?) ON DUPLICATE KEY UPDATE shares = shares + VALUES(shares)", $_SESSION["id"], $stock["symbol"], $number);
                    if ($insert)
                    {
                        $update = CS50::query("UPDATE users SET cash = cash - ? WHERE id = ?", $cost, $_SESSION["id"]);
                        if ($update)
                        {
                            CS50::query("INSERT INTO transactions (user_id, symbol, qty, price) VALUES(?, ?, ?, ?)", $_SESSION["id"], $stock["symbol"], $number, $stock["price"]);
                            render("bought.php", ["symbol" => $stock["symbol"], "cost" => $cost, "number" => $number]);
                        }
                        else
                        {
                            apologize("Your shares were bought, but we didn't charge you any money for them. Jackpot!");
                        }
                    }
                    else
                    {
                        apologize("Your shares couldn't be bought rn. Soz. At least we left your cash alone.");
                    }
                }
                else
                {
                    $cash = number_format($user["cash"], 2);
                    $cost = number_format($cost, 2);
                    apologize("Your cash balance of $cash is insufficient to buy $number shares of {$stock["symbol"]}, which would cost $cost.");
                }
            }
            else
            {
                apologize("The entered stock code {$_POST["symbol"]} wasn't recognized.");
            }
        }
        else
        {
            apologize("Somehow you snuck a purchase request for a non positive integer through. Don't do that.");
        }
    }

?>