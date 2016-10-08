<?php

    // configuration
    require("../includes/config.php");

    // if user reached page via GET (as by clicking a link or via redirect)
    if ($_SERVER["REQUEST_METHOD"] == "GET")
    {
        // else render form
        render("register_form.php", ["title" => "Register"]);
    }

    // else if user reached page via POST (as by submitting a form via POST)
    elseif ($_SERVER["REQUEST_METHOD"] == "POST")
    {
        // validate input
        if (empty($_POST["username"]) || empty($_POST["password"]) || empty($_POST["password_confirmation"]))
        {
            apologize("Fill in all the fields yo.");
        }
        elseif ($_POST["password"] != $_POST["password_confirmation"])
        {
            apologize("Your password didn't match your confirmation.");
        }
        else
        {
            $insert = CS50::query("INSERT IGNORE INTO users (username, hash, cash) VALUES(?, ?, 10000.0000)",
                        $_POST["username"], password_hash($_POST["password"], PASSWORD_DEFAULT));
            
            if ($insert == 0)
            {
                apologize("Probably, you tried a username that already exists. Alternatively, the internet might be broken");
            }
            else
            {
                $_SESSION["id"] = CS50::query("SELECT LAST_INSERT_ID() AS id")[0]["id"];
                redirect("/");
            }
        }
    }
    
?>