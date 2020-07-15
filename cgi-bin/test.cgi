#!/usr/local/bin/php
<?php
$_name = getenv('name');
$address = getenv('address');
?>

<!DOCTYPE html>
<html>
	<head>
		<title>Thank you page</title>
		<link rel="stylesheet" href="index.css">
	</head>
	<body>
	<h1>Thank you for filling out my form!</h1>
	Thank you, <?php echo $_name; ?>, for filling out my form!
	An e-mail will be sent to <?php echo $address; ?>
	</body>
</html>