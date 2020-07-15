#!/bin/bash
brew install nginx
cp /usr/local/etc/nginx/nginx.conf /usr/local/etc/nginx/nginx_backup.conf
cp testeur.conf /usr/local/etc/nginx/nginx.conf
mkdir /var/www/html/
sudo cp ../www/* /var/www/html/ #changer le port pour les liens!!
sudo mkdir /var/www/cgi-bin
sudo cp ../cgi-bin/test.cgi /var/www/cgi-bin # attention * pour tous les cgi

brew services start php #pour utiliser php fpm
sudo nginx