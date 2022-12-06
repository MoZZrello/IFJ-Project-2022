<?php
declare(strict_types=1);
// Hello World example in IFJ22
// run it on Merlin by: php8.1 ifj22.php hello.php

function hlavni_program(?int $year) : float {
  write("Hello from IFJ", $year, "\n");
  while($year < 25){
    write("while|");
    $year = $year + 1;
  }
  return 1.1 + 1;
}

function pohlavni_program(string $month, int $year) : void {
  $var = $month;
  write("Hello from IFJ", $var, "\n");
  return;
}

$tmp = "ab" . "c";
$tmp2 = 1;

if(1>=2){
    write("true");
} else {
    write("false");
}

hlavni_program($tmp2);
$tmp = hlavni_program(23); // pozdrav z budoucnosti
?>