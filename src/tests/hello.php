<?php
declare(strict_types=1);
// Hello World example in IFJ22
// run it on Merlin by: php8.1 ifj22.php hello.php

function hlavni_program(?int $year) : float {
  write("Hello from IFJ", $year, "\n");

  while($year < 25 - 5){
    write("while|");
    $year = $year + 1;
  }

  return 1.1 + 1;
}

$tmp = "ab" . "c";
$tmp2 = 1;

if(1 < 2){
    $var = strlen("true");
} else {
    write("false");
}

$year = 1;
while($year < 25){
   write("while|");
   $year = $year + 1;
 }

hlavni_program($tmp2);
$tmp = hlavni_program(23); // pozdrav z budoucnosti
?>