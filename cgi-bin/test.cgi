#!/usr/local/bin/php
HTTP/1.1 200 OK
Content-type = text/html

<?php
$_name = getenv('name');
?>
<html>
<title>Thank You Page</title>
<h1>Thank you for filling out my form!</h1>
Thank you, <?php echo $_name; ?>, for filling out my form!
</html>