.data
	new_line: .asciiz "\n"
	menu_question: .asciiz "\nSelect one of the options:\n"
	insert_text: .asciiz "[1] - Insert a value to the tree\n"
	search_text: .asciiz "[2] - Search a value in the tree\n"
	size_text: .asciiz "[3] - Get tree size\n"
	exit_text: .asciiz "[4] - Exit program\n"
	value_text: .asciiz "\nThe value "
	address_text: .asciiz  " is located at the address: "
	size_reponse_text: .asciiz "There is "
	size_reponse_text1: .asciiz " nodes in the tree.\n"
.text
	la $s5, ($gp)
	jal print_menu
	
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
	beq $v0, 2, search_value
	beq $v0, 3, tree_size
	beq $v0, 4, exit      

	li $v0, 4           
	la $a0, new_line          
	syscall
	
	j user_input         
	
############################################### INSERE_REC #################################################

insert_value:
	la $s0, ($s5)        # s0 = root address
	li $v0, 5
	syscall
	
	move $s1, $v0

recursion:
	addi $sp, $sp, -12
	sw $s0, 0($sp)
	sw $s3, 4($sp)
	sw $ra, 8($sp) 

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
	lw $s3, 4($sp)
	lw $ra, 8($sp) 
	addi $sp, $sp, 12
	add $s2, $s2, 1
	addi $gp, $gp, 12
	addi $s0, $gp, -12
	jr $ra
	
rec_left:
	la $s3, ($s0)
	lw $s0, 4($s0)
	bnez  $s0, l1
	
	lw $s0, 0($sp)
	addi $s2, $zero, 0
	
l1:
	jal recursion
	sw $s0, 4($s3)
	lw $s0, 0($sp)
	lw $s3, 4($sp)
	lw $ra, 8($sp) 
	addi $sp, $sp, 12
	jr $ra
	
	
rec_right:
	la $s3, ($s0)
	lw $s0, 8($s0)
	bnez  $s0, l2
	
	lw $s0, 0($sp)
	addi $s2, $zero, 0
	
l2:
	jal recursion
	sw $s0, 8($s3)
	lw $s0, 0($sp)
	lw $s3, 4($sp)
	lw $ra, 8($sp) 
	addi $sp, $sp, 12
	jr $ra

############################################### END INSERE_REC #############################################


############################################### BUSCA_REC ##################################################
search_value:
	la $s0, ($s5)        # s0 = root address
	li $v0, 5
	syscall
	
	move $s1, $v0 	     # s1 = value
	

search_rec:
	addi $sp, $sp, -12
	sw $s0, 0($sp)
	sw $s3, 4($sp)
	sw $ra, 8($sp) 
	
	lw $t0, 0($s0)
	beqz $t0, error
	beq $s1, $t0, print_values
	
	blt $s1, $t0, search_left
	bgt $s1, $t0, search_right
	
	jr $ra
	
search_left:
	lw $s0, 4($s0)
	bnez  $s0, l3
	lw $s0, 0($sp)	
l3:	
	j search_rec
	
	
search_right:
	lw $s0, 8($s0)
	bnez  $s0, l4
	lw $s0, 0($sp)
l4:	
	j search_rec
	

	
error:
	jr $ra

print_values:
	li $v0, 4
	la $a0, value_text
	syscall
	
	li $v0, 34
	addi $a0, $s1, 0 
	syscall
	
	li $v0, 4
	la $a0, address_text 
	syscall
	
	li $v0, 34
	addi $a0, $s0, 0 
	syscall
	
	li $v0, 4           
	la $a0, new_line          
	syscall
	
	jr $ra

############################################### END BUSCA_REC ##############################################

############################################### SIZE_REC ###################################################
tree_size:
	addi $s7, $zero, 0
	addi $s3, $s0, 0
	addi $fp, $sp, -12
tree_rec:
	addi $sp, $sp, -12
	sw $s0, 0($sp)
	sw $s3, 4($sp)
	sw $ra, 8($sp) 
	
	lw $t0, 0($s3)
	beqz $t0, ret_zero
	jal get_size_left
	addi $s7, $s7, 1
	jal get_size_right
	
	b print_size
	
ret_zero:
	lw $s0, 0($sp)
	lw $s3, 4($sp)
	lw $ra, 8($sp) 
	addi $sp, $sp, 12
	jr $ra

get_size_left:
	addi $sp, $sp, -12
	sw $s0, 0($sp)
	sw $s3, 4($sp)
	sw $ra, 8($sp) 
	
	lw $s0, 4($s0)	# root->left
	addi $s3, $s0, 0 
	bnez  $s0, l5 #se root->left = 0
	lw $s0, 0($sp) 
	addi $s3, $s0, 4
l5:	
	jal tree_rec

	lw $s0, 0($sp)
	lw $s3, 4($sp)
	lw $ra, 8($sp) 
	addi $sp, $sp, 12
	jr $ra

get_size_right:
	addi $sp, $sp, -12
	sw $s0, 0($sp)
	sw $s3, 4($sp)
	sw $ra, 8($sp) 
	
	lw $s0, 8($s0)
	addi $s3, $s0, 0
	bnez  $s0, l6
	lw $s0, 0($sp)
	addi $s3, $s0, 8
l6:	
	jal tree_rec
	
	lw $s0, 0($sp)
	lw $s3, 4($sp)
	lw $ra, 8($sp) 
	addi $sp, $sp, 12
	jr $ra

print_size:
	bne $fp, $sp, continue_get_size
	li $v0, 4
	la $a0, size_reponse_text
	syscall
	
	li $v0, 1
	addi $a0, $s7, 0
	syscall
		
	li $v0, 4
	la $a0, size_reponse_text1
	syscall

continue_get_size:
	lw $s0, 0($sp)
	lw $s3, 4($sp)
	lw $ra, 8($sp) 
	addi $sp, $sp, 12
	jr $ra
	
############################################### SIZE_REC ###################################################
exit: 
	li $v0, 10          #Terminate Program
	syscall
