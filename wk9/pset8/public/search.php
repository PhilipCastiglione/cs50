<?php

    require(__DIR__ . "/../includes/config.php");
    
    $raw = $_GET["geo"];
    if (strpos($raw, ",") !== FALSE)
    {
        $terms = explode(",", $raw);
    }
    elseif (strpos($raw, " ") !== FALSE)
    {
        $terms = explode(" " ,$raw);
    }
    else
    {
        $terms = [$raw];
    }

    $places = [];
    
    // could probably reduce this to a single query by chaining ANDs based on the number of $terms,
    // but let's consider that a premature optimisation until we hit WebScale™ ;-)
    foreach ($terms as $term)
    {
        $query = CS50::query("SELECT * FROM places WHERE postal_code LIKE ? OR place_name LIKE ? LIMIT 10", $term . "%", "%" . $term . "%");
        if ($query)
        {
            $places = array_merge($places, $query);
        }
    }

    // output places as JSON (pretty-printed for debugging convenience)
    header("Content-type: application/json");
    print(json_encode($places, JSON_PRETTY_PRINT));

?>