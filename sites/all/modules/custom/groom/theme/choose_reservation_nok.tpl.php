<div id="messages">
    <div id="reservation-container" class="<?= $container_class ?>">
        <section class="alert error">
            <h1><?= $title ?></h1>
            <div><?= $message ?></div>

            <?php if (!empty($buttons)): ?>
                <br />
                <div class="buttons cf">
                    <?php foreach ($buttons as $button): ?>
                        <a href="<?= $button['href'] ?>" title="<?= $button['title'] ?>" class="btn <?= $button['class'] ?>">
                            <?= $button['title'] ?>
                        </a>
                    <?php endforeach; ?>
                </div>
            <?php endif; ?>
        </section>
    </div>
</div>