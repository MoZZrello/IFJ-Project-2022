<?php
declare(strict_types=1);
// Hello World example in IFJ22
// run it on Merlin by: php8.1 ifj22.php hello.php

function hlavni_program(?int $year) : float {
  write("Hello from IFJ", $year, "\n");
  return 1.1 + 1;
}

$tmp = "ab" . "c";
$tmp2 = strlen("truee");

if(1+0 < 2-1){
    $var = strlen("true");
} else {
    write("false");
}

$year = 19;
while($year + 1 < 25){
   write("while|");
   $year = $year + 1;
 }

hlavni_program($tmp2);
$tmp = hlavni_program(23); // pozdrav z budoucnosti
?>