#!/usr/local/bin/php

<?php 
    
    if(isset($name) and !empty($name)){
            $location = 'Users/Othilie/';      
            if(move_uploaded_file($temp_name, $location.$name)){
                echo 'File uploaded successfully';
            }
        } else {
            echo 'You should select a file to upload !!';
        }
?>

<!DOCTYPE html>
<html>
	<head>
		<title>Thank you page</title>
		<link rel="stylesheet" href="index.css">
	</head>
	<body>
	<h1>Thank you for uploading files!</h1>
	</body>
</html>