.data
	new_line: .asciiz "\n"
	menu_question: .asciiz "\nSelect one of the options: \n"
	insert_text: .asciiz "[1] - Insert a value to the tree\n"
	search_text: .asciiz "[2] - Search a value in the tree\n"
	size_text: .asciiz "[3] - Get three size\n"
	exit_text: .asciiz "[4] - Exit program\n"
	
.text

print_menu:
	li $v0, 4
	la $a0, menu_question
	syscall
	
	li $v0, 4
	la $a0, search_text
	syscall
	
	li $v0, 4
	la $a0, size_text
	syscall
	
	li $v0, 4
	la $a0, menu_question
	syscall
	
	li $v0, 4
	la $a0, exit_text
	syscall
	
user_input:
	li $v0, 5
	syscall
	
	move $s0, $v0
	
	beq $s0, 1, insert_value    
	beq $s0, 2, search_value    
	beq $s0, 3, three_size     
	beq $s0, 4, exit      

	li $v0, 4           #If you press something random
	la $a0, new_line          #Display new line
	syscall
	
	j user_input          #And ask for a new character
