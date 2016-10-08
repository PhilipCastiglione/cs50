<?php

    // configuration
    require("../includes/config.php"); 

    // if user reached page via GET (as by clicking a link or via redirect)
    if ($_SERVER["REQUEST_METHOD"] == "GET")
    {
        render("quote_form.php", ["title" => "Quote"]);
    }

    // else if user reached page via POST (as by submitting a form via POST)
    else if ($_SERVER["REQUEST_METHOD"] == "POST")
    {
        if (empty($_POST["symbol"]))
        {
            apologize("Please enter a stock symbol.");
        }
        else {
            $stock = lookup($_POST["symbol"]);
            
            if ($stock)
            {
                render("stock.php", ["stock" => $stock]);
            }
            else
            {
                apologize("The entered stock code {$_POST["symbol"]} wasn't recognized.");
            }
        }
    }

?>