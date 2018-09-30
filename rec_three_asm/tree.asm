.data
	new_line: .asciiz "\n"
	menu_question: .asciiz "\nSelect one of the options: \n "
	insert_text: .asciiz "[1] - Insert a value to the tree\n"
	search_text: .asciiz "[2] - Search a value in the tree\n"
	size_text: .asciiz "[3] - Get tree size\n"
	exit_text: .asciiz "[4] - Exit program\n"
	
.text

print_menu:
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
	
	move $s0, $v0
	
	beq $s0, 1, insert_value     
	beq $s0, 4, exit      

	li $v0, 4           
	la $a0, new_line          
	syscall
	
	j user_input         

insert_value:
	li $v0, 5
	syscall
	
	move $s1, $v0

recursion:
	subu $sp, $sp, 8
	sw $gp, 0($sp)
	sw $ra, 4($sp) 
	lw $s0, 0($gp) #s0 = root value

	beqz $s0, create_node
	ble  $s1, $s0, rec_left
	bgt $s1, $s0, rec_right
	addi $sp, $sp, 8
	jr $ra

create_node:
	sw $s1, 0($gp)
	sw $s2, 4($gp)
	sw $s3, 8($gp)
	b exit
	
rec_left:

rec_right:

exit: 
	li $v0, 10          #Terminate Program
	syscall