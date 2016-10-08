<h1>Transaction History</h1>
<table class="stock-table">
    <tr>
        <th>TXN</th>
        <th>DATE/TIME</th>
        <th>SYMBOL</th>
        <th>SHARES</th>
        <th>PRICE</th>
        <th>TOTAL</th>
    </tr>
<?php foreach ($transactions as $transaction): ?>
    <tr>
        <td><?= $transaction["sale"] ? "SELL" : "BUY" ?></td>
        <td><?= $transaction["timestamp"] ?></td>
        <td><?= $transaction["symbol"] ?></td>
        <td><?= number_format($transaction["qty"]) ?></td>
        <td>$<?= number_format($transaction["price"], 2) ?></td>
        <td>$<?= number_format($transaction["price"] * $transaction["qty"], 2) ?></td>
    </tr>
<?php endforeach ?>
</table>
