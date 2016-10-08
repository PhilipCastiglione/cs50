<h1><?= htmlspecialchars($stock["symbol"]) ?></h1>
<p><?= htmlspecialchars($stock["name"]) ?></p>
<p>Last traded at price: <?= number_format($stock["price"], 4) ?></p>