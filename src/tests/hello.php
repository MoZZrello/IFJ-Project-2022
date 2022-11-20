<?php
declare(strict_types=1);
// Hello World example in IFJ22
// run it on Merlin by: php8.1 ifj22.php hello.php

function hlavni_program(int $year) : string {
  write("Hello from IFJ", $year, "\n");
  return "ano";
}

function pohlavni_program(string $month) : void {
  write("Hello from IFJ", $year, "\n");
}

hlavni_program(22);
hlavni_program(23); // pozdrav z budoucnosti
