<?php
declare(strict_types=1);

$x = readi();

function f(int $x) : void {
 if($x >= 2) {
     $y = $x + 1;
     write($y);
 } else {
     write("Smaller than two!\n");
 }

 $y = $x;

 while($y <= 6) {
     $y = $y + 1;
     write($y);
 }
}

f($x);
