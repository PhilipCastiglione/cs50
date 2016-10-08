<?php

    // configuration
    require("../includes/config.php");

    // if user reached page via GET (as by clicking a link or via redirect)
    if ($_SERVER["REQUEST_METHOD"] == "GET")
    {
        render("password_form.php", ["title" => "Update Password"]);
    }

    // else if user reached page via POST (as by submitting a form via POST)
    elseif ($_SERVER["REQUEST_METHOD"] == "POST")
    {
        // validate input
        if (empty($_POST["password"]) || empty($_POST["password_confirmation"]))
        {
            apologize("Fill in all the fields yo.");
        }
        elseif ($_POST["password"] != $_POST["password_confirmation"])
        {
            apologize("Your password didn't match your confirmation.");
        }
        else
        {
            $update = CS50::query("UPDATE users SET hash = ? WHERE id = ?", password_hash($_POST["password"], PASSWORD_DEFAULT), $_SESSION["id"]);
            if ($update == 0)
            {
                apologize("For some reason we couldn't update your password. Probably because of Trump somehow.");
            }
            else
            {
                render("password_confirmation.php", ["title" => "Password Confirmation"]);
            }
        }
    }
    
?>