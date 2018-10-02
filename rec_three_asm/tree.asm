.data
	new_line: .asciiz "\n"
	error_msg: .asciiz "\nType one of the options above\n"
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

############################################### MENU #######################################################
print_menu:
	addi $s2, $s2, 0 # Flag de inserção usada em insere_rec.
	
	# Imprime o menu inicial do programa.
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
	
	# Realiza interação com usuário, e o direciona para a opção escolhida.
user_input:
	li $v0, 5
	syscall
	
	beq $v0, 1, insert_value     
	beq $v0, 2, search_value
	beq $v0, 3, tree_size
	beq $v0, 4, exit      

	# Caso nao seja nenhuma das opções disponiveis, imprime mensagem de erro e espera novamente a entrada.
	li $v0, 4           
	la $a0, error_msg         
	syscall
	
	j user_input         
############################################### END MENU ###################################################

############################################### INSERE_REC #################################################

insert_value:
	la $s0, ($s5) # s0 recebe o valor da raiz da árvore.
	# Espera entrada do usuário.
	li $v0, 5	
	syscall
	
	move $s1, $v0 # s1 guarda o valor escolhido pelo usuário a ser inserido.

insert_rec:
	# Reserva espaço na pilha para tres palavras, sendo estas o valor atual do nó (s3), o valor
	# anterior do nó (s0) e o endereço de retorno ra.
	addi $sp, $sp, -12
	sw $s0, 0($sp)
	sw $s3, 4($sp)
	sw $ra, 8($sp) 

	# Caso a flag de inserção seja 0 (Ocorre quando raiz == NULL), cria-se o nó.
	beqz $s2, create_node
	
	# Checa o valor do nó atual, se valor a ser inserido > valor do nó, faz-se a recurssão para
	# a direita, se valor a ser inserido <= valor do nó, faz-se a recurssão para a esquerda.
	lw $t0, 0($s0)
	ble  $s1, $t0, rec_left
	bgt $s1, $t0, rec_right
	jr $ra

create_node:
	# Cria o nó alocando-o na memória "HEAP" atravez do gp.
	sw $s1, 0($gp) # 0($gp) é o valor do nó.
	li $t0, 0
	sw $t0, 4($gp) # 4($gp) é o valor de memória do nó a esquerda.
	li $t1, 0
	sw $t1, 8($gp) # 8($gp) é o valor de memória do nó a direita.
	
	# Restaura os endereços da pilha e retira os mesmos.
	lw $s0, 0($sp)
	lw $s3, 4($sp)
	lw $ra, 8($sp) 
	addi $sp, $sp, 12
	
	add $s2, $s2, 1 # Desativa flag de inserção, pois valor ja foi criado.
	addi $gp, $gp, 12 # Incrementa memória "HEAP" para o proximo valor a ser inserido.
	addi $s0, $gp, -12 
	jr $ra
	
rec_left:
	la $s3, ($s0) # Carrega s3 com o endereço do nó atual.
	lw $s0, 4($s0) # Carrega s0 com o endereço do nó da esquerda.
	bnez  $s0, l1 # Verifica se o endereço do nó a esquerda é nulo.
	
	# Caso nulo, carrega o endereço que aponta para o endereço do nó a esquerda, e ativa a flag
	# de inserção.
	lw $s0, 0($sp)
	addi $s2, $zero, 0
	
l1:
	# Caso não seja nulo, entra-se na recursão com o endereço do nó a esquerda.
	jal insert_rec
	
	# Após retorno da recursão, estabelece o endereço do nó a esquerda ( raiz->left  = insert(raiz->esq, valor) ).
	sw $s0, 4($s3)
	# Restaura os endereços da pilha e retira os mesmos.
	lw $s0, 0($sp)
	lw $s3, 4($sp)
	lw $ra, 8($sp) 
	addi $sp, $sp, 12
	jr $ra
	
	
rec_right:
	la $s3, ($s0) # Carrega s3 com o endereço do nó atual.
	lw $s0, 8($s0) # Carrega s0 com o endereço do nó da direita.
	bnez  $s0, l2 # Verifica se o endereço do nó a direita é nulo.
	
	# Caso nulo, carrega o endereço que aponta para o endereço do nó a direita, e ativa a flag
	# de inserção.
	lw $s0, 0($sp)
	addi $s2, $zero, 0
	
l2:
	# Caso não seja nulo, entra-se na recursão com o endereço do nó a direita.
	jal insert_rec
	# Após retorno da recursão, estabelece o endereço do nó a direita ( raiz->dir  = insert(raiz->dir, valor) ).
	sw $s0, 8($s3)
	# Restaura os endereços da pilha e retira os mesmos.
	lw $s0, 0($sp)
	lw $s3, 4($sp)
	lw $ra, 8($sp) 
	addi $sp, $sp, 12
	jr $ra

############################################### END INSERE_REC #############################################

############################################### BUSCA_REC ##################################################
search_value:
	la $s0, ($s5) # s0 recebe o valor da raiz da árvore.
	# Espera entrada do usuário.
	li $v0, 5
	syscall
	
	move $s1, $v0 # s1 guarda o valor escolhido pelo usuário a ser procurado.	    
	
	addi $s3, $s0, 0 
	addi $fp, $sp, -12 # fp carrega o valor de endereço da primeira recurssão feita na pilha.

search_rec:
	# Reserva espaço na pilha para tres palavras, salvando o contexto da recurssao, sendo estas o 
	# valor atual do nó (s3), o valor anterior do nó (s0) e o endereço de retorno ra.
	addi $sp, $sp, -12
	sw $s0, 0($sp)
	sw $s3, 4($sp)
	sw $ra, 8($sp) 
	
	lw $t0, 0($s3) # Carrega o conteúdo do nó atual em t0
	beqz $t0, ret_null # Caso t0 seja nulo (if (raiz == NULL)) retorna-se a nulo.
	beq $s1, $t0, ret_value # Caso t0 seja igual a s1, retorna-se o valor de endereço do nó com o conteudo desejado
	blt $s1, $t0, search_left # Caso valor desejado < t0, faz-se a recurssao para esquerda.
	bgt $s1, $t0, search_right # Caso valor desejado > t0, faz-se a recurssao para direita.
	
	jr $ra
	
ret_null:
	# Restaura os endereços da pilha e retira os mesmos.
	lw $s0, 0($sp)
	lw $s3, 4($sp)
	lw $ra, 8($sp) 
	addi $sp, $sp, 12
	jr $ra
	
search_left:
	lw $s0, 4($s0)	# Carrega o nó a esquerda.
	addi $s3, $s0, 0 
	bnez  $s0, l3 # Se o nó não for nulo, faz-se a recursão para a esquerda.
	lw $s0, 0($sp) # Se o nó for nulo, carrega o endereço que aponta para o nó a esquerda.
	addi $s3, $s0, 4
l3:	
	# Realize recussão
	jal search_rec
	# Após retorno, restaura os endereços da pilha e retira os mesmos.
	lw $s0, 0($sp)
	lw $s3, 4($sp)
	lw $ra, 8($sp) 
	addi $sp, $sp, 12
	jr $ra
	
	
search_right:
	lw $s0, 8($s0)	# Carrega o nó a direita.
	addi $s3, $s0, 0 
	bnez  $s0, l4 # Se o nó não for nulo, faz-se a recursão para a direita.
	lw $s0, 0($sp)  # Se o nó for nulo, carrega o endereço que aponta para o nó a esquerda.
	addi $s3, $s0, 8
l4:	
	# Realize recussão
	jal search_rec
	# Após retorno, restaura os endereços da pilha e retira os mesmos.
	lw $s0, 0($sp)
	lw $s3, 4($sp)
	lw $ra, 8($sp) 
	addi $sp, $sp, 12
	jr $ra

ret_value:
	# Valor desejo encontrado, imprimi-se o seu valor, e seu correspondente endereço do nó.
	# Caso não exista o valor na árvore, nada é imprimido
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
	addi $s7, $zero, 0 # s7 guarda a quantidade e nós da arvore
	la $s0, ($s5) # s0 recebe o valor da raiz da árvore.
	addi $s3, $s0, 0
	addi $fp, $sp, -12 # fp carrega o valor de endereço da primeira recurssão feita na pilha.
tree_rec:
	# Reserva espaço na pilha para tres palavras, salvando o contexto da recurssao, sendo estas o 
	# valor atual do nó (s3), o valor anterior do nó (s0) e o endereço de retorno ra.
	addi $sp, $sp, -12
	sw $s0, 0($sp)
	sw $s3, 4($sp)
	sw $ra, 8($sp) 
	
	lw $t0, 0($s3) # Carrega o conteúdo do nó atual em t0
	beqz $t0, ret_zero  # Caso seja NULL (0), retorna-se zero
	jal get_size_left # Entra em recurssão para esquerda.
	addi $s7, $s7, 1 # Adiciona 1 a quantidade na volta da recurssão das sub árvores.
	jal get_size_right # Entra em recurssão para direita.
	
	b print_size # Ao fim de todas as recurssões, imprime o tamanho.
	
ret_zero:
        # Restaura os endereços da pilha e retira os mesmos.
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
	
	lw $s0, 4($s0)	# Carrega o valor do nó a esquerda
	addi $s3, $s0, 0 
	bnez  $s0, l5 # Se o valor do nó a esquerda não é nulo, realiza-se recurssão a esquerda.
	lw $s0, 0($sp) # Se o valor do nó a esquerda é nulo, carrega o endereço que aponta para o nó da esquerda.
	addi $s3, $s0, 4
l5:	
	jal tree_rec
	# Restaura os endereços da pilha e retira os mesmos.
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
	
	lw $s0, 8($s0) # Carrega o valor do nó a direita
	addi $s3, $s0, 0
	bnez  $s0, l6 # Se o valor do nó a direita não é nulo, realiza-se recurssão a direita.
	lw $s0, 0($sp) # Se o valor do nó a direita é nulo, carrega o endereço que aponta para o nó da direita.
	addi $s3, $s0, 8
l6:	
	jal tree_rec
	# Restaura os endereços da pilha e retira os mesmos.
	lw $s0, 0($sp)
	lw $s3, 4($sp)
	lw $ra, 8($sp) 
	addi $sp, $sp, 12
	jr $ra

print_size:
	bne $fp, $sp, continue_get_size # Verifica se todas as recurssões terminaram, caso não tenha terminado realiza as mesmas
	# Caso recurssão tenha terminado, imprime o resultado.
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
	
############################################### END SIZE_REC ###############################################
exit: 
	li $v0, 10          #Termina o programa Program
	syscall
