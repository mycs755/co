.data
list:  .word  3,0,1,2,6,2,4,7,3,7
.text
main:
la  $s2,list
addi $s0,$s0,5
addi $s0,$s0,5
add $s1,$s0,$s0
add $s4,$s1,$s5
lw $s3,$s4($s2)
add $s5,$s3,$s3


