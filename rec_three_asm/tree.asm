.data
	new_line: .asciiz "\n"
	menu_question: .asciiz "\nSelect one of the options: \n "
	insert_text: .asciiz "[1] - Insert a value to the tree\n"
	search_text: .asciiz "[2] - Search a value in the tree\n"
	size_text: .asciiz "[3] - Get tree size\n"
	exit_text: .asciiz "[4] - Exit program\n"
	
.text

print_menu:
	addi $s2, $s2, 0
	
	li $v0, 4
	la $a0, menu_question
	syscall
	
	li $v0, 4
	la $a0, insert_text
	syscall
	
	li $v0, 4
	la $a0, search_text
	syscall
	
	li $v0, 4
	la $a0, size_text
	syscall
	
	li $v0, 4
	la $a0, exit_text
	syscall
	
user_input:
	li $v0, 5
	syscall
	
	beq $v0, 1, insert_value     
	beq $v0, 4, exit      

	li $v0, 4           
	la $a0, new_line          
	syscall
	
	j user_input         

insert_value:
	la $s0, ($gp)        # s0 = root address
	li $v0, 5
	syscall
	
	move $s1, $v0

recursion:
	addi $sp, $sp, -16
	sw $s0, 0($sp)
	sw $gp, 4($sp)
	sw $ra, 8($sp) 
	sw $s3, 12($sp)
	

	beqz $s2, create_node
	
	lw $t0, 0($s0)
	ble  $s1, $t0, rec_left
	bgt $s1, $t0, rec_right
	jr $ra

create_node:
	sw $s1, 0($gp)
	li $t0, 0
	sw $t0, 4($gp)
	li $t1, 0
	sw $t1, 8($gp)
	
	lw $s0, 0($sp)
	lw $gp, 4($sp)
	lw $ra, 8($sp) 
	lw $s3, 12($sp)
	addi $sp, $sp, 16
	add $s2, $s2, 1
	jr $ra
	
rec_left:
	la $s3, ($s0)
	lw $s0, 4($s0)
	bnez  $s0, l1
	
	lw $s0, 0($sp)
	addi $s2, $zero, 0
	
l1:
	addi $gp, $gp, 12
	jal recursion
	
	sw $gp, 4($s3)
	lw $s0, 0($sp)
	lw $gp, 4($sp)
	lw $ra, 8($sp) 
	lw $s3, 12($sp)
	addi $sp, $sp, 16
	jr $ra
	
	
rec_right:
	la $s3, ($s0)
	lw $s0, 8($s0)
	bnez  $s0, l2
	
	lw $s0, 0($sp)
	addi $s2, $zero, 0
	
l2:
	addi $gp, $gp, 12
	jal recursion
	
	sw $gp, 8($s3)
	lw $s0, 0($sp)
	lw $gp, 4($sp)
	lw $ra, 8($sp) 
	lw $s3, 12($sp)
	addi $sp, $sp, 16
	jr $ra

exit: 
	li $v0, 10          #Terminate Program
	syscall
