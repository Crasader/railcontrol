#!/usr/bin/php
<?php

echo "Starting railcontrol\n";
exec('./startrailcontrol.sh');

// open control socket
$port = 2222;
$address = '::1';
$connection_wait = 100;
while (true) {
	if ($connection_wait == 0) {
		echo "Too many failers, exiting\n";
		exit;
	}
	sleep(1);
	$socket = socket_create(AF_INET6, SOCK_STREAM, SOL_TCP);
	if ($socket === false) {
		echo "Unable to create Socket: " . socket_strerror(socket_last_error()) . "\n";
		$connection_wait--;
		continue;
	}
	$connection = socket_connect($socket, $address, $port);
	if ($connection === false) {
		echo "Unable to connect to [$address]:$port " . socket_strerror(socket_last_error($socket)) . "\n";
		socket_close($socket);
		$connection_wait--;
		continue;
	}
	break;
}
echo "Socket to control opened\n";

// read wellcome message
$data = socket_read($socket, 128);

// list locos
socket_write($socket, "lla\n");
$data = socket_read($socket, 128);
echo $data;

// close control socket
socket_write($socket, "q\n");
socket_close($socket);
echo "Socket to control closed\n";
echo "Waiting for railcontrol shutdown\n";
exec('./stoprailcontrol.sh');

?>
