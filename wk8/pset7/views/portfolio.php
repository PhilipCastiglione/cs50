<h1>Hello, <?= $user["username"] ?></h1>
<p>This is the current state of your portfolio.</p>
<table class="stock-table">
    <tr>
        <th>SYMBOL</th>
        <th>NAME</th>
        <th>SHARES</th>
        <th>PRICE</th>
        <th>TOTAL</th>
    </tr>
<?php foreach ($positions as $position): ?>
    <tr>
        <td><?= $position["symbol"] ?></td>
        <td><?= $position["name"] ?></td>
        <td><?= number_format($position["shares"]) ?></td>
        <td>$<?= number_format($position["price"], 4) ?></td>
        <td>$<?= number_format($position["shares"] * $position["price"], 2) ?></td>
    </tr>
<?php endforeach ?>
</table>
<p>Total portfolio value: $<?= number_format($portfolio_total, 2) ?></p>
<p>Cash position: $<?= number_format($user["cash"], 2) ?></p>